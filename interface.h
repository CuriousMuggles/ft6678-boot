#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#define DSP_BOOT_VERSION	"02.01.51"

typedef struct
{
	int CodeInfo;/*软件程序代码编号*/
	int versionH;/*软件版本高8位*/
	int versionM;/*软件版本中8位*/
	int versionL;/*软件版本低8位*/
	int year;/*年*/
	int month;/*月*/
	int day;/*日*/
	int hour;/*时*/
	int minute;/*分*/
	int second;/*秒*/
} version_drv;

typedef struct
{
	unsigned char SoftWare_CodeInfo;  /*BYTE0: 软件构件版本信息：程序代码编号*/
	unsigned char SoftWare_VersionH; /*BYTE1: 软件构件版本-版本号-版本号高位-10_BCD码+BCD码*/
	unsigned char SoftWare_VersionM; /*BYTE2:软件构件版本-版本号-版本号中位-10_BCD码+BCD码*/
	unsigned char SoftWare_VersionL; /*BYTE3:软件构件版本-版本号-版本号低位-10_BCD码+BCD码*/
	unsigned char SoftWare_Date_yearH; /*BYTE4:软件构件版本-编译时间-年-1000_BCD码+100_BCD码*/
	unsigned char SoftWare_Date_yearL; /*BYTE5:软件构件版本-编译时间-年-10_BCD码+BCD码*/
	unsigned char SoftWare_Date_month; /*BYTE6:软件构件版本-编译时间-月-10_BCD码+BCD码*/
	unsigned char SoftWare_Date_day; /*BYTE7:软件构件版本-编译时间-日-10_BCD码+BCD码*/
	unsigned char SoftWare_Time_hour; /*BYTE8:软件构件版本-编译时间-时-10_BCD码+BCD码*/
	unsigned char SoftWare_Time_minute; /*BYTE9:软件构件版本-编译时间-分-10_BCD码+BCD码*/
	unsigned char SoftWare_Time_second; /*BYTE10:软件构件版本-编译时间-秒-10_BCD码+BCD码*/
}SOFTWARE_VERSION_INFOR;

enum{
	DSP_HANDSHAKE_OK 		= 0x55,
	DSP_HANDSHAKE_WAIT		= 0xa5
};/*DSP握手状态*/

enum{
	BOOT_START_ERROR	= 0x11,
	APP_START_ERROR		= 0x22,
	START_SUCCESS		= 0x33
};/*DSP复位原因*/
/******************************emif_interface(FPGA3) ----SHARE ROM for communication**************************************/
/****************************0x7C000000~0x7C007FFF**************************************/
#define DSP_FPGA_bram_BASE_ADDR				(0x7c000000)
#define EMIF_BRAM_ADDR_OFFSET             	(0x0000)
#define DSP_FPGA_BRAM_ADDR                	(DSP_FPGA_bram_BASE_ADDR+EMIF_BRAM_ADDR_OFFSET)
#define EMIF_BRAM_ONEDATA_SIZE            	(0x1)
#define EMIF_BRAM_ONEDATA_OFFSET          	(0x0)
#define EMIF_BRAM_BIT_OFFSET             	(16)  
#define EMIF_BRAM_ADDR_SIZE              	(0x200*EMIF_BRAM_ONEDATA_SIZE)
#define FPGA3_EMIF_ADDR(x)					((x)*EMIF_BRAM_ONEDATA_SIZE+EMIF_BRAM_ONEDATA_OFFSET)

#define OFFSET_DSP_FLASH_SWITCH_RES         FPGA3_EMIF_ADDR(0x13)/*1=DSP FLASH切换完成*/
#define OFFSET_DSP_FLASH_ADDR  				FPGA3_EMIF_ADDR(0x22)/*当前DSP FLASH地址*/
#define OFFSET_DSP_RESET_REASON  			FPGA3_EMIF_ADDR(0x24)/*DSP复位原因*/
#define OFFSET_DSP_FLASH_SWITCH_CMD         FPGA3_EMIF_ADDR(0x30)/*1=开始切换DSP FLASH地址*/
#define OFFSET_DSP_FLASH_SWITCH_ADDR        FPGA3_EMIF_ADDR(0x31)/*DSP FLASH要切换的地址，0~31*/

/*上报BOOT版本号给FPGA3*/
#define OFFSET_FPGA_BOOT_VERSION_XX     	FPGA3_EMIF_ADDR(0x90)
#define OFFSET_FPGA_BOOT_VERSION_YY     	FPGA3_EMIF_ADDR(0x91)
#define OFFSET_FPGA_BOOT_VERSION_ZZ     	FPGA3_EMIF_ADDR(0x92)
#define OFFSET_FPGA_BOOT_YEAR_H       		FPGA3_EMIF_ADDR(0x93)
#define OFFSET_FPGA_BOOT_YEAR_L       		FPGA3_EMIF_ADDR(0x94)
#define OFFSET_FPGA_BOOT_MONTH       		FPGA3_EMIF_ADDR(0x95)
#define OFFSET_FPGA_BOOT_DAY       			FPGA3_EMIF_ADDR(0x96)
#define OFFSET_FPGA_BOOT_HOUR       	   	FPGA3_EMIF_ADDR(0x97)
#define OFFSET_FPGA_BOOT_MINUTE       	   	FPGA3_EMIF_ADDR(0x98)
#define OFFSET_FPGA_BOOT_SECOND       	  	FPGA3_EMIF_ADDR(0x99)
#define OFFSET_BOOT_VERSION_STATUS       	FPGA3_EMIF_ADDR(0x9C)

void setSoftwareInfo(void);
void softInfoToFpga(void);
int dspFlashAddrSwitch(unsigned int flashBlockNo);
#endif/*_INTERFACE_H_*/
