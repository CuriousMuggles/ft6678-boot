
/*******************************************************************************
*�ļ�����UART.h
*��    �ã� ��������������صĽӿں����ͼĴ����궨�塣
*��    ����Version 1.0  ������2018.05.08
*******************************************************************************/
#ifndef UART_H_
#define UART_H_

#define	DataReg          *(unsigned char*)0x02540000    //���ݼĴ������ֵ
#define	ControlReg 	     *(unsigned int* )0x02540008    //���ƼĴ������ֵ
#define	StatusReg 	     *(unsigned int* )0x02540004    //״̬�Ĵ������ֵ
#define	ScalerReg	     *(unsigned int* )0x0254000c    //��Ƶ�����Ĵ������ֵ
#define	SartAddress	      (unsigned char*)0x0c00b000    //�������ݵ���ʼ��ַ

#define MAIN_PLL         1000000000                     //оƬ������Ƶ
#define BaudRate_Value   115200                         //UART�Ĳ�����

#define PSC_PTCMD        0x02350120                     //��Դ����ת������Ĵ���
#define PSC_MDCTL26      *(unsigned int*)0x02350a68     //ģ����ƼĴ���
#define PSC_MDSTAT26     *(unsigned int*)0x02350868     //ģ��״̬�Ĵ���

#define UART_MAX_BUFLEN	(1024)

extern void  UART_Print(const char *strFmt);
extern void  UART_Config(unsigned int BaudRate);
void bspPrintf(const char *strFmt,int data0,int data1,int data2,int data3,int data4,int data5);

#endif /* UART_H_ */
