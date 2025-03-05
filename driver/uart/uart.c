
/*******************************************************************************
*文件名：UART.c
*作    用： 用于实现串口功能的接口函数
*版    本：Version 1.0  创建于2018.05.08
*******************************************************************************/
#include <string.h>
#include "UART.h"

char gUartBuffer[UART_MAX_BUFLEN];
static void uart_delay(UINT32 loop)
{
	while(loop--){
		asm("	nop");
	}
}
/***************************************************************************
函数名称：INT32 bspUartSend(UINT32 dev,UINT32 channel,UINT8 *pSendData,UINT32 sendLen,UINT32 wait)
函数功能：使用指定UART通道发送指定长度数据
输入变量：UINT32 dev：UART控制器号(0)
          UINT32 channel：UART通道号
          UINT8 *pSendData：发送数据指针
          UINT32 sendLen：发送数据长度
          UINT32 wait：是否等待发送全部完成标志，0-不等待，1-等待
输出变量：无
返回值：非负值-已成功发送的数据长度，若wait为1，返回值非负但与sendLen不等，则为等待超时，返回超时前成功发送的数据长度
        RET_RARAM1_ERROR-发送数据指针为空
        RET_RARAM2_ERROR-等待标志不在有效范围
        RET_ERROR-操作失败
约束条件：必须在bspUartInit初始化后调用
*****************************************************************************/
INT32 bspUartSend(INT8 *pSendData,UINT32 sendLen,UINT32 wait)
{
	INT32 i;
	INT32 timeout;
	if(!pSendData){
		return RET_RARAM1_ERROR;
	}
	if(wait>1){
		return RET_RARAM2_ERROR;
	}

	for(i=0;i<sendLen;i++){
		/* 查询发送FIFO是否非满 */
		timeout = 5000;
		while((StatusReg & STATUS_TF) == STATUS_TF){
			if((wait == 1) && (timeout-- > 0)){
				uart_delay(1000);
				continue;
			}
			else{
				return i;
			}
		}
		DataReg = pSendData[i];
	}
	return i;
}
/******************************************************************************
*函数名：UART_Receive(char *DestData_Addr)
*功    能： 串口接收数据
*参    数：DestData_Addr表示需要接收的数据存放的地址。
*******************************************************************************/
void  UART_Receive(char *DestData_Addr)
{
	char *p;
	int i=0;
	p = DestData_Addr;
	while(1)                                          //一直处于接收状态，设置串口助手为发送时，
	{												  //要注意设置发送间隔时间，如>=2毫秒
		while((StatusReg&0xfc000000)<0x04000000){};  //查询状态寄存器是否有有效数据
		*p = DataReg;
		p = p+1;
		i = i+1;
		if(i == 1296)
			break;
	}
}
/*******************************************************************************
*函数名：UART_Print(Uint32 BaudRate, const char *strFmt)
*功    能： 串口打印字符串
*参    数：BaudRate为波特率，*strFmt代表要传输的字符串。
*******************************************************************************/
void UART_Print(char *strFmt)
{
	 bspUartSend(strFmt,strlen(strFmt),1);
}

/*******************************************************************************
*函数名：UART_Config()
*功    能： 开UART时钟函数
*参    数：无
*******************************************************************************/
void UART_Config(unsigned int BaudRate)
{
	unsigned int temp11,temp22;

	PSC_Open_Clk("UART",0);
	temp11 = BaudRate*8*4;
	temp22= MAIN_PLL/temp11;
	ScalerReg=(temp22*10- 5 )/10;
	ControlReg=(1<<0)|(1<<1)|(1<<2)|(1<<3);	// 接收和发送使能 并且打开接收和发送中断,（注意：如果不打开接收中断，则接收无法完成。）
}

void bspPrintf(const char *strFmt,int data0,int data1,int data2,int data3,int data4,int data5)
{
    sprintf(gUartBuffer,strFmt,data0,data1,data2,data3,data4,data5);
	UART_Print(gUartBuffer);
}
