
/*******************************************************************************
*文件名：UART.h
*作    用： 用于声明串口相关的接口函数和寄存器宏定义。
*版    本：Version 1.0  创建于2018.05.08
*******************************************************************************/
#ifndef UART_H_
#define UART_H_

#define	DataReg          *(unsigned char*)0x02540000    //数据寄存器里的值
#define	ControlReg 	     *(unsigned int* )0x02540008    //控制寄存器里的值
#define	StatusReg 	     *(unsigned int* )0x02540004    //状态寄存器里的值
#define	ScalerReg	     *(unsigned int* )0x0254000c    //分频计数寄存器里的值
#define	SartAddress	      (unsigned char*)0x0c00b000    //接收数据的起始地址

#define MAIN_PLL         1000000000                     //芯片工作主频
#define BaudRate_Value   115200                         //UART的波特率

#define PSC_PTCMD        0x02350120                     //电源区域转换命令寄存器
#define PSC_MDCTL26      *(unsigned int*)0x02350a68     //模块控制寄存器
#define PSC_MDSTAT26     *(unsigned int*)0x02350868     //模块状态寄存器

#define UART_MAX_BUFLEN	(1024)

extern void  UART_Print(const char *strFmt);
extern void  UART_Config(unsigned int BaudRate);
void bspPrintf(const char *strFmt,int data0,int data1,int data2,int data3,int data4,int data5);

#endif /* UART_H_ */
