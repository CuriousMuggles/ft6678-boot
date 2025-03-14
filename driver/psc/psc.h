
/*******************************************************************************
*文件名：PSC.h
*作    用： 用于声明开时钟接口函数和寄存器宏定义。
*版    本：Version 1.0  创建于2018.03.26
*******************************************************************************/
#ifndef   _PSC_H_
#define   _PSC_H_

/***************PSC 接口函数声明***************************************************/
extern void PSC_Open_Clk(const char *strName);
extern void PSC_Close_Clk(const char *string);
extern void PSC_Open_Power(const char *string);
extern void PSC_Close_Power(const char *string);

/***************PSC 寄存器*******************************************************/
#define PSC_BASE            0X02350000
#define PTCMD_REG           0X02350120
#define PTSTAT_REG	        0X02350128

/***************PSC 电源时钟寄存器**************************************************/
#define Power_SMC_offset    0x31c
#define Power_Core0_offset  0x320
#define Power_Core1_offset  0x324
#define Power_Core2_offset  0x328
#define Power_Core3_offset  0x32c
#define Power_Core4_offset  0x330
#define Power_Core5_offset  0x334
#define Power_Core6_offset  0x338
#define Power_Core7_offset  0x33c

/***************PSC 模块时钟寄存器**************************************************/
#define DDR3_offset         0xa08
#define EMIF32_offset       0xa0c
#define GMAC_offset         0xa20
#define PCIe_offset         0xa28
#define SRIO0_offset        0xa2c
#define SRIO1_offset        0xa34
#define MSMC_offset         0xa38

#define Core0_offset        0xa3c
#define Core1_offset        0xa40
#define Core2_offset        0xa44
#define Core3_offset        0xa48
#define Core4_offset        0xa4c
#define Core5_offset        0xa50
#define Core6_offset        0xa54
#define Core7_offset        0xa58

#define FFT_offset          0xa5c
#define SPI_offset          0xa60
#define I2C_offset          0xa64
#define UART_offset         0xa68
#define Timer_offset        0xa6c
#define M1553B_offset       0xa70
#define DMA0_offset         0xa74
#define DMA1_offset         0xa78
#define DMA2_offset         0xa7c

#endif
