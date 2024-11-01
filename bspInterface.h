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

typedef int 			INT32;
typedef unsigned int 	UINT32;
typedef short 			INT16;
typedef unsigned short 	UINT16;
typedef char			INT8;
typedef unsigned char	UINT8;

#define CHAR2BCD(x)			((((x)/10)<<4) | ((x)%10))

#define DSP_BOOT_VERSION	"02.00.00"
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
extern void bspPrintf(const char *strFmt,int data0,int data1,int data2,int data3,int data4,int data5);
#endif/*_BSPINTERFACE_H_*/
