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
static int spiHeadCheck(unsigned int type,SPI_ICD *readbuf)
{
	char ch[20];

	if(!readbuf){
		return ;
	}

	if((readbuf->sync == SYNC_MISO) && (readbuf->msgtype == type)){
		return 0;
	}
	else{
		UART_Print("spi transfer fail,");
		sprintf(ch,"msg type is %d\r\n",type);
		UART_Print(ch);
		return 1;
	}
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


	data1 = char2BCD((DspVersion.versionH)&0xff);
	Sw_version.SoftWare_VersionH =data1;
	data1 = char2BCD((DspVersion.versionM)&0xff);
	Sw_version.SoftWare_VersionM =data1;
	data1 = char2BCD(DspVersion.versionL&0xff);
	Sw_version.SoftWare_VersionL =data1;

	data1 = char2BCD((DspVersion.year/100)&0xff);
	Sw_version.SoftWare_Date_yearH =data1;
	data1 = char2BCD((DspVersion.year%100)&0xff);
	Sw_version.SoftWare_Date_yearL =data1;

	data1 = char2BCD((DspVersion.month)&0xff);
	Sw_version.SoftWare_Date_month =data1;
	data1 = char2BCD((DspVersion.day)&0xff);
	Sw_version.SoftWare_Date_day =data1;

	data1 = char2BCD((DspVersion.hour)&0xff);
	Sw_version.SoftWare_Time_hour =data1;
	data1 = char2BCD((DspVersion.minute)&0xff);
	Sw_version.SoftWare_Time_minute =data1;
	data1 = char2BCD((DspVersion.second)&0xff);
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
	SPI_ICD framewrite={0},frameread={0};
	int readlength = 0;

	framewrite.sync = SYNC_MOSI;
	framewrite.msglength = 10;
	framewrite.msgtype = BOOT_VERSION;
	framewrite.msgdata[0] = DspVersion.versionH;
	framewrite.msgdata[1] = DspVersion.versionM;
	framewrite.msgdata[2] = DspVersion.versionL;
	framewrite.msgdata[3] = DspVersion.year/100;
	framewrite.msgdata[4] = DspVersion.year%100;
	framewrite.msgdata[5] = DspVersion.month;
	framewrite.msgdata[6] = DspVersion.day;
	framewrite.msgdata[7] = DspVersion.hour;
	framewrite.msgdata[8] = DspVersion.minute;
	framewrite.msgdata[9] = DspVersion.second;

	spiTransfer(0,0,(Uint8*)&framewrite,framewrite.msglength + FRAME_HEAD_LENGTH,(Uint8*)&frameread,readlength + FRAME_HEAD_LENGTH);
	spiHeadCheck(framewrite.msgtype,&frameread);
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
	SPI_ICD framewrite={0},frameread={0};
	int readlength = 0;

	if(flashBlockNo>31){
		return -1;
	}

	framewrite.sync = SYNC_MOSI;
	framewrite.msglength = 1;
	framewrite.msgtype = DSP_FLASH_SET;
	framewrite.msgdata[0] = flashBlockNo;

	spiTransfer(0,0,(Uint8*)&framewrite,framewrite.msglength + FRAME_HEAD_LENGTH,(Uint8*)&frameread,readlength + FRAME_HEAD_LENGTH);
	if(spiHeadCheck(framewrite.msgtype,&frameread) != 0){
		return -2;
	}

	ddr_pll_delay(1000);

	readlength = 1;
	framewrite.sync = SYNC_MOSI;
	framewrite.msglength = 0;
	framewrite.msgtype = DSP_FLASH_GET;
	spiTransfer(0,0,(Uint8*)&framewrite,framewrite.msglength + FRAME_HEAD_LENGTH,(Uint8*)&frameread,readlength + FRAME_HEAD_LENGTH);
	if(spiHeadCheck(framewrite.msgtype,&frameread) != 0){
		return -2;
	}
	return frameread.msgdata[0];
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
	SPI_ICD framewrite={0},frameread={0};
	int readlength = 2;

	framewrite.sync = SYNC_MOSI;
	framewrite.msglength = 0;
	framewrite.msgtype = MARK_SLOT_GET;

	spiTransfer(0,0,(Uint8*)&framewrite,framewrite.msglength + FRAME_HEAD_LENGTH,(Uint8*)&frameread,readlength + FRAME_HEAD_LENGTH);
	spiHeadCheck(framewrite.msgtype,&frameread);

	*pmark_num = (frameread.msgdata[0]>16 ? 16:frameread.msgdata[0]);
	*pdsp_num = frameread.msgdata[1];
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
	SPI_ICD framewrite={0},frameread={0};
	int readlength = 1;

	framewrite.sync = SYNC_MOSI;
	framewrite.msglength = 0;
	framewrite.msgtype = BOOT_MODE;

	spiTransfer(0,0,(Uint8*)&framewrite,framewrite.msglength + FRAME_HEAD_LENGTH,(Uint8*)&frameread,readlength + FRAME_HEAD_LENGTH);
	if(spiHeadCheck(framewrite.msgtype,&frameread) != 0){
		return -1;
	}

	return frameread.msgdata[0];
}
