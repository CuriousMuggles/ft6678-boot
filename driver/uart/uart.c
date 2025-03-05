
/*******************************************************************************
*�ļ�����UART.c
*��    �ã� ����ʵ�ִ��ڹ��ܵĽӿں���
*��    ����Version 1.0  ������2018.05.08
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
�������ƣ�INT32 bspUartSend(UINT32 dev,UINT32 channel,UINT8 *pSendData,UINT32 sendLen,UINT32 wait)
�������ܣ�ʹ��ָ��UARTͨ������ָ����������
���������UINT32 dev��UART��������(0)
          UINT32 channel��UARTͨ����
          UINT8 *pSendData����������ָ��
          UINT32 sendLen���������ݳ���
          UINT32 wait���Ƿ�ȴ�����ȫ����ɱ�־��0-���ȴ���1-�ȴ�
�����������
����ֵ���Ǹ�ֵ-�ѳɹ����͵����ݳ��ȣ���waitΪ1������ֵ�Ǹ�����sendLen���ȣ���Ϊ�ȴ���ʱ�����س�ʱǰ�ɹ����͵����ݳ���
        RET_RARAM1_ERROR-��������ָ��Ϊ��
        RET_RARAM2_ERROR-�ȴ���־������Ч��Χ
        RET_ERROR-����ʧ��
Լ��������������bspUartInit��ʼ�������
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
		/* ��ѯ����FIFO�Ƿ���� */
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
*��������UART_Receive(char *DestData_Addr)
*��    �ܣ� ���ڽ�������
*��    ����DestData_Addr��ʾ��Ҫ���յ����ݴ�ŵĵ�ַ��
*******************************************************************************/
void  UART_Receive(char *DestData_Addr)
{
	char *p;
	int i=0;
	p = DestData_Addr;
	while(1)                                          //һֱ���ڽ���״̬�����ô�������Ϊ����ʱ��
	{												  //Ҫע�����÷��ͼ��ʱ�䣬��>=2����
		while((StatusReg&0xfc000000)<0x04000000){};  //��ѯ״̬�Ĵ����Ƿ�����Ч����
		*p = DataReg;
		p = p+1;
		i = i+1;
		if(i == 1296)
			break;
	}
}
/*******************************************************************************
*��������UART_Print(Uint32 BaudRate, const char *strFmt)
*��    �ܣ� ���ڴ�ӡ�ַ���
*��    ����BaudRateΪ�����ʣ�*strFmt����Ҫ������ַ�����
*******************************************************************************/
void UART_Print(char *strFmt)
{
	 bspUartSend(strFmt,strlen(strFmt),1);
}

/*******************************************************************************
*��������UART_Config()
*��    �ܣ� ��UARTʱ�Ӻ���
*��    ������
*******************************************************************************/
void UART_Config(unsigned int BaudRate)
{
	unsigned int temp11,temp22;

	PSC_Open_Clk("UART",0);
	temp11 = BaudRate*8*4;
	temp22= MAIN_PLL/temp11;
	ScalerReg=(temp22*10- 5 )/10;
	ControlReg=(1<<0)|(1<<1)|(1<<2)|(1<<3);	// ���պͷ���ʹ�� ���Ҵ򿪽��պͷ����ж�,��ע�⣺������򿪽����жϣ�������޷���ɡ���
}

void bspPrintf(const char *strFmt,int data0,int data1,int data2,int data3,int data4,int data5)
{
    sprintf(gUartBuffer,strFmt,data0,data1,data2,data3,data4,data5);
	UART_Print(gUartBuffer);
}
