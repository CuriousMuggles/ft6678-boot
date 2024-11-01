#ifndef _BSPINTERFACE_H_
#define _BSPINTERFACE_H_


/***************************************************************************
通用宏定义
*****************************************************************************/
#define RET_SUCCESS         0   /*操作成功*/
#define RET_ERROR           -1  /*操作失败*/
#define RET_TIMEOUT         -2  /*操作超时错误*/
#define RET_BUSY            -3  /*当前设备正忙*/
#define RET_RARAM1_ERROR    -4  /*输入参数1错误*/
#define RET_RARAM2_ERROR    -5  /*输入参数2错误*/
#define RET_RARAM3_ERROR    -6  /*输入参数3错误*/
#define RET_RARAM4_ERROR    -7  /*输入参数4错误*/
#define RET_RARAM5_ERROR    -8  /*输入参数5错误*/
#define RET_RARAM6_ERROR    -9  /*输入参数6错误*/
#define RET_RARAM7_ERROR    -10 /*输入参数7错误*/

typedef int 			INT32;
typedef unsigned int 	UINT32;
typedef short 			INT16;
typedef unsigned short 	UINT16;
typedef char			INT8;
typedef unsigned char	UINT8;

#define CHAR2BCD(x)			((((x)/10)<<4) | ((x)%10))

#define DSP_BOOT_VERSION	"02.00.00"
/***************************************************************************
数据类型定义
*****************************************************************************/
typedef struct
{
    INT32 version1;         /*版本号1,0-9*/
    INT32 version2;         /*版本号2,0-99*/
    INT32 version3;         /*版本号3,0-99*/
    INT32 version4;         /*版本号4,0-999*/
    INT32 reserved[6];      /*预留（默认填0）*/
    INT32 year;             /*生成时间-年份*/
    INT32 month;            /*生成时间-月份（1-12）*/
    INT32 day;              /*生成时间-日期（1-31）*/
    INT32 hour;             /*生成时间-时钟（0-23）*/
    INT32 minute;           /*生成时间-分钟（0-59）*/
    INT32 second;           /*生成时间-秒钟（0-59）*/
} VERSION;/*版本信息结构体*/
/***************************************************************************
全局变量声明
*****************************************************************************/
extern void bspPrintf(const char *strFmt,int data0,int data1,int data2,int data3,int data4,int data5);
#endif/*_BSPINTERFACE_H_*/
