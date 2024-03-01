#include "stdio.h"
#include "interface.h"
#include "ti/csl/tistdtypes.h"
#include "spi/spi.h"

SOFTWARE_VERSION_INFOR Sw_version;
version_drv DspVersion;

/********************************************************/
 //dsp & fpga communication with EMIF interface
/********************************************************/
static void ddr_pll_delay(unsigned int ix)
{
    while (ix--) {
        asm("   NOP");
    }
}
/*****************************************************************/
/*函数：int getDrvVersion(version_drv * version)
 *功能：获取软件版本
 *输入：version：存放软件版本的地址
 *返回值：0=成功，-1=失败
 */
int getDrvVersion(version_drv * version)
{
//	float version_f;
	char version_s[4];

 	sscanf(DSP_BOOT_VERSION, "%2d.%2d.%2d", &(version->versionH),&(version->versionM),&(version->versionL));

	sscanf(__TIME__, "%d:%d:%d", &(version->hour), &(version->minute), &(version->second));

	sscanf(__DATE__, "%3s  %d %d", version_s, &(version->day), &(version->year));

	version_s[3] = '\0';

	if ((version_s[0] == 'J') && (version_s[1] == 'a') && (version_s[2] == 'n'))
	{
		version->month = 1;
	}
	else if ((version_s[0] == 'F') && (version_s[1] == 'e') && (version_s[2] == 'b'))
	{
		version->month = 2;
	}
	else if ((version_s[0] == 'M') && (version_s[1] == 'a') && (version_s[2] == 'r'))
	{
		version->month = 3;
	}
	else if ((version_s[0] == 'A') && (version_s[1] == 'p') && (version_s[2] == 'r'))
	{
		version->month = 4;
	}
	else if ((version_s[0] == 'M') && (version_s[1] == 'a') && (version_s[2] == 'y'))
	{
		version->month = 5;
	}
	else if ((version_s[0] == 'J') && (version_s[1] == 'u') && (version_s[2] == 'n'))
	{
		version->month = 6;
	}
	else if ((version_s[0] == 'J') && (version_s[1] == 'u') && (version_s[2] == 'l'))
	{
		version->month = 7;
	}
	else if ((version_s[0] == 'A') && (version_s[1] == 'u') && (version_s[2] == 'g'))
	{
		version->month = 8;
	}
	else if ((version_s[0] == 'S') && (version_s[1] == 'e') && (version_s[2] == 'p'))
	{
		version->month = 9;
	}
	else if ((version_s[0] == 'O') && (version_s[1] == 'c') && (version_s[2] == 't'))
	{
		version->month = 10;
	}
	else if ((version_s[0] == 'N') && (version_s[1] == 'o') && (version_s[2] == 'v'))
	{
		version->month = 11;
	}
	else if ((version_s[0] == 'D') && (version_s[1] == 'e') && (version_s[2] == 'c'))
	{
		version->month = 12;
	}
	else
	{
		return -1;
	}

	return 0;
}

/*****************************************************************/
/*函数：void setSoftwareInfo(void)
 *功能：获取软件版本
 *输入：无
 *返回值：无
 */
void setSoftwareInfo(void)
{
	unsigned char data1=0;

	/*获取软件版本和软件版本编译时�?*/
	getDrvVersion(&DspVersion);

	/*软件构型版本信息-程序代码编号*/
	Sw_version.SoftWare_CodeInfo =DspVersion.CodeInfo;


	data1 = CHAR2BCD((DspVersion.versionH)&0xff);
	Sw_version.SoftWare_VersionH =data1;
	data1 = CHAR2BCD((DspVersion.versionM)&0xff);
	Sw_version.SoftWare_VersionM =data1;
	data1 = CHAR2BCD(DspVersion.versionL&0xff);
	Sw_version.SoftWare_VersionL =data1;

	data1 = CHAR2BCD((DspVersion.year/100)&0xff);
	Sw_version.SoftWare_Date_yearH =data1;
	data1 = CHAR2BCD((DspVersion.year%100)&0xff);
	Sw_version.SoftWare_Date_yearL =data1;

	data1 = CHAR2BCD((DspVersion.month)&0xff);
	Sw_version.SoftWare_Date_month =data1;
	data1 = CHAR2BCD((DspVersion.day)&0xff);
	Sw_version.SoftWare_Date_day =data1;

	data1 = CHAR2BCD((DspVersion.hour)&0xff);
	Sw_version.SoftWare_Time_hour =data1;
	data1 = CHAR2BCD((DspVersion.minute)&0xff);
	Sw_version.SoftWare_Time_minute =data1;
	data1 = CHAR2BCD((DspVersion.second)&0xff);
	Sw_version.SoftWare_Time_second =data1;

	return;
}
/*****************************************************************/
/*函数：void softInfoToFpga(void)
 *功能：把软件版本上报给FPGA3
 *输入：无
 *返回值：无
 */
