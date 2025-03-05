/*******************************************************************************
    File        : spi.c
    Description : This file provides user spi API
    Version     : 1.0
    Author      : ldy
    Date        : 2023/02/05
*******************************************************************************/
#include <tistdtypes.h>
#include <string.h>
#include "bspInterface.h"
#include "spi.h"
#include "driver/psc/psc.h"

static SPI_MODE_CONFIG gSpiDevModeConfig[SPI_DEV_NUM]={
		{
				0,
				1,
				1,
				1,
				1,
				SPI_MODE_LEN(8),
		}
};
static void spiDelay(unsigned int count)
{
	int j;
	for(j=0;j<=count;j++)
		asm("	nop");
}
/***************************************************************************
�������ƣ�UINT32 isSpiRecvFifoEmpty(UINT32 dev)
�������ܣ��жϽ���FIFO�Ƿ�Ϊ��
���������UINT32 dev:SPI�豸��
�����������
����ֵ��0-�ǿգ�1-��
Լ����������
*****************************************************************************/
static UINT32 isSpiRecvFifoEmpty(UINT32 dev)
{
	return (1<<9) != (SPI_EVENT & (1<<9));
}
/***************************************************************************
�������ƣ�UINT32 waitSpiSendComplete(UINT32 dev)
�������ܣ��ȴ�SPI�������
���������UINT32 dev:SPI�豸��
�����������
����ֵ��0-������1-��
Լ����������
*****************************************************************************/
static UINT32 waitSpiSendComplete(UINT32 dev)
{
	SPI_COMMAND = 0xffffffff;   //LST��1
	while( !(0x00004000 == (SPI_EVENT & (1<<14)))){}; //LT�Ƿ�Ϊ1
	SPI_EVENT |= (1<<14); //�����Ӧ�¼���־
}
/***************************************************************************
�������ƣ�static INT32 spiMasterInit(UINT32 dev)
�������ܣ�SPI ��ģʽ��ʼ������
���������UINT32 dev:SPI�豸��
�����������
����ֵ��RET_SUCCESS-�ɹ�
       RET_RARAM1_ERROR-��������
Լ����������
*****************************************************************************/
static INT32 spiMasterInit(UINT32 dev)
{
	SPI_MODE_CONFIG *pSpiMode;
	UINT32 temp = 0x010f2004;
	
//	PARAMETER_ASSERT((dev < SPI_DEV_NUM),RET_RARAM1_ERROR);
	
	pSpiMode = &gSpiDevModeConfig[dev];
	SPI_MODE &= ~(1<<24);/*ʹ�ܹر�*/
	
	temp |= (pSpiMode->loop & 1) << 30;
	temp |= (pSpiMode->cpol & 1) << 29;
	temp |= (pSpiMode->cpha & 1) << 28;
	temp |= (pSpiMode->rev & 1) << 26;
	temp |= (pSpiMode->ms & 1) << 25;
	temp |= (pSpiMode->len & 0xf) << 20;
	
//	SPIDBG("spi mode config 0x%08x\n",temp);
	SPI_MODE = temp;
	return RET_SUCCESS;
}
/***************************************************************************
�������ƣ�void spiSlaveSelectEnable(UINT32 spiConfigAddr,UINT32 cs_bit)
�������ܣ�SPI�ֶ�Ƭѡʹ��
���������UINT32 dev��SPI�豸��
        UINT32 cs_bit��Ƭѡͨ��
�����������
����ֵ����
Լ����������
*****************************************************************************/
static void spiSlaveSelectEnable(UINT32 dev,UINT32 cs_bit)
{
	switch(cs_bit){
		case 0:SPI_SLVSEL = SPI_SCS0;break;
		case 1:SPI_SLVSEL = SPI_SCS1;break;
		default:SPI_SLVSEL = SPI_SCS0;break;
	}
}
/***************************************************************************
�������ƣ�void spiSlaveSelectDisable(UINT32 dev)
�������ܣ�SPI�ֶ�Ƭѡȥʹ��
���������UINT32 dev��SPI�豸��
�����������
����ֵ����
Լ����������
*****************************************************************************/
static void spiSlaveSelectDisable(UINT32 dev)
{
	SPI_SLVSEL = 0x3;
}
/***************************************************************************
�������ƣ�void spiClearRecvFifo(UINT32 dev)
�������ܣ�SPI����FIFO��λ
���������UINT32 dev��SPI�豸��
�����������
����ֵ����
Լ����������
*****************************************************************************/
static void spiClearRecvFifo(UINT32 dev)
{
	UINT32 temp;
	
	while (!isSpiRecvFifoEmpty(dev)) 
	{
		temp = SPI_RECEIVE;
	}
}
/***************************************************************************
�������ƣ�INT32 spiTransfer(UINT32 spiConfigAddr,UINT32 cs,UINT8 *wrData,UINT32 wrLen,UINT8 *rdData,UINT32 rdLen)
�������ܣ�SPI���亯��
���������spiConfigAddr��SPI��������ַ
          cs���ӻ�Ƭѡ
          wrData�����������ݴ�ŵ�ַ
          wrLen�����������ݳ���
          rdLen������ȡ���ݳ���
���������rdData����ȡ���ݴ�ŵ�ַ
����ֵ��RET_SUCCESS���������
        RET_ERROR������ʧ��
        RET_RARAM3_ERROR�����������ݵ�ַΪ��
        RET_RARAM4_ERROR�����ͺͶ�ȡ���ݳ���֮�ʹ���SPI FIFO���
        RET_RARAM5_ERROR����ȡ���ݴ�ŵ�ַΪ��
Լ����������
*****************************************************************************/
INT32 spiTransfer(UINT32 dev,UINT32 cs,UINT8 *wrData,UINT32 wrLen,UINT8 *rdData,UINT32 rdLen)
{
    UINT32 i,temp=0;

//    PARAMETER_ASSERT((wrData != NULL),RET_RARAM3_ERROR);
//    PARAMETER_ASSERT(((wrLen + rdLen) <= SPI_FIFO_DEPTH),RET_RARAM4_ERROR);
//    PARAMETER_ASSERT((rdData != NULL),RET_RARAM5_ERROR);
    spiClearRecvFifo(dev);
    spiSlaveSelectEnable(dev,cs);
    
    for(i = 0 ; i < (wrLen+rdLen); i++){
    	if(i<wrLen){
			if(gSpiDevModeConfig[dev].rev){
				SPI_TRANSMIT = *(wrData+i) << 24;
			}
			else{
				SPI_TRANSMIT = *(wrData+i);
			}
    	}
    	else{
    		SPI_TRANSMIT = 0;
    	}
//            SPIDBG("write %d/%d:0x%x\n",i,wrLen,*(wrData+i));
    }
    waitSpiSendComplete(dev);
    spiSlaveSelectDisable(dev);

    if(rdLen == 0){
    	return RET_SUCCESS;
    }
    for(i = 0 ; i < (wrLen+rdLen); i++){
    	if(i<wrLen){
    		temp = SPI_RECEIVE;
    	}
    	else{
			if(gSpiDevModeConfig[dev].rev){
				*(rdData + i - wrLen) = SPI_RECEIVE >> 16 & 0xff;
			}
			else{
				*(rdData + i - wrLen) = SPI_RECEIVE >> 8 & 0xff;
			}
    	}
    }

    return RET_SUCCESS;
}
/***********************************************************************************
 * �������ƣ�INT32 bspSpiInit(UINT32 dev)
 * �������ܣ�SPI��������ʼ��
 * ���������UINT32 dev ��������
 * �����������
 * ����ֵ��    RET_SUCCESS �C �ɹ�
 *			RET_ERROR �C ʧ��
 * Լ�������������ڱ��ڵ�ΪSPI Master���������֧�ֶ��̲߳�������
 ***********************************************************************************/
INT32 bspSpiInit(UINT32 dev)
{
	PSC_Open_Clk("SPI");
	spiMasterInit(dev);
}

#define TEST_LENGTH 16
void bspSpiTest(void)
{
	UINT8 sendbuf[TEST_LENGTH],recvbuf[TEST_LENGTH];
	int i=0;
	bspSpiInit(0);

	while(1){
		sendbuf[0] = 0xa5;
		sendbuf[1] = 3;
		sendbuf[2] = 0;
		sendbuf[3] = 1;
		sendbuf[4] = 1;
		sendbuf[5] = 1;
		memset(recvbuf,0,sizeof(recvbuf));
		spiTransfer(0,0,sendbuf,sendbuf[1]+3,recvbuf,3);
	}

}
