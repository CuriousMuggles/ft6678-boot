
/*******************************************************************************
*文件名：UART.c
*作    用： 用于实现串口功能的接口函数
*版    本：Version 1.0  创建于2018.05.08
*******************************************************************************/
#include <string.h>
#include "UART.h"

char gUartBuffer[UART_MAX_BUFLEN];

/*******************************************************************************
*函数名：UART_Print(Uint32 BaudRate, const char *strFmt)
*功    能： 串口打印字符串
*参    数：BaudRate为波特率，*strFmt代表要传输的字符串。
*******************************************************************************/
void UART_Print(const char *strFmt)
{
	unsigned int i,length;
    length = strlen(strFmt);
    for (i=0;i<length;i++)
    {
    	DataReg=strFmt[i];
    }
 }

/*******************************************************************************
*函数名：UART_Config()
*功    能： 开UART时钟函数
*参    数：无
*******************************************************************************/
void UART_Config(unsigned int BaudRate)
{
	unsigned int temp11,temp22;
	PSC_MDCTL26=0x00000003;                         //使能模块
	*(unsigned int *)PSC_PTCMD=0x00000001;          //开时钟
	while(!((PSC_MDSTAT26 & 0x00001003)==0x00001003))
	{};
	temp11 = BaudRate*8*4;
	temp22= MAIN_PLL/temp11;
	ScalerReg=(temp22*10- 5 )/10;
	ControlReg=0x220A;                //UART配置为向外部发送数据
}

void bspPrintf(const char *strFmt,int data0,int data1,int data2,int data3,int data4,int data5)
{
    sprintf(gUartBuffer,strFmt,data0,data1,data2,data3,data4,data5);
	UART_Print(gUartBuffer);
}
