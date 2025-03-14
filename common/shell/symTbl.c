/*----------------------------------------------------*/
/*! \file symTbl.c
    \brief 

COPYRIGHT(c)  AVIC, Dep6  2012  All Rights Reserved.


*/
#include "cmd_table.h"
#include "symTbl.h"

#ifdef SPM_DEBUG

/* Symbol for the shell */
SYM_TBL symTbl[] = 
{
	{"d",                		 d,                    	3},
    {"m",                		 m,                    	3},
    {"clear",            		 clear,                	0},
    {"help",              		 h,                   	0},
    {"h",                		 h,                     0},
#if 1
    {"bspVersionInfoShow",       bspVersionInfoShow,    0},
    {"loadx",       			 loadx,    				1},
    {"autoboot",       			 autoboot,    			1},
    {"dspFlashWrite",       	 dspFlashWrite,    		3},
    {"dspFlashRead",       	 	 dspFlashRead,    		3},
    {"dspFlashAddrSwitch", 		 dspFlashAddrSwitch,   	1},
#endif
};

int symNum = sizeof(symTbl)/sizeof(symTbl[0]);
#endif

