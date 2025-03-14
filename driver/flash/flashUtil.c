#include <stdio.h>
#include <string.h>
#include "bspInterface.h"
#include "flash.h"
#include "flash_nor.h"

NOR_InfoObj gNorInfo =
{
	CSL_EMIF16_data_REGS,/*flashBase;                     */
	FLASH_BUS_WIDTH_2_BYTES,/*busWidth;                   */
	FLASH_BUS_WIDTH_2_BYTES,/*chipOperatingWidth;         */
	FLASH_BUS_WIDTH_2_BYTES/*maxTotalWidth;              */
};

/*****************************************************************/
/*函数：INT32 dspFlashWrite(UINT32 offset,UINT8 *pBuf,UINT32 bufLen);
 *功能：DSP flash烧写函数（将存储于pBuf,长度为bufLen的DSP文件写入FLASH中偏移为offset的存储区）
 *输入：
 *	   offset:flash偏移地址
 *     pBuf ：待写入二进制文件首地址
 *     bufLen：待写数据长度
 *
 *
 *返回值：0-----烧写成功
 *	   -1-----检验失败
 *     -2----擦FLASH失败
 *     -3----写FLASH失败
 *     RET_RARAM1_ERROR----参数错误
 *
 */
INT32 dspFlashWrite(UINT32 offset,UINT8 *pBuf,UINT32 bufLen)
{
	INT32 tempRes=0;
	UINT32 flashAddr =0;
	INT32 flashBlkRet=0,blockNo=0;

	bufLen += (bufLen & 1);
	PARAMETER_ASSERT(((offset&1) == 0),return RET_RARAM1_ERROR);
	PARAMETER_ASSERT(((offset + bufLen) <= 0x10000000),return RET_RARAM1_ERROR);
	PARAMETER_ASSERT((pBuf != NULL),return RET_RARAM1_ERROR);

	blockNo = offset >> 23 & 0x1f;
	flashAddr = (offset & 0x7FFFFF) + CSL_EMIF16_data_REGS;
	// DPRINTF("offset = 0x%08x,blockNo = 0x%x,bufLen = 0x%x,flashAddr = 0x%08x\n",
	// 			offset,blockNo,bufLen,flashAddr);

	/*切换DSP对应的FLASH地址,用于代码的烧写执行,后续切换将DSP对应的FLASH块号切换回来切换回来*/
	flashBlkRet = dspFlashAddrSwitch(blockNo);
	if(flashBlkRet !=RET_SUCCESS)
	{
		printfk("DSP_FLASH switch err,please check return =%d\n",flashBlkRet);
		return -1;
	}

	/*擦除字节长度bufLen 对应的FLASH块大小空间*/
	tempRes = NOR_erase( &gNorInfo, flashAddr, bufLen );
	if(tempRes !=E_PASS)
	{
		printfk("dspFlashWrite erase failed\r\n");
		return -2;  //flash擦除失败
	}
	/*烧写文件*/
	tempRes = NOR_writeBytes(&gNorInfo,flashAddr, bufLen, (UINT32)pBuf);
	if(tempRes !=E_PASS)
	{
		printfk("dspFlashWrite program failed\r\n");
		return -3;  //flash写入数据失败
	}

	dspFlashAddrSwitch(0);
}

/*****************************************************************/
/*函数：INT32 dspFlashRead(UINT32 offset,UINT8 *pBuf,UINT32 bufLen);
 *功能：DSP flash读取函数（将存储于FLASH中偏移为offset,长度为bufLen的数据存储于 pBuf中）
 *输入：
*	   offset:flash偏移地址
 *     pBuf ：待写入二进制文件首地址
 *     bufLen：待写数据长度
 *
 *
 *返回值：0-----读取成功
 *	   -1-----检验失败
 *	   RET_RARAM1_ERROR----参数错误
 *
 */
INT32 dspFlashRead(UINT32 offset,UINT8 *pBuf,UINT32 bufLen)
{
	UINT32 flashAddr=0;
	INT32 flashBlkRet=0,blockNo=0;
	INT32 i,loopcnt;

	PARAMETER_ASSERT(((offset&1) == 0),return RET_RARAM1_ERROR);
	PARAMETER_ASSERT(((offset + bufLen) <= 0x10000000),return RET_RARAM1_ERROR);
	PARAMETER_ASSERT((pBuf != NULL),return RET_RARAM1_ERROR);

	blockNo = offset >> 23 & 0x1f;
	flashAddr = (offset & 0x7FFFFF) + CSL_EMIF16_data_REGS;

//	DPRINTF("offset = 0x%08x,blockNo = 0x%x\n",offset,blockNo);
	/*切换DSP对应的FLASH块号,用于代码的烧写执行,后续切换将DSP对应的FLASH块号切换回来切换回来*/
	flashBlkRet =dspFlashAddrSwitch(blockNo);
	if(flashBlkRet !=RET_SUCCESS)
	{
		dspFlashAddrSwitch(0);
		printfk("DSP_FLASH switch err,please check return =%d\n",flashBlkRet);
		return -1;
	}

	loopcnt = bufLen >> 1;
	for(i=0;i<loopcnt;i++){
		*(UINT16*)(pBuf + i*2) = *(UINT16*)(flashAddr + i*2);
	}
	if(bufLen & 1){
		pBuf[bufLen-1] = (UINT8)(*(UINT16*)(flashAddr + loopcnt*2));
	}

	dspFlashAddrSwitch(0);
	return 0;
}
