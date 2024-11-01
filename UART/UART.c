
/*******************************************************************************
*�ļ�����UART.c
*��    �ã� ����ʵ�ִ��ڹ��ܵĽӿں���
*��    ����Version 1.0  ������2018.05.08
*******************************************************************************/
#include <string.h>
#include "UART.h"

char gUartBuffer[UART_MAX_BUFLEN];

/*******************************************************************************
*��������UART_Print(Uint32 BaudRate, const char *strFmt)
*��    �ܣ� ���ڴ�ӡ�ַ���
*��    ����BaudRateΪ�����ʣ�*strFmt����Ҫ������ַ�����
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
*��������UART_Config()
*��    �ܣ� ��UARTʱ�Ӻ���
*��    ������
*******************************************************************************/
void UART_Config(unsigned int BaudRate)
{
	unsigned int temp11,temp22;
	PSC_MDCTL26=0x00000003;                         //ʹ��ģ��
	*(unsigned int *)PSC_PTCMD=0x00000001;          //��ʱ��
	while(!((PSC_MDSTAT26 & 0x00001003)==0x00001003))
	{};
	temp11 = BaudRate*8*4;
	temp22= MAIN_PLL/temp11;
	ScalerReg=(temp22*10- 5 )/10;
	ControlReg=0x220A;                //UART����Ϊ���ⲿ��������
}

void bspPrintf(const char *strFmt,int data0,int data1,int data2,int data3,int data4,int data5)
{
    sprintf(gUartBuffer,strFmt,data0,data1,data2,data3,data4,data5);
	UART_Print(gUartBuffer);
}
