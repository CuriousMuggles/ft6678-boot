#include "stdio.h"
#include "interface.h"
#include "ti/csl/tistdtypes.h"

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
/*函数：unsigned char char2BCD(unsigned char val)
 *功能：二进制数转BCD格式
 *输入：val：要转换的数
 *返回值：转换结果
 */
unsigned char char2BCD(unsigned char val)
{
	char temp;
	temp =(val/10)<<4;
	temp = temp +(val%10);
	return temp;
	/*return ((val/10)<<4+(val%10));*/
}
/*****************************************************************/
/*函数：unsigned char fpga3_bram_Write8(unsigned int address,unsigned char data)
 *功能： 写入连接fpga3的emif数据
 *输入： address：要写入的地址
 * 	  data：要写入的数据
 *返回值：写入的值
 */
unsigned char fpga3_bram_Write8(unsigned int address,unsigned char data)
{
	*(volatile unsigned char*)(address) = data;
}
/*****************************************************************/
/*函数：Uint8 fpga3_bram_Read8(Uint32 address,Uint8 *data)
 *功能： 读取连接fpga3的emif数据
 *输入： address：要读取的地址
 * 	  data：存放读取数据的地址
 *返回值：读取的值
 */
Uint8 fpga3_bram_Read8(Uint32 address,Uint8 *data)
{
	*data =*(volatile Uint8*)(address);
	return (*data);
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

	sscanf(__TIME__, "%d:%d:%d", &(version->hour), &(version->minute),
			&(version->second));

	sscanf(__DATE__, "%3s  %d %d", version_s, &(version->day),
			&(version->year));

	version_s[3] = '\0';

	if ((version_s[0] == 'J') && (version_s[1] == 'a') && (version_s[2] == 'n'))
	{
		version->month = 1;
	}
	else if ((version_s[0] == 'F') && (version_s[1] == 'e')
			&& (version_s[2] == 'b'))
	{
		version->month = 2;
	}
	else if ((version_s[0] == 'M') && (version_s[1] == 'a')
			&& (version_s[2] == 'r'))
	{
		version->month = 3;
	}
	else if ((version_s[0] == 'A') && (version_s[1] == 'p')
			&& (version_s[2] == 'r'))
	{
		version->month = 4;
	}
	else if ((version_s[0] == 'M') && (version_s[1] == 'a')
			&& (version_s[2] == 'y'))
	{
		version->month = 5;
	}
	else if ((version_s[0] == 'J') && (version_s[1] == 'u')
			&& (version_s[2] == 'n'))
	{
		version->month = 6;
	}
	else if ((version_s[0] == 'J') && (version_s[1] == 'u')
			&& (version_s[2] == 'l'))
	{
		version->month = 7;
	}
	else if ((version_s[0] == 'A') && (version_s[1] == 'u')
			&& (version_s[2] == 'g'))
	{
		version->month = 8;
	}
	else if ((version_s[0] == 'S') && (version_s[1] == 'e')
			&& (version_s[2] == 'p'))
	{
		version->month = 9;
	}
	else if ((version_s[0] == 'O') && (version_s[1] == 'c')
			&& (version_s[2] == 't'))
	{
		version->month = 10;
	}
	else if ((version_s[0] == 'N') && (version_s[1] == 'o')
			&& (version_s[2] == 'v'))
	{
		version->month = 11;
	}
	else if ((version_s[0] == 'D') && (version_s[1] == 'e')
			&& (version_s[2] == 'c'))
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

	/*获取软件版本和软件版本编译时间*/
	getDrvVersion(&DspVersion);

	/*软件构型版本信息-程序代码编号*/
	Sw_version.SoftWare_CodeInfo =DspVersion.CodeInfo;

	/*软件版本号信息*/
	data1 = char2BCD((DspVersion.versionH)&0xff);
	Sw_version.SoftWare_VersionH =data1;
	data1 = char2BCD((DspVersion.versionM)&0xff);
	Sw_version.SoftWare_VersionM =data1;
	data1 = char2BCD(DspVersion.versionL&0xff);
	Sw_version.SoftWare_VersionL =data1;

	//编译时间-年-（年_1000 +年_100）  +(年_10 +年_0)
	data1 = char2BCD((DspVersion.year/100)&0xff);
	Sw_version.SoftWare_Date_yearH =data1;
	data1 = char2BCD((DspVersion.year%100)&0xff);
	Sw_version.SoftWare_Date_yearL =data1;

	//编译时间——月 +日
	data1 = char2BCD((DspVersion.month)&0xff);
	Sw_version.SoftWare_Date_month =data1;
	data1 = char2BCD((DspVersion.day)&0xff);
	Sw_version.SoftWare_Date_day =data1;

	//编译时间——时  --分 -- 秒
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
	unsigned int address = 0;

	address = DSP_FPGA_BRAM_ADDR +OFFSET_FPGA_BOOT_VERSION_XX;
	fpga3_bram_Write8(address,Sw_version.SoftWare_VersionH);
	address = DSP_FPGA_BRAM_ADDR +OFFSET_FPGA_BOOT_VERSION_YY;
	fpga3_bram_Write8(address,Sw_version.SoftWare_VersionM);
	address = DSP_FPGA_BRAM_ADDR +OFFSET_FPGA_BOOT_VERSION_ZZ;
	fpga3_bram_Write8(address,Sw_version.SoftWare_VersionL);
	address = DSP_FPGA_BRAM_ADDR +OFFSET_FPGA_BOOT_YEAR_H;
	fpga3_bram_Write8(address,Sw_version.SoftWare_Date_yearH);
	address = DSP_FPGA_BRAM_ADDR +OFFSET_FPGA_BOOT_YEAR_L;
	fpga3_bram_Write8(address,Sw_version.SoftWare_Date_yearL);
	address = DSP_FPGA_BRAM_ADDR +OFFSET_FPGA_BOOT_MONTH;
	fpga3_bram_Write8(address,Sw_version.SoftWare_Date_month);
	address = DSP_FPGA_BRAM_ADDR +OFFSET_FPGA_BOOT_DAY;
	fpga3_bram_Write8(address,Sw_version.SoftWare_Date_day);
	address = DSP_FPGA_BRAM_ADDR +OFFSET_FPGA_BOOT_HOUR;
	fpga3_bram_Write8(address,Sw_version.SoftWare_Time_hour);
	address = DSP_FPGA_BRAM_ADDR +OFFSET_FPGA_BOOT_MINUTE;
	fpga3_bram_Write8(address,Sw_version.SoftWare_Time_minute);
	address = DSP_FPGA_BRAM_ADDR +OFFSET_FPGA_BOOT_SECOND;
	fpga3_bram_Write8(address,Sw_version.SoftWare_Time_second);
//	address = DSP_FPGA_BRAM_ADDR +OFFSET_BOOT_VERSION_STATUS;
//	fpga3_bram_Write8(address,DSP_HANDSHAKE_OK);

}
/*****************************************************************/
/*函数：int dspFlashAddrSwitch(unsigned int flashBlockNo)
 *功能： 切换DSP FLASH的地址线
 *输入： flashBlockNo:要切换的flash地址线,0-31
 *返回值：	-1=参数错误
 * 	   	-2=切换失败
 * 	   	其它=切换成功的地址线
 */
