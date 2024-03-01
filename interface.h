#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#define DSP_BOOT_VERSION	"02.00.00"

#define CHAR2BCD(x)			((((x)/10)<<4) | ((x)%10))

#define SYNC_MOSI			(0xa5)
#define SYNC_MISO			(0x5a)
#define NORMAL_OR_OK		(0x11)
#define BACKUP_OR_FAIL		(0x22)
#define FRAME_MAX_LENGTH	(18)

#define SPI_FRAME_CMD_READ			(0)
#define SPI_FRAME_CMD_WRITE			(1)
#define SPI_FRAME_REG_ADDR(reg)		(4 * reg)
typedef enum{
	/*DSP写，FPGA3读*/
	SPI_DSP_BIT,
	SPI_FPGA1_BIT,
	SPI_FPGA2_BIT,
	SPI_DSP_FLASH_SET,
	SPI_FPGA1_FALSH_SET,
	SPI_FPGA2_FLASH_SET,
	SPI_BOOT_VERSION1_SET,
	SPI_BOOT_VERSION2_SET,
	SPI_BOOT_VERSION3_SET,
	SPI_DSP_VERSION1_SET,
	SPI_DSP_VERSION2_SET,
	SPI_DSP_VERSION3_SET,
	SPI_FPGA1_VERSION1_SET,
	SPI_FPGA1_VERSION2_SET,
	SPI_FPGA1_VERSION3_SET,
	SPI_FPGA2_VERSION1_SET,
	SPI_FPGA2_VERSION2_SET,
	SPI_FPGA2_VERSION3_SET,
	/*DSP写，FPGA3读后清空*/
	SPI_BOOT_ACTIVE = 28,
	SPI_APP_ACTIVE =29,
	/*DSP写，逻辑判断*/
	SPI_FPGA1_RELOAD = 30,
	SPI_FPGA2_RELOAD = 31,
	/*DSP读，FPGA3写*/
	SPI_MARK_GET = 32,
	SPI_BOOTMODE_GET,
	SPI_DSP_FLASH_GET,
	SPI_FPGA1_FLASH_GET,
	SPI_FPGA2_FLASH_GET,
	SPI_BOOT_VERSION1_GET,
	SPI_BOOT_VERSION2_GET,
	SPI_BOOT_VERSION3_GET,
	SPI_FPGA3_VERSION1_GET,
	SPI_FPGA3_VERSION2_GET,
	SPI_FPGA3_VERSION3_GET,
	SPI_EXTERNAL_TIME1_GET,
	SPI_EXTERNAL_TIME2_GET,
	SPI_FPGA1_LOADSTATUS_GET,
	SPI_FPGA2_LOADSTATUS_GET
}SPI_REG;

typedef enum{
	DSP_STATUS,
	FPGA1_STATUS,
	FPGA2_STATUS,
	BOOT_VERSION,
	DSP_VERSION,
	FPGA1_VERSION,
	FPGA2_VERSION,
	DSP_FLASH_SET,
	DSP_FLASH_GET,
	FPGA1_FLASH_SET,
	FPGA1_FLASH_GET,
	FPGA2_FLASH_SET,
	FPGA2_FLASH_GET,
	MARK_SLOT_GET,
	EXTERNAL_TIME,
	BOOT_MODE,
	FPGA1_RELOAD,
	FPGA2_RELOAD,
	FPGA1_LOADSTATUS_GET,
	FPGA2_LOADSTATUS_GET,
	TYPE_MAX_NUM
}ICD_TYPE;

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

void setSoftwareInfo(void);
void softInfoToFpga(void);
int dspFlashAddrSwitch(unsigned int flashBlockNo);
int getBootMode(void);
void bootActiveSet(void);
#endif/*_INTERFACE_H_*/
