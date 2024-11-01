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
/*������UINT32 swap32(UINT32 val)
 *���ܣ�32λ��С��ת��
 *���룺val����ת������
 *�������
 *����ֵ��ת�������
 */
UINT32 swap32(UINT32 val)
{
	return ((val>>24)&0xff) |
			((val>>8)&0xff00) |
			((val<<8)&0xff0000) |
			((val<<24)&0xff000000);
}
/*****************************************************************/
/*������INT32 bspSpiRegRead(UINT32 regaddr,UINT32 *val)
 *���ܣ�spi��FMQL�Ĵ���
 *���룺regaddr���Ĵ�����ַ
 *�����val����ȡ���ݴ�ŵ�ַ
 *����ֵ��RET_SUCCESS-�ɹ�
 *		  RET_ERROR-ʧ��
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
/*������INT32 bspSpiRegWrite(UINT32 regaddr,UINT32 val)
 *���ܣ�spiдFMQL�Ĵ���
 *���룺regaddr���Ĵ�����ַ
 *�����val����д�������
 *����ֵ��RET_SUCCESS-�ɹ�
 *		  RET_ERROR-ʧ��
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
/*������void softInfoToFpga(void)
 *���ܣ�������汾�ϱ���FMQL
 *���룺��
 *�������
 *����ֵ����
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
/*������INT32 dspFlashAddrSwitch(UINT32 flashBlockNo)
 *���ܣ� �л�DSP FLASH�ĵ�ַ
 *���룺flashBlockNo:Ҫ�л���flash��ַ,0-31
 *����ֵ��RET_RARAM1_ERROR=��������
 * 	   	RET_ERROR=�л�ʧ��
 * 	   	RET_SUCCESS=�л��ɹ�
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
/*������INT32 getSlot(UINT8 *pmark_num,UINT8 *pdsp_num)
 *���ܣ���ȡdspоƬ��ʶ�ź�ģ���λ
 *���룺pslot_num�����ģ���λ�ŵĵ�ַ
 *	  pdsp_num�����оƬ��ʶ�ŵĵ�ַ
 *����ֵ��RET_RARAM1_ERROR=��������
 * 	   	RET_ERROR=����ʧ��
 * 	   	RET_SUCCESS=�����ɹ�
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
/*������INT32 getBootMode(UINT32 *resettype)
 *���ܣ� ��ȡ��λԭ��
 *���룺flashBlockNo:Ҫ�л���flash��ַ,0-31
 *�����resettype:1-�ϵ縴λ
		2-���ȫ�ָ�λ
		3-������ڵ㸴λ
		4-���Ź����ڵ㸴λ
 *����ֵ��RET_RARAM1_ERROR=��������
 * 	   	RET_ERROR=����ʧ��
 * 	   	RET_SUCCESS=�����ɹ�
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
/*������INT32 getSlot(UINT8 *pmark_num,UINT8 *pdsp_num)
 *���ܣ���ȡdspоƬ��ʶ�ź�ģ���λ
 *���룺pslot_num�����ģ���λ�ŵĵ�ַ
 *	  pdsp_num�����оƬ��ʶ�ŵĵ�ַ
 *����ֵ��RET_ERROR=�Լ�ʧ��
 * 	   	 RET_SUCCESS=�Լ�ɹ�
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
