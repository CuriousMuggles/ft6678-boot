#ifndef _BSPINTERFACE_H_
#define _BSPINTERFACE_H_



/***************************************************************************
ͨ�ú궨��
*****************************************************************************/
#define RET_SUCCESS         0   /*�����ɹ�*/
#define RET_ERROR           -1  /*����ʧ��*/
#define RET_TIMEOUT         -2  /*������ʱ����*/
#define RET_BUSY            -3  /*��ǰ�豸��æ*/
#define RET_RARAM1_ERROR    -4  /*�������1����*/
#define RET_RARAM2_ERROR    -5  /*�������2����*/
#define RET_RARAM3_ERROR    -6  /*�������3����*/
#define RET_RARAM4_ERROR    -7  /*�������4����*/
#define RET_RARAM5_ERROR    -8  /*�������5����*/
#define RET_RARAM6_ERROR    -9  /*�������6����*/
#define RET_RARAM7_ERROR    -10 /*�������7����*/

typedef unsigned long long int 	UINT64;
typedef long long int 			INT64;
typedef int 					INT32;
typedef unsigned int 			UINT32;
typedef short 					INT16;
typedef unsigned short 			UINT16;
typedef char					INT8;
typedef unsigned char			UINT8;

#define CHAR2BCD(x)			((((x)/10)<<4) | ((x)%10))
/***************************************************************************
 ȫ�ּĴ����궨��
*****************************************************************************/
#define	KICK0 	        *(unsigned int *)0x02620038
#define	KICK1	        *(unsigned int *)0x0262003C
#define KICK0_UNLOCK    (0x83E70B13)
#define KICK1_UNLOCK    (0x95A4F1E0)
#define KICK_LOCK       0

#define BOOT_MAGIC_ADDR(x)  *(unsigned int*)(0x87fffc + (1<<28) + ((x)<<24))
#define IPCGR(x)		    *(unsigned int *)(0x2620240 + (x)*4)

/***************************************************************************
�û��궨��
*****************************************************************************/
#define DSP_BOOT_VERSION	"02.00.00"

#define FLASH_STARTUP_ADDRS_BOOT    (0x70000000)    /* BOOT��FLASH�еĵ�ַ */
#define FLASH_STARTUP_ADDRS_APP     (0x70000000)    /* APP��FLASH�еĵ�ַ */
#define DDR_TEMP_ADDRS_CODE         (0xA0000000)    /* APP��ʱ������ַ */
#define APP_FLASH_LEN               (0x300000)      /* APP��ʱ�������� */
/***************************************************************************
�������Ͷ���
*****************************************************************************/
typedef struct
{
    INT32 version1;         /*�汾��1,0-9*/
    INT32 version2;         /*�汾��2,0-99*/
    INT32 version3;         /*�汾��3,0-99*/
    INT32 version4;         /*�汾��4,0-999*/
    INT32 reserved[6];      /*Ԥ����Ĭ����0��*/
    INT32 year;             /*����ʱ��-���*/
    INT32 month;            /*����ʱ��-�·ݣ�1-12��*/
    INT32 day;              /*����ʱ��-���ڣ�1-31��*/
    INT32 hour;             /*����ʱ��-ʱ�ӣ�0-23��*/
    INT32 minute;           /*����ʱ��-���ӣ�0-59��*/
    INT32 second;           /*����ʱ��-���ӣ�0-59��*/
} VERSION;/*�汾��Ϣ�ṹ��*/
/***************************************************************************
ȫ�ֱ�������
*****************************************************************************/
extern void usleep(unsigned int n_us);
extern void msleep(unsigned int n_ms);
extern void sleep(unsigned int n_s);
extern void pll_wait(unsigned int i);
extern UINT32 reload_dat_boot(UINT32 srcAddr,UINT32 coreNum);
extern UINT32 reload_dat_app(UINT32 srcAddr,UINT32 coreNum);
extern _c_int00(void);

extern void bspPrintf(const char *strFmt,int data0,int data1,int data2,int data3,int data4,int data5);
#endif/*_BSPINTERFACE_H_*/
