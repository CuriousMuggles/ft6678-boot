/*******************************************************************************
    File        : spiCmdLib.h
    Description : This file provides user spi API
    Version     : 1.0
    Author      : ldy
    Date        : 2024/10/30
*******************************************************************************/
#ifndef	__SPICMDLIB_H__
#define	__SPICMDLIB_H__

#include "bspInterface.h"
/********************************************************************************
	宏定义
********************************************************************************/
#define SPI_FRAME_CMD_READ			(0)
#define SPI_FRAME_CMD_WRITE			(1)
#define FRAME_MAX_LENGTH			(10)
/********************************************************************************
	寄存器定义
********************************************************************************/
typedef enum{
	TEST,
	MARK_ID,
	CHIP_BIT,
	BOOT_VER1,
	BOOT_VER2,
	BOOT_VER3,
	BSP_VER1,
	BSP_VER2,
	BSP_VER3,
	RTC1,
	RTC2,
	WD_CFG,
	WD_FEED,
	WD_ENABLE,
	DSP_RST,
	DSP_RST_TYPE,
	FPGA_RELOAD,
	SPI_RESERVE1,
	DSP1_TEMP,
	DSP2_TEMP,
	DSP3_TEMP,
	DSP4_TEMP,
	FPGA1_TEMP,
	FPGA2_TEMP,
	RIOA_TEMP,
	RIOB_TEMP,
	FMQL_TEMP,
	DSP_FLASH_HADDR = 0x40,//DSP FLASH高为地址线控制，0-31
	FPGA_FLASH_HADDR,//V7 FLASH高位地址线控制，0-7
	FPGA_UPDATE_FLAG,//V7升级标志，写1使能升级，写0正常启动
}SPI_BRAM_REG;
#define SPI_BRAM_REG(reg)			(4 * reg)
/********************************************************************************
	结构体定义
********************************************************************************/

/********************************************************************************
	函数声明
********************************************************************************/
INT32 bspSpiRegRead(UINT32 regaddr,UINT32 *val);
INT32 bspSpiRegWrite(UINT32 regaddr,UINT32 val);
void softInfoToFpga(void);
INT32 dspFlashAddrSwitch(UINT32 flashBlockNo);

#endif/*__SPICMDLIB_H__*/
