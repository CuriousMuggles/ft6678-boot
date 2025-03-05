
/*******************************************************************************
*文件名：PSC.c
*作    用： 设置电源、时钟相关接口函数。
*版    本：Version 1.0  创建于2018.03.26
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "PSC.h"

/*******************************************************************************
*函数名：PSC_Open_Clk(const char *string)
*功    能： 开时钟接口函数
*参    数：string表示准备开启时钟的区域
*******************************************************************************/
void PSC_Open_Clk(const char *string)
{
	int *psc_unit;
	int *psc_com  =(int*)PTCMD_REG;
	int *psc_check=(int*)PTSTAT_REG;
	if (strcmp(string ,"DDR3") == 0)
	{
		psc_unit=(int*)(PSC_BASE+DDR3_offset);
	}
	else if (strcmp(string ,"EMIF32") == 0)
	{
		psc_unit=(int*)(PSC_BASE+EMIF32_offset);
	}
	else if(strcmp(string ,"GMAC") == 0)
	{
		psc_unit=(int*)(PSC_BASE+GMAC_offset);
	}
	else if (strcmp(string ,"PCIe") == 0)
	{
		psc_unit=(int*)(PSC_BASE+PCIe_offset);
	}
	else if (strcmp(string ,"SRIO0") == 0)
	{
		psc_unit=(int*)(PSC_BASE+SRIO0_offset);
	}
	else if (strcmp(string ,"SRIO1") == 0)
	{
		psc_unit=(int*)(PSC_BASE+SRIO1_offset);
	}
	else if (strcmp(string ,"MSMC") == 0)
	{
		psc_unit=(int*)(PSC_BASE+MSMC_offset);
	}
	else if (strcmp(string ,"Core0") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Core0_offset);
	}
	else if (strcmp(string ,"Core1") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Core1_offset);
	}
	else if (strcmp(string ,"Core2") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Core2_offset);
	}
	else if (strcmp(string ,"Core3") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Core3_offset);
	}
	else if (strcmp(string ,"Core4") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Core4_offset);
	}
	else if (strcmp(string ,"Core5") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Core5_offset);
	}
	else if (strcmp(string ,"Core6") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Core6_offset);
	}
	else if (strcmp(string ,"Core7") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Core7_offset);
	}
	else if (strcmp(string ,"FFT") == 0)
	{
		psc_unit=(int*)(PSC_BASE+FFT_offset);
	}
	else if (strcmp(string ,"SPI") == 0)
	{
		psc_unit=(int*)(PSC_BASE+SPI_offset);
	}
	else if (strcmp(string ,"I2C") == 0)
	{
		psc_unit=(int*)(PSC_BASE+I2C_offset);
	}
	else if (strcmp(string ,"UART") == 0)
	{
		psc_unit=(int*)(PSC_BASE+UART_offset);
	}
	else if (strcmp(string ,"Timer") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Timer_offset);
	}
	else if (strcmp(string ,"M1553B") == 0)
	{
		psc_unit=(int*)(PSC_BASE+M1553B_offset);
	}
	else if (strcmp(string ,"DMA0") == 0)
	{
		psc_unit=(int*)(PSC_BASE+DMA0_offset);
	}
	else if (strcmp(string ,"DMA1") == 0)
	{
		psc_unit=(int*)(PSC_BASE+DMA1_offset);
	}
	else if (strcmp(string ,"DMA2") == 0)
	{
		psc_unit=(int*)(PSC_BASE+DMA2_offset);
	}
	*psc_unit=3;
	*psc_com=1;
	int temp;
	do
	{
		temp=*psc_check;
	}
	while(temp!=0);

	//different from demo of PSC
	//while (((*(unsigned int *) PSC_MDSTAT2) & 0x1003) != 0x1003);

}

