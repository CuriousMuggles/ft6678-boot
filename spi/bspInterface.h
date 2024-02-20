#ifndef SYLIXOS_DRIVER_BSPINTERFACE_H_
#define SYLIXOS_DRIVER_BSPINTERFACE_H_
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

/***************************************************************************
全局变量声明
*****************************************************************************/

#endif/*SYLIXOS_DRIVER_BSPINTERFACE_H_*/
