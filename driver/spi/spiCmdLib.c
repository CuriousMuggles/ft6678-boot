#include <stdio.h>
#include <string.h>
#include "bspInterface.h"
#include "spi.h"
#include "spiCmdLib.h"

/********************************************************/
 //dsp & fpga communication with EMIF interface
/********************************************************/
static void spi_delay(UINT32 ix)
{
    while (ix--) {
        asm("   NOP");
    }
}
/*****************************************************************/
/*函数：UINT32 swap32(UINT32 val)
 *功能：32位大小端转换
 *输入：val：待转换的数
 *输出：无
 *返回值：转换后的数
 */
UINT32 swap32(UINT32 val)
{
	return ((val>>24)&0xff) |
			((val>>8)&0xff00) |
			((val<<8)&0xff0000) |
			((val<<24)&0xff000000);
}
/*****************************************************************/
/*函数：INT32 bspSpiRegRead(UINT32 regaddr,UINT32 *val)
 *功能：spi读FMQL寄存器
 *输入：regaddr：寄存器地址
 *输出：val：读取数据存放地址
 *返回值：RET_SUCCESS-成功
 *		  RET_ERROR-失败
 */
INT32 bspSpiRegRead(UINT32 regaddr,UINT32 *val)
{
	UINT8 datawrite[FRAME_MAX_LENGTH]={0};
	UINT32 readdata = 0;
	INT32 ret;

	datawrite[0] = SPI_FRAME_CMD_READ;
	datawrite[1] = regaddr>>24&0xff;
	datawrite[2] = regaddr>>16&0xff;
	datawrite[3] = regaddr>>8&0xff;
	datawrite[4] = regaddr&0xff;
	datawrite[5] = 3;
	ret = spiTransfer(0,0,datawrite,6,(UINT8 *)&readdata,4);
	if(ret != RET_SUCCESS){
		return RET_ERROR;
	}
	else{
		*val = swap32(readdata);
		return RET_SUCCESS;
	}
}
/*****************************************************************/
/*函数：INT32 bspSpiRegWrite(UINT32 regaddr,UINT32 val)
 *功能：spi写FMQL寄存器
 *输入：regaddr：寄存器地址
 *输出：val：待写入的数据
 *返回值：RET_SUCCESS-成功
 *		  RET_ERROR-失败
 */
INT32 bspSpiRegWrite(UINT32 regaddr,UINT32 val)
{
	UINT8 datawrite[FRAME_MAX_LENGTH]={0};
	UINT32 readdata = 0;

	datawrite[0] = SPI_FRAME_CMD_WRITE;
	datawrite[1] = regaddr>>24&0xff;
	datawrite[2] = regaddr>>16&0xff;
	datawrite[3] = regaddr>>8&0xff;
	datawrite[4] = regaddr&0xff;
	datawrite[5] = 3;
	datawrite[6] = val>>24&0xff;
	datawrite[7] = val>>16&0xff;
	datawrite[8] = val>>8&0xff;
	datawrite[9] = val&0xff;
	return spiTransfer(0,0,datawrite,10,(UINT8 *)&readdata,0);
}
/*****************************************************************/
/*函数：void softInfoToFpga(void)
 *功能：把软件版本上报给FMQL
 *输入：无
 *输出：无
 *返回值：无
 */
