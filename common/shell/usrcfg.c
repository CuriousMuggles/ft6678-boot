#include<stdio.h>
#include<stdlib.h>
#include "string.h"
#include "uart/uart.h"

char getck(void)
{
	char temp;
	bspUartRecv((unsigned char*)&temp,1,0);
	return temp;
}

char putbufk(const char *strFmt)
{
	bspUartPrintString(strFmt);
}

void usrBanner(void)
{
	printfk("***********************************************\r\n");
	printfk("**** *************** *******************\r\n");
	printfk("****** *********** *******************\r\n");
	printfk("******** ******* *******************\r\n");
	printfk("********** *** *******************\r\n");
	printfk("************ *******************\r\n");
	printfk("************ *****************\r\n");
	printfk("************ ***************\r\n");
	printfk("************ *************\r\n");
	printfk("************ ***********\r\n");
	printfk("************ *********\r\n");
	printfk("Target Shell version:%s\r\n","A5");
	printfk("File Created Time: %s,%s\n\r",__DATE__,__TIME__);
	printfk("**********************************************\r\n");
}
