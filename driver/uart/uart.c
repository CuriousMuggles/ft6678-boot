
/*******************************************************************************
*文件名：UART.c
*作    用： 用于实现串口功能的接口函数
*版    本：Version 1.0  创建于2018.05.08
*******************************************************************************/
#include <string.h>
#include "UART.h"
#include "bspInterface.h"

char gUartBuffer[UART_MAX_BUFLEN];
static void uart_delay(UINT32 loop)
{
	while(loop--){
		asm("	nop");
	}
}
/***************************************************************************
函数名称：INT32 bspUartSend(UINT8 *pSendData,UINT32 sendLen,UINT32 wait)
函数功能：使用指定UART通道发送指定长度数据
输入变量： UINT8 *pSendData：发送数据指针
          UINT32 sendLen：发送数据长度
          UINT32 wait：是否等待发送全部完成标志，0-不等待，1-等待
输出变量：无
返回值：非负值-已成功发送的数据长度，若wait为1，返回值非负但与sendLen不等，则为等待超时，返回超时前成功发送的数据长度
        RET_RARAM1_ERROR-发送数据指针为空
        RET_RARAM2_ERROR-等待标志不在有效范围
        RET_ERROR-操作失败
约束条件：必须在bspUartInit初始化后调用
*****************************************************************************/
INT32 bspUartSend(const INT8 *pSendData,UINT32 sendLen,UINT32 wait)
{
	INT32 i;
	INT32 timeout;

	PARAMETER_ASSERT(pSendData,return RET_RARAM1_ERROR);
	PARAMETER_ASSERT(wait<2,return RET_RARAM2_ERROR);

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
/***************************************************************************
函数名称：INT32 bspUartRecv(UINT8 *pRecvData,UINT32 recvLen,UINT32 wait)
函数功能：使用指定UART通道接收指定长度数据
输入变量： UINT8 *pRecvData：接收数据指针
          UINT32 recvLen：接收数据长度
          UINT32 wait：是否等待接收全部完成标志，0-不等待，1-等待
输出变量：无
返回值：非负值-已成功接收的数据长度，若wait为1，返回值非负但与sendLen不等，则为等待超时，返回超时前成功发送的数据长度
        RET_RARAM1_ERROR-接收数据指针为空
        RET_RARAM2_ERROR-等待标志不在有效范围
        RET_ERROR-操作失败
约束条件：必须在bspUartInit初始化后调用
*****************************************************************************/
INT32 bspUartRecv(UINT8 *pRecvData,UINT32 recvLen,UINT32 wait)
{
	INT32 i;

	PARAMETER_ASSERT(pRecvData,return RET_RARAM1_ERROR);
	PARAMETER_ASSERT(wait<2,return RET_RARAM2_ERROR);

	for(i=0;i<recvLen;i++){
		while((StatusReg >> 6 & 0x3f) == 0){
			/* 接收FIFO为空 */
			if(wait == 0){
				return i;
			}
			else{
				uart_delay(1000);
			}
		}
		pRecvData[i] = DataReg;
	}
	return i;
}
/*******************************************************************************
*函数名：void bspUartPrintString(const char *strFmt
*功    能： 串口打印字符串
*参    数：无
*******************************************************************************/
void bspUartPrintString(const char *strFmt)
{
	 bspUartSend(strFmt,strlen(strFmt),1);
}
/***************************************************************************
函数名称：INT32 bspUartInit(UINT32 baudRate)
函数功能：复位并初始化指定通道，配置波特率
输入变量： UINT32 baudRate:UART波特率（bps）
输出变量：无
返回值：无
约束条件：无
*****************************************************************************/
void bspUartInit(unsigned int BaudRate)
{
	UINT32 temp11,temp22;

	PSC_Open_Clk("UART",0);
	temp11 = BaudRate*8*4;
	temp22= MAIN_PLL/temp11;
	ScalerReg=(temp22*10- 5 )/10;
	ControlReg=(1<<0)|(1<<1)|(1<<2)|(1<<3);	// 接收和发送使能 并且打开接收和发送中断,（注意：如果不打开接收中断，则接收无法完成。）
}

