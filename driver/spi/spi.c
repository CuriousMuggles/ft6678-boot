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
函数名称：UINT32 isSpiRecvFifoEmpty(UINT32 dev)
函数功能：判断接收FIFO是否为空
输入变量：UINT32 dev:SPI设备号
输出变量：无
返回值：0-非空，1-空
约束条件：无
*****************************************************************************/
static UINT32 isSpiRecvFifoEmpty(UINT32 dev)
{
	return (1<<9) != (SPI_EVENT & (1<<9));
}
/***************************************************************************
函数名称：UINT32 waitSpiSendComplete(UINT32 dev)
函数功能：等待SPI发送完成
输入变量：UINT32 dev:SPI设备号
输出变量：无
返回值：0-非满，1-满
约束条件：无
*****************************************************************************/
static UINT32 waitSpiSendComplete(UINT32 dev)
{
	SPI_COMMAND = 0xffffffff;   //LST置1
	while( !(0x00004000 == (SPI_EVENT & (1<<14)))){}; //LT是否为1
	SPI_EVENT |= (1<<14); //清除对应事件标志
}
/***************************************************************************
函数名称：static INT32 spiMasterInit(UINT32 dev)
函数功能：SPI 主模式初始化函数
输入变量：UINT32 dev:SPI设备号
输出变量：无
返回值：RET_SUCCESS-成功
       RET_RARAM1_ERROR-参数错误
约束条件：无
*****************************************************************************/
static INT32 spiMasterInit(UINT32 dev)
{
	SPI_MODE_CONFIG *pSpiMode;
	UINT32 temp = 0x010f2004;
	
//	PARAMETER_ASSERT((dev < SPI_DEV_NUM),RET_RARAM1_ERROR);
	
	pSpiMode = &gSpiDevModeConfig[dev];
	SPI_MODE &= ~(1<<24);/*使能关闭*/
	
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
函数名称：void spiSlaveSelectEnable(UINT32 spiConfigAddr,UINT32 cs_bit)
函数功能：SPI手动片选使能
输入变量：UINT32 dev：SPI设备号
        UINT32 cs_bit：片选通道
输出变量：无
返回值：无
约束条件：无
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
函数名称：void spiSlaveSelectDisable(UINT32 dev)
函数功能：SPI手动片选去使能
输入变量：UINT32 dev：SPI设备号
输出变量：无
返回值：无
约束条件：无
*****************************************************************************/
static void spiSlaveSelectDisable(UINT32 dev)
{
	SPI_SLVSEL = 0x3;
}
/***************************************************************************
函数名称：void spiClearRecvFifo(UINT32 dev)
函数功能：SPI接收FIFO复位
输入变量：UINT32 dev：SPI设备号
输出变量：无
返回值：无
约束条件：无
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
函数名称：INT32 spiTransfer(UINT32 spiConfigAddr,UINT32 cs,UINT8 *wrData,UINT32 wrLen,UINT8 *rdData,UINT32 rdLen)
函数功能：SPI传输函数
输入变量：spiConfigAddr：SPI控制器地址
          cs：从机片选
          wrData：待发送数据存放地址
          wrLen：待发送数据长度
          rdLen：待读取数据长度
输出变量：rdData：读取数据存放地址
返回值：RET_SUCCESS：操作完成
        RET_ERROR：操作失败
        RET_RARAM3_ERROR：待发送数据地址为空
        RET_RARAM4_ERROR：发送和读取数据长度之和大于SPI FIFO深度
        RET_RARAM5_ERROR：读取数据存放地址为空
约束条件：无
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
 * 函数名称：INT32 bspSpiInit(UINT32 dev)
 * 函数功能：SPI控制器初始化
 * 输入变量：UINT32 dev 控制器号
 * 输出变量：无
 * 返回值：    RET_SUCCESS – 成功
 *			RET_ERROR – 失败
 * 约束条件：适用于本节点为SPI Master的情况，不支持多线程并发调用
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