void softInfoToFpga(void)
{
	unsigned char datawrite[FRAME_MAX_LENGTH]={0},dataread[FRAME_MAX_LENGTH-6]={0};
	unsigned int regAddr,temp;
	unsigned int payloadlength = 12;

	regAddr = SPI_FRAME_REG_ADDR(SPI_BOOT_VERSION1_SET);
	datawrite[0] = SPI_FRAME_CMD_WRITE;
	datawrite[1] = regAddr>>24&0xff;
	datawrite[2] = regAddr>>16&0xff;
	datawrite[3] = regAddr>>8&0xff;
	datawrite[4] = regAddr&0xff;
	datawrite[5] = payloadlength-1;
	datawrite[6] = Sw_version.SoftWare_Date_yearH;
	datawrite[7] = Sw_version.SoftWare_VersionL;
	datawrite[8] = Sw_version.SoftWare_VersionM;
	datawrite[9] = Sw_version.SoftWare_VersionH;
	datawrite[10] = Sw_version.SoftWare_Time_hour;
	datawrite[11] = Sw_version.SoftWare_Date_day;
	datawrite[12] = Sw_version.SoftWare_Date_month;
	datawrite[13] = Sw_version.SoftWare_Date_yearL;
	datawrite[14] = SYNC_MOSI;
	datawrite[15] = 0;
	datawrite[16] = Sw_version.SoftWare_Time_second;
	datawrite[17] = Sw_version.SoftWare_Time_minute;
	spiTransfer(0,0,datawrite,payloadlength+6,dataread,0);
}
/*****************************************************************/
/*函数：int dspFlashAddrSwitch(unsigned int flashBlockNo)
 *功能： 切换DSP FLASH的地址
 *输入：flashBlockNo:要切换的flash地址,0-31
 *返回值：	-1=参数错误
 * 	   	-2=切换失败
 * 	   	其它=切换成功的地址
 */
int dspFlashAddrSwitch(unsigned int flashBlockNo)
{
	unsigned char datawrite[FRAME_MAX_LENGTH]={0},dataread[FRAME_MAX_LENGTH-6]={0};
	unsigned int regAddr,temp;
	unsigned int payloadlength;
	int loopcnt = 20;

	if(flashBlockNo>31){
		return -1;
	}

	regAddr = SPI_FRAME_REG_ADDR(SPI_DSP_FLASH_SET);
	payloadlength = 4;
	datawrite[0] = SPI_FRAME_CMD_WRITE;
	datawrite[1] = regAddr>>24&0xff;
	datawrite[2] = regAddr>>16&0xff;
	datawrite[3] = regAddr>>8&0xff;
	datawrite[4] = regAddr&0xff;
	datawrite[5] = payloadlength-1;
	datawrite[6] = SYNC_MOSI;
	datawrite[7] = 0;
	datawrite[8] = 0;
	datawrite[9] = flashBlockNo & 0x1f;
	spiTransfer(0,0,datawrite,10,dataread,0);

	ddr_pll_delay(2000000);

	regAddr = SPI_FRAME_REG_ADDR(SPI_DSP_FLASH_GET);
	payloadlength = 4;
	datawrite[0] = SPI_FRAME_CMD_READ;
	datawrite[1] = regAddr>>24&0xff;
	datawrite[2] = regAddr>>16&0xff;
	datawrite[3] = regAddr>>8&0xff;
	datawrite[4] = regAddr&0xff;
	datawrite[5] = payloadlength-1;
	while(loopcnt--){
		spiTransfer(0,0,datawrite,6,dataread,payloadlength);
		if(dataread[0] == SYNC_MISO && dataread[3] == flashBlockNo){
			return flashBlockNo;
		}
		ddr_pll_delay(2000000);
	}
	return -2;
}
/*****************************************************************/
/*函数：void getSlot(Uint8 *pslot_num,Uint8 *pdsp_num)
 *功能：获取dsp芯片标识号和模块槽位
 *输入：pslot_num：存放模块槽位号的地址
 *	  pdsp_num：存放芯片标识号的地址
 *返回值：
 */