/*******************************************************************************
*函数名：PSC_Close_Clk(const char *string)
*功    能： 关时钟接口函数
*参    数：string表示准备关闭时钟的区域
*******************************************************************************/
void PSC_Close_Clk(const char *string)
{
	int *psc_unit;
	int *psc_com  =(int*)PTCMD_REG;
	int *psc_check=(int*)PTSTAT_REG;
	if (strcmp(string ,"DDR3") == 0)
	{
		psc_unit=(int*)(PSC_BASE+DDR3_offset);
	}
	else if (strcmp(string ,"EMIF32") == 0)
	{
		psc_unit=(int*)(PSC_BASE+EMIF32_offset);
	}
	else if(strcmp(string ,"GMAC") == 0)
	{
		psc_unit=(int*)(PSC_BASE+GMAC_offset);
	}
	else if (strcmp(string ,"PCIe") == 0)
	{
		psc_unit=(int*)(PSC_BASE+PCIe_offset);
	}
	else if (strcmp(string ,"SRIO0") == 0)
	{
		psc_unit=(int*)(PSC_BASE+SRIO0_offset);
	}
	else if (strcmp(string ,"SRIO1") == 0)
	{
		psc_unit=(int*)(PSC_BASE+SRIO1_offset);
	}
	else if (strcmp(string ,"MSMC") == 0)
	{
		psc_unit=(int*)(PSC_BASE+MSMC_offset);
	}
	else if (strcmp(string ,"Core0") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Core0_offset);
	}
	else if (strcmp(string ,"Core1") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Core1_offset);
	}
	else if (strcmp(string ,"Core2") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Core2_offset);
	}
	else if (strcmp(string ,"Core3") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Core3_offset);
	}
	else if (strcmp(string ,"Core4") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Core4_offset);
	}
	else if (strcmp(string ,"Core5") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Core5_offset);
	}
	else if (strcmp(string ,"Core6") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Core6_offset);
	}
	else if (strcmp(string ,"Core7") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Core7_offset);
	}
	else if (strcmp(string ,"FFT") == 0)
	{
		psc_unit=(int*)(PSC_BASE+FFT_offset);
	}
	else if (strcmp(string ,"SPI") == 0)
	{
		psc_unit=(int*)(PSC_BASE+SPI_offset);
	}
	else if (strcmp(string ,"I2C") == 0)
	{
		psc_unit=(int*)(PSC_BASE+I2C_offset);
	}
	else if (strcmp(string ,"UART") == 0)
	{
		psc_unit=(int*)(PSC_BASE+UART_offset);
	}
	else if (strcmp(string ,"Timer") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Timer_offset);
	}
	else if (strcmp(string ,"M1553B") == 0)
	{
		psc_unit=(int*)(PSC_BASE+M1553B_offset);
	}
	else if (strcmp(string ,"DMA0") == 0)
	{
		psc_unit=(int*)(PSC_BASE+DMA0_offset);
	}
	else if (strcmp(string ,"DMA1") == 0)
	{
		psc_unit=(int*)(PSC_BASE+DMA1_offset);
	}
	else if (strcmp(string ,"DMA2") == 0)
	{
		psc_unit=(int*)(PSC_BASE+DMA2_offset);
	}
	*psc_unit=0;
	*psc_com=1;
	int temp;
	do
	{
		temp=*psc_check;
	}
	while(temp!=0);
}

/*******************************************************************************
*函数名：PSC_Open_Power(const char *string)
*功    能： 开电源区域接口函数
*参    数：string表示准备开启电源的区域
*******************************************************************************/
void PSC_Open_Power(const char *string)
{
	int temp;
	int *psc_unit;
	int *psc_com  =(int*)PTCMD_REG;
	int *psc_check=(int*)PTSTAT_REG;
	if (strcmp(string ,"SMC") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_SMC_offset);
		*psc_unit=1;
		*psc_com=0x80;
	}
	else if (strcmp(string ,"Core0") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_Core0_offset);
		*psc_unit=1;
		*psc_com=0x100;
	}
	else if (strcmp(string ,"Core1") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_Core1_offset);
		*psc_unit=1;
		*psc_com=0x200;
	}
	else if (strcmp(string ,"Core2") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_Core2_offset);
		*psc_unit=1;
		*psc_com=0x400;
	}
	else if (strcmp(string ,"Core3") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_Core3_offset);
		*psc_unit=1;
		*psc_com=0x800;
	}
	else if (strcmp(string ,"Core4") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_Core4_offset);
		*psc_unit=1;
		*psc_com=0x1000;
	}
	else if (strcmp(string ,"Core5") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_Core5_offset);
		*psc_unit=1;
		*psc_com=0x2000;
	}
	else if (strcmp(string ,"Core6") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_Core6_offset);
		*psc_unit=1;
		*psc_com=0x4000;
	}
	else if (strcmp(string ,"Core7") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_Core7_offset);
		*psc_unit=1;
		*psc_com=0x8000;
	}
	do
	{
		temp=*psc_check;
	}
	while((temp)!=0);
}

/*******************************************************************************
*函数名：PSC_Close_Power(const char *string)
*功    能： 关电源区域接口函数
*参    数：string表示准备关闭电源的区域
*******************************************************************************/
void PSC_Close_Power(const char *string)
{
	int temp;
	int *psc_unit;
	int *psc_com  =(int*)PTCMD_REG;
	int *psc_check=(int*)PTSTAT_REG;

	if (strcmp(string ,"SMC") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_SMC_offset);
		*psc_unit=0;
		*psc_com=0x80;
	}
	else if (strcmp(string ,"Core0") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_Core0_offset);
		*psc_unit=0;
		*psc_com=0x100;
	}
	else if (strcmp(string ,"Core1") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_Core1_offset);
		*psc_unit=0;
		*psc_com=0x200;
	}
	else if (strcmp(string ,"Core2") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_Core2_offset);
		*psc_unit=0;
		*psc_com=0x400;
	}
	else if (strcmp(string ,"Core3") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_Core3_offset);
		*psc_unit=0;
		*psc_com=0x800;
	}
	else if (strcmp(string ,"Core4") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_Core4_offset);
		*psc_unit=0;
		*psc_com=0x1000;
	}
	else if (strcmp(string ,"Core5") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_Core5_offset);
		*psc_unit=0;
		*psc_com=0x2000;
	}
	else if (strcmp(string ,"Core6") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_Core6_offset);
		*psc_unit=0;
		*psc_com=0x4000;
	}
	else if (strcmp(string ,"Core7") == 0)
	{
		psc_unit=(int*)(PSC_BASE+Power_Core7_offset);
		*psc_unit=0;
		*psc_com=0x8000;
	}
	do
	{
		temp=*psc_check;
	}
	while((temp)!=0);
}