extern INT32 bspBspVersionGet(VERSION * pVersion);
void softInfoToFpga(void)
{
	VERSION DspVersion;
	UINT32 temp;

	bspBspVersionGet(&DspVersion);
	temp = CHAR2BCD(DspVersion.version1&0xff)|
		   CHAR2BCD(DspVersion.version2&0xff)<<8|
		   CHAR2BCD(DspVersion.version3&0xff)<<16|
		   CHAR2BCD((DspVersion.year/100)&0xff)<<24;
	bspSpiRegWrite(SPI_BRAM_REG(BOOT_VER1),temp);
	temp = CHAR2BCD((DspVersion.year%100)&0xff)|
		   CHAR2BCD(DspVersion.month&0xff)<<8|
		   CHAR2BCD(DspVersion.day&0xff)<<16|
		   CHAR2BCD(DspVersion.hour&0xff)<<24;
	bspSpiRegWrite(SPI_BRAM_REG(BOOT_VER2),temp);
	temp = CHAR2BCD(DspVersion.minute&0xff)|
		   CHAR2BCD(DspVersion.second&0xff)<<8;
	bspSpiRegWrite(SPI_BRAM_REG(BOOT_VER3),temp);
}
/*****************************************************************/
/*函数：INT32 dspFlashAddrSwitch(UINT32 flashBlockNo)
 *功能： 切换DSP FLASH的地址
 *输入：flashBlockNo:要切换的flash地址,0-31
 *返回值：RET_RARAM1_ERROR=参数错误
 * 	   	RET_ERROR=切换失败
 * 	   	RET_SUCCESS=切换成功
 */
INT32 dspFlashAddrSwitch(UINT32 flashBlockNo)
{
	if(flashBlockNo>31){
		return RET_RARAM1_ERROR;
	}

	if(bspSpiRegWrite(SPI_BRAM_REG(DSP_FLASH_HADDR),flashBlockNo) == RET_SUCCESS){
		return RET_SUCCESS;
	}
	else{
		return RET_ERROR;
	}
}
/*****************************************************************/
/*函数：INT32 getSlot(UINT8 *pmark_num,UINT8 *pdsp_num)
 *功能：获取dsp芯片标识号和模块槽位
 *输入：pslot_num：存放模块槽位号的地址
 *	  pdsp_num：存放芯片标识号的地址
 *返回值：RET_RARAM1_ERROR=参数错误
 * 	   	RET_ERROR=操作失败
 * 	   	RET_SUCCESS=操作成功
 */
INT32 getSlot(UINT8 *pmark_num,UINT8 *pdsp_num)
{
	UINT32 temp;

	if(!pmark_num || !pdsp_num){
		return RET_RARAM1_ERROR;
	}
	if(bspSpiRegRead(SPI_BRAM_REG(MARK_ID),&temp) == RET_SUCCESS){
		*pmark_num = temp & 0xff;
		*pdsp_num = temp >> 8 & 0xff;
		return RET_SUCCESS;
	}
	else{
		return RET_ERROR;
	}
}
/*****************************************************************/
/*函数：INT32 getBootMode(UINT32 *resettype)
 *功能： 获取复位原因
 *输入：flashBlockNo:要切换的flash地址,0-31
 *输出：resettype:1-上电复位
		2-软件全局复位
		3-软件单节点复位
		4-看门狗单节点复位
 *返回值：RET_RARAM1_ERROR=参数错误
 * 	   	RET_ERROR=操作失败
 * 	   	RET_SUCCESS=操作成功
 */
INT32 getBootMode(UINT32 *resettype)
{
	UINT32 temp;

	if(!resettype){
		return RET_RARAM1_ERROR;
	}
	if(bspSpiRegRead(SPI_BRAM_REG(DSP_RST_TYPE),resettype) == RET_SUCCESS){
		return RET_SUCCESS;
	}
	else{
		return RET_ERROR;
	}
}
/*****************************************************************/
/*函数：INT32 getSlot(UINT8 *pmark_num,UINT8 *pdsp_num)
 *功能：获取dsp芯片标识号和模块槽位
 *输入：pslot_num：存放模块槽位号的地址
 *	  pdsp_num：存放芯片标识号的地址
 *返回值：RET_ERROR=自检失败
 * 	   	 RET_SUCCESS=自检成功
 */
INT32 bspSpiBit(void)
{
	UINT32 temp;

	if(bspSpiRegRead(SPI_BRAM_REG(TEST),&temp) == RET_SUCCESS){
		if(temp == 0x12345678){
			return RET_SUCCESS;
		}
	}
	return RET_ERROR;
}
