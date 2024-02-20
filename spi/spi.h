/*******************************************************************************
    File        : spi.h
    Description : This file provides user spi API
    Version     : 1.0
    Author      : ldy
    Date        : 2023/02/05
*******************************************************************************/
#ifndef	__SPI_H__
#define	__SPI_H__

/********************************************************************************
	�궨��
********************************************************************************/
//#define SPIDBG(fmt, ...)	printf("[%s %d @%d]:"fmt, __FUNCTION__, __LINE__,g_dnum,  ##__VA_ARGS__)
#define SPI_DEV_NUM		(1)
#define SPI_MODE_LEN(x)	(x-1)	/*SPI�����ֳ���0b0000-32λ�ֳ���0b11~0b1111��4λ~16λ�ֳ�*/
#define SPI_SCS0		(2)		/*Ƭѡ0*/
#define SPI_SCS1		(1)		/*Ƭѡ1*/
#define SPI_FIFO_DEPTH	(128)
/********************************************************************************
	�Ĵ�������
********************************************************************************/
#define SPI_CAPABILITY		*(volatile unsigned int *)0x20BF0000
#define SPI_MODE			*(volatile unsigned int *)0x20BF0020
#define SPI_EVENT			*(volatile unsigned int *)0x20BF0024
#define SPI_COMMAND			*(volatile unsigned int *)0x20BF002c
#define SPI_TRANSMIT		*(volatile unsigned int *)0x20BF0030
#define SPI_RECEIVE			*(volatile unsigned int *)0x20BF0034
#define SPI_SLVSEL			*(volatile unsigned int *)0x20BF0038
#define SPI_AUTOSLVSEL		*(volatile unsigned int *)0x20BF003c
#define SPI_SPIINT0			*(volatile unsigned int *)0x20BF0600
#define SPI_SPIIVL			*(volatile unsigned int *)0x20BF0604

/********************************************************************************
	�ṹ�嶨��
********************************************************************************/
typedef struct{
	UINT8 loop;		/*�ػ�ģʽ��0-����ģʽ��1-�ػ�ģʽ*/
	UINT8 cpol;		/*ʱ�Ӽ��ԣ�0-���е͵�ƽ��1-���иߵ�ƽ*/
	UINT8 cpha;		/*ʱ����λ��0-��һ�������ض�����1-��һ�������ظ���*/
	UINT8 rev;		/*���ݴ��䷽ʽ��0-���λ���ȴ��䣬1-���λ���ȴ���*/
	UINT8 ms;		/*�������ã�0-�ӣ�1-��*/
	UINT8 len;		/*�����ֳ�*/
}SPI_MODE_CONFIG;

/********************************************************************************
	��������
********************************************************************************/
//INT32 spiTransfer(UINT32 dev,UINT32 cs,UINT8 *wrData,UINT8 *rdData,UINT32 wrLen);
INT32 spiTransfer(UINT32 dev,UINT32 cs,UINT8 *wrData,UINT32 wrLen,UINT8 *rdData,UINT32 rdLen);
INT32 bspSpiInit(UINT32 dev);
#endif