int dspFlashAddrSwitch(unsigned int flashBlockNo)
{
	unsigned char  BlockNo =0;
	unsigned int address = 0;
	unsigned char  flashRet =0;
	unsigned int  coutNum =0;


	if(flashBlockNo>31)
	{
		return -1;
	}

	/*如果当前地址等于要切换地址，则直接返回*/
	address = DSP_FPGA_BRAM_ADDR + OFFSET_DSP_FLASH_ADDR;
	fpga3_bram_Read8(address,&BlockNo);
	if(BlockNo == flashBlockNo)
	{
		return BlockNo;
	}

	/*写入需要加载的FLASH块号*/
	address =DSP_FPGA_BRAM_ADDR + OFFSET_DSP_FLASH_SWITCH_ADDR;
	fpga3_bram_Write8(address,flashBlockNo);
	/*写入DSP对应的FLASH地址切换命令*/
	address =DSP_FPGA_BRAM_ADDR +OFFSET_DSP_FLASH_SWITCH_CMD;
	fpga3_bram_Write8(address,1);

	/*等待切换flash地址命令响应成功,while循环等待*/
	flashRet =0;
	address = DSP_FPGA_BRAM_ADDR + OFFSET_DSP_FLASH_SWITCH_RES;
	while((flashRet&0x01)!=1)
	{
		/*获取DSP FLASH地址切换结果*/
		fpga3_bram_Read8(address,&flashRet);
//		if(coutNum++>=10000)
//		{
//			/*将flash地址线切换回之前的地址位置,避免切换失败以后影响后续操作【不写flash地址块 + 失能切换命令】*/
//			address =DSP_FPGA_BRAM_ADDR + OFFSET_DSP_FLASH_SWITCH_CMD;
//			fpga3_bram_Write8(address,0);
//			return -2;
//		}
		ddr_pll_delay(700);
	}

	/*获取切换到的FLASH块号信息*/
	address = DSP_FPGA_BRAM_ADDR +OFFSET_DSP_FLASH_ADDR;
	fpga3_bram_Read8(address,&BlockNo);
	/*清零DSP FLASH地址切换结果*/
	address =DSP_FPGA_BRAM_ADDR +OFFSET_DSP_FLASH_SWITCH_RES;
	fpga3_bram_Write8(address,0);
	
	return BlockNo;
}

