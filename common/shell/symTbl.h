/*----------------------------------------------------*/
/*! \file symTbl.h
    \brief Symbol Table for the target shell.

COPYRIGHT(c)  AVIC, Dep6  2012  All Rights Reserved.

This file define the structure that describes the target shell cmd. The 
structure includes the string for command and corresponding function.

*/

#ifndef _SYMTBL_H_
#define _SYMTBL_H_

#include "bspInterface.h"

#define SPM_DEBUG

#ifdef SPM_DEBUG
#define MAX_CHR_LEN    64
typedef void (*VOIDFUNCPTR4) (int, int, int, int);
typedef struct
{
    char             *sym_str;
    VOIDFUNCPTR4     func_ptr;
    int                func_type;
}SYM_TBL;

/*全局变量*/
extern SYM_TBL symTbl[];
extern int symNum;

/*函数声明*/
void bspVersionInfoShow(void);
void autoboot(unsigned int blocknum);
INT32 dspFlashWrite(UINT32 offset,UINT8 *pBuf,UINT32 bufLen);
INT32 dspFlashRead(UINT32 offset,UINT8 *pBuf,UINT32 bufLen);
INT32 dspFlashAddrSwitch(UINT32 flashBlockNo);
#endif/* SPM_DEBUG */

#endif/*_SYMTBL_H_*/