void getSlot(Uint8 *pmark_num,Uint8 *pdsp_num)
{
	unsigned char datawrite[FRAME_MAX_LENGTH]={0},dataread[FRAME_MAX_LENGTH-6]={0};
	unsigned int regAddr,temp;
	unsigned int payloadlength;
	int loopcnt = 100;

	regAddr = SPI_FRAME_REG_ADDR(SPI_MARK_GET);
	payloadlength = 4;
	datawrite[0] = SPI_FRAME_CMD_READ;
	datawrite[1] = regAddr>>24&0xff;
	datawrite[2] = regAddr>>16&0xff;
	datawrite[3] = regAddr>>8&0xff;
	datawrite[4] = regAddr&0xff;
	datawrite[5] = payloadlength-1;
	spiTransfer(0,0,datawrite,6,dataread,payloadlength);
	while(loopcnt--){
		spiTransfer(0,0,datawrite,6,dataread,payloadlength);
		if(dataread[0] == SYNC_MISO){
			*pmark_num = dataread[3];
			*pdsp_num = dataread[2];
			return ;
		}
		ddr_pll_delay(2000000);
	}
	UART_Print("spi read timeout...\r\n");
}
/*****************************************************************/
/*函数：int getBootMode(void)
 *功能： 切换DSP FLASH的地址
 *输入：flashBlockNo:要切换的flash地址,0-31
 *返回值：	-1 失败
 * 	   	0x11 正常启动
 * 	   	0x22 备份启动
 */
int getBootMode(void)
{
	unsigned char datawrite[FRAME_MAX_LENGTH]={0},dataread[FRAME_MAX_LENGTH-6]={0};
	unsigned int regAddr,temp;
	unsigned int payloadlength;

	regAddr = SPI_FRAME_REG_ADDR(SPI_BOOTMODE_GET);
	payloadlength = 4;
	datawrite[0] = SPI_FRAME_CMD_READ;
	datawrite[1] = regAddr>>24&0xff;
	datawrite[2] = regAddr>>16&0xff;
	datawrite[3] = regAddr>>8&0xff;
	datawrite[4] = regAddr&0xff;
	datawrite[5] = payloadlength-1;
	spiTransfer(0,0,datawrite,6,dataread,payloadlength);
	if(dataread[0] == SYNC_MISO){
		return dataread[3];
	}
}
void bootActiveSet(void)
{
	unsigned char datawrite[FRAME_MAX_LENGTH]={0},dataread[FRAME_MAX_LENGTH-6]={0};
	unsigned int regAddr,temp;
	unsigned int payloadlength;

	regAddr = SPI_FRAME_REG_ADDR(SPI_BOOT_ACTIVE);
	payloadlength = 4;
	datawrite[0] = SPI_FRAME_CMD_WRITE;
	datawrite[1] = regAddr>>24&0xff;
	datawrite[2] = regAddr>>16&0xff;
	datawrite[3] = regAddr>>8&0xff;
	datawrite[4] = regAddr&0xff;
	datawrite[5] = payloadlength-1;
	datawrite[6]= 0xa5;
	datawrite[7]= 0xa5;
	datawrite[8]= 0xa5;
	datawrite[9]= 0xa5;
	spiTransfer(0,0,datawrite,10,dataread,0);
}
void testfunc(void)
{
	unsigned char datawrite[FRAME_MAX_LENGTH]={0},dataread[FRAME_MAX_LENGTH-6]={0};
	unsigned int regAddr,temp;
	unsigned int payloadlength;
	int ret,i;
	char ch[30];
#if 0
	for(i=0;i<32;i++){
		ret = dspFlashAddrSwitch(i);
		if(ret != i){
			ret = 0;
		}
	}
	ret = getBootMode();
#endif
#if 0
	regAddr = SPI_FRAME_REG_ADDR(SPI_FPGA1_RELOAD);
	payloadlength = 4;
	datawrite[0] = SPI_FRAME_CMD_WRITE;
	datawrite[1] = regAddr>>24&0xff;
	datawrite[2] = regAddr>>16&0xff;
	datawrite[3] = regAddr>>8&0xff;
	datawrite[4] = regAddr&0xff;
	datawrite[5] = payloadlength-1;
	datawrite[9] = 	0;
	spiTransfer(0,0,datawrite,10,dataread,0);

	datawrite[9] = 	1;
	spiTransfer(0,0,datawrite,10,dataread,0);
#endif
#if 1
	unsigned short *emifaddr = (unsigned short *)0x7c00fff0;
	unsigned short *srcaddr = (unsigned short *)0xc000000;
	unsigned short *destaddr = (unsigned short *)0x7c00fff4;
	unsigned int length;

	*emifaddr = 1;
	for(i=0;i<(length/2);i++){
		*destaddr = *srcaddr;
		srcaddr++;
	}
	*emifaddr = 0;
#endif
}
