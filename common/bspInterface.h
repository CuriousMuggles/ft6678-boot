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

typedef unsigned long long int 	UINT64;
typedef long long int 			INT64;
typedef int 					INT32;
typedef unsigned int 			UINT32;
typedef short 					INT16;
typedef unsigned short 			UINT16;
typedef char					INT8;
typedef unsigned char			UINT8;

#define CHAR2BCD(x)			((((x)/10)<<4) | ((x)%10))

#define PARAMETER_ASSERT(expr, action) \
        if (!(expr)) { \
            action; \
        }

/***************************************************************************
 全局寄存器宏定义
*****************************************************************************/
#define	KICK0 	        *(unsigned int *)0x02620038
#define	KICK1	        *(unsigned int *)0x0262003C
#define KICK0_UNLOCK    (0x83E70B13)
#define KICK1_UNLOCK    (0x95A4F1E0)
#define KICK_LOCK       0

#define BOOT_MAGIC_ADDR(x)  *(unsigned int*)(0x87fffc + (1<<28) + ((x)<<24))
#define IPCGR(x)		    *(unsigned int *)(0x2620240 + (x)*4)

/***************************************************************************
用户宏定义
*****************************************************************************/
#define DSP_BOOT_VERSION	"02.00.00"

#define FLASH_STARTUP_ADDRS_BOOT    (0x70000000)    /* BOOT在FLASH中的地址 */
#define FLASH_STARTUP_ADDRS_APP     (0x70000000)    /* APP在FLASH中的地址 */
#define DDR_TEMP_ADDRS_CODE         (0xA0000000)    /* APP临时拷贝地址 */
#define APP_FLASH_LEN               (0x300000)      /* APP临时拷贝长度 */
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
extern void usleep(unsigned int n_us);
extern void msleep(unsigned int n_ms);
extern void sleep(unsigned int n_s);
extern void pll_wait(unsigned int i);
extern UINT32 reload_dat_boot(UINT32 srcAddr,UINT32 coreNum);
extern UINT32 reload_dat_app(UINT32 srcAddr,UINT32 coreNum);
extern _c_int00(void);
extern void printfk(const char *fmt, ...);
#endif/*_BSPINTERFACE_H_*/
