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
	宏定义
********************************************************************************/
//#define SPIDBG(fmt, ...)	printf("[%s %d @%d]:"fmt, __FUNCTION__, __LINE__,g_dnum,  ##__VA_ARGS__)
#define SPI_DEV_NUM		(1)
#define SPI_MODE_LEN(x)	(x-1)	/*SPI传输字长，0b0000-32位字长，0b11~0b1111：4位~16位字长*/
#define SPI_SCS0		(2)		/*片选0*/
#define SPI_SCS1		(1)		/*片选1*/
#define SPI_FIFO_DEPTH	(128)
/********************************************************************************
	寄存器定义
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
	结构体定义
********************************************************************************/
typedef struct{
	UINT8 loop;		/*回环模式，0-正常模式，1-回环模式*/
	UINT8 cpol;		/*时钟极性，0-空闲低电平，1-空闲高电平*/
	UINT8 cpha;		/*时钟相位，0-第一个跳变沿读数，1-第一个跳变沿改数*/
	UINT8 rev;		/*数据传输方式，0-最低位优先传输，1-最高位优先传输*/
	UINT8 ms;		/*主从配置，0-从，1-主*/
	UINT8 len;		/*传输字长*/
}SPI_MODE_CONFIG;

/********************************************************************************
	函数声明
********************************************************************************/
//INT32 spiTransfer(UINT32 dev,UINT32 cs,UINT8 *wrData,UINT8 *rdData,UINT32 wrLen);
INT32 spiTransfer(UINT32 dev,UINT32 cs,UINT8 *wrData,UINT32 wrLen,UINT8 *rdData,UINT32 rdLen);
INT32 bspSpiInit(UINT32 dev);
#endif
