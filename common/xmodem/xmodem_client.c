/**********************    COPYRIGHT 2014-2015, EYBOND    ************************
  * @File    : xmodem_client.c 
  * @Author  : R.Y. 
  * @Date    : 2016-05-17
  * @Brief   : The public types,constants,macros and functions for xmodem recv
               This code needs standard functions memcpy() and memset()
               and input/output functions xmodem_inbyte() and xmodem_outbyte().
               使用说明:
               1、xmodem_lowLevelRecvByte是底层接收函数，与平台相关，需要按需修改。
               2、xmodem_lowlevelSendByte是底层发送函数，与平台相关，需要按需修改。
               3、该协议是基于XMODEM-1K协议，单次数据包缓存必须保证大于1030个字节。
               4、xmodem_1K_recv()的buf是入口函数作为演示用的缓存，其大小需要根据
                  实际环境更改。
**********************************************************************************/
#include <stdio.h>
#include "bspInterface.h"
#include "xmodem_client.h"

/* Private macro ------------------------------------------------------------*/
/* Private types ------------------------------------------------------------*/
/* Private constants --------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
// 定义Xmodem当前工作模式
static UINT8 bXmodemExit=XMODEM_NO_EXIT;

// 定义Xmodem缓存，1024 for XModem 1k + 3 head chars + 2 crc + nul
static UINT8 bXmodemBuf[1030];

/* Private functions ------------------------------------------------------- */



/*****************************************************************************
 Function : xmodem_inbyte
 Brief    : xmodem输入字节，移植无需修改
 Input    : [UINT16 wMs]: 未使用
 Output   : [UINT8* bData]: 接收数据地址
 Return   : [UINT8]       : 返回接收结果，TRUE是接收正确，FALSE是接收失败
 Author   : R.Y.
 Date     : 2016-05-17
*****************************************************************************/
UINT8 xmodem_inbyte(UINT16 wMs, UINT8* pbData)
{
	INT32 ret;

	if(wMs){
		ret = bspUartRecv(pbData,1,1);	
	}
	else{
		ret = bspUartRecv(pbData,1,0);	
	}
	return (ret == 1)?TRUE:FALSE;
}

/*****************************************************************************
 Function : xmodem_outbyte
 Brief    : xmodem输出字节，移植无需修改
 Input    : [UINT8 bData]: 输出字符
 Output   : null
 Return   : void
 Author   : R.Y.
 Date     : 2016-05-17
*****************************************************************************/
void xmodem_outbyte(UINT8 bData)
{
	INT8 ch = (INT8)bData;
	bspUartSend(&ch,1,0);
}

/*****************************************************************************
 Function : xmodem_crc16_ccitt
 Brief    : crc16_ccitt校验核心算法
 Input    : [const UINT8 *pbBuf]: 待校验数据首地址
            [UINT32 dwLen]      : 待校验数据长度
 Output   : null
 Return   : [UINT16]: 校验结果
 Author   : R.Y.
 Date     : 2016-05-17
*****************************************************************************/
static UINT16 xmodem_crc16_ccitt(const UINT8 *pbBuf, UINT32 dwLen)
{
	UINT16 wCRC = 0;
	UINT8  bCycle;

	while (dwLen-- > 0) {
		wCRC ^= (unsigned short) *pbBuf++ << 8;
		for (bCycle = 0; bCycle < 8; bCycle++){
			if (wCRC & 0x8000){
				wCRC = wCRC << 1 ^ 0x1021;
			}else{
				wCRC <<= 1;
			}
		}
	}
	return wCRC;
}

/*****************************************************************************
 Function : xmodem_check
 Brief    : xmodem校验，根据选择的校验方式进行，有累加校验和CRC校验
 Input    : [UINT8 bCheckMode]  : 校验方式
            [const UINT8 *pbBuf]: 校验数据首地址
            [UINT32 dwLen]      : 校验数据长度
 Output   : null
 Return   : [UINT8]: 返回校验结果，TRUE是成功，FALSE是失败
 Author   : R.Y.
 Date     : 2016-05-17
*****************************************************************************/
static UINT8 xmodem_check(UINT8 bCheckMode, const UINT8 *pbBuf, UINT32 dwLen)
{
	if (bCheckMode){
		UINT16 wCRC    = xmodem_crc16_ccitt(pbBuf, dwLen);
		UINT16 wCRCRef = (pbBuf[dwLen]<<8)+pbBuf[dwLen+1];
		if (wCRC == wCRCRef){
			return TRUE;
		}
	}else{
		UINT32 dwCycle;
		UINT8  bSum = 0;
		for (dwCycle = 0; dwCycle < dwLen; ++dwCycle){
			bSum += pbBuf[dwCycle];
		}
		if (bSum == pbBuf[dwLen]){
			return TRUE;
		}
	}

	return FALSE;
}

/*****************************************************************************
 Function : xmodem_flushinput
 Brief    : 
 Input    : null
 Output   : null
 Return   : void
 Author   : R.Y.
 Date     : 2016-05-17
*****************************************************************************/
static void xmodem_flushinput(void)
{
	UINT8  bDummyByte;
	
	while (xmodem_inbyte(((DLY_1S)*3)>>1,&bDummyByte) == TRUE){
		bXmodemExit=XMODEM_EXIT;
		break;
	}
}

/*****************************************************************************
 Function : xmodemReceive
 Brief    : xmodem接收主流程
 Input    : [UINT8 *pbBuf]    : 接收缓存首地址
            [UINT32 dwBufSize]: 接收缓存大小
 Output   : null
 Return   : [INT8]: 接收结果，0是接收正常，负数是接收失败。
 Author   : R.Y.
 Date     : 2016-05-17
*****************************************************************************/
INT8 xmodemReceive(UINT8 *pbBuf, UINT32 dwBufSize, UINT32 * fileSize)
{
	UINT8 *pbData;                  // 接收缓存指针
	UINT32 dwPktDataLen;            // 信息帧中数据区长度
	UINT8  bXmodemCheckMode = 0;    // xmodem校验方式:0是累加，1是CRC
	UINT8  bTryChar = 'C';          // 传输启动环节的发送字符
	UINT8  bPktNum = 1;             // 接收包序号
	UINT32 dwCycle;                 //
	UINT32 dwOffset = 0;            // 与接收缓存首地址相对的偏移量
	UINT8  bData;                   // 接收到的数据
	UINT8  bRetry;                  // 错误超时的重复次数
	UINT8  bRetrans = MAXRETRANS;   // 最大重复接收次数
	
	
	for(;;) {
		for( bRetry = 0; bRetry < MAXRETRANS; ++bRetry) {
			
			if (xmodem_inbyte(DLY_1S*8, &bData) == TRUE) {
				switch (bData) {
				case SOH:
					dwPktDataLen = 128;
					goto start_recv;
				case STX:
					dwPktDataLen = 1024;
					goto start_recv;
				case ETX:
					xmodem_flushinput();
					xmodem_outbyte(ACK);
					for(dwCycle=0; dwCycle<50000; dwCycle++);
					return -4; /* interrupt by remote */
					break;
				case EOT:
					xmodem_flushinput();
					xmodem_outbyte(ACK);
					for(dwCycle=0; dwCycle<50000; dwCycle++);
					*fileSize = dwOffset;
					printfk("\n\rXmodem Receive OK, FileSize %dKB\n\r", dwOffset/1024);
					return 0; /* normal end */
				case CAN:
					if ((xmodem_inbyte(DLY_1S, &bData) == TRUE)&&(bData==CAN)) {
						xmodem_flushinput();
						xmodem_outbyte(ACK);
						for(dwCycle=0; dwCycle<50000; dwCycle++);
						printfk("\n\rXmodem Receive Canceled By Remote\n\r");
						return -1; /* canceled by remote */
					}
					break;
				default:
					break;
				}
			}
			if (bTryChar) xmodem_outbyte(bTryChar);
		}
		if (bTryChar == 'C'){
			bTryChar = NAK; 
			continue;
		}
		xmodem_flushinput();
		xmodem_outbyte(CAN);
		xmodem_outbyte(CAN);
		xmodem_outbyte(CAN);
		printfk("\n\rXmodem Receive Sync Error\n\r");
		return -2; /* sync error */

	start_recv:
		if (bTryChar == 'C') bXmodemCheckMode = 1;
		bTryChar = 0;
		pbData = bXmodemBuf;
		*pbData++ = bData;
		for (dwCycle = 0;  dwCycle < (dwPktDataLen+(bXmodemCheckMode?1:0)+3); ++dwCycle) {
			if (xmodem_inbyte(DLY_1S, &bData) == FALSE){
				goto reject;
			}
			*pbData++ = bData;
		}

		if (bXmodemBuf[1] == (unsigned char)(~bXmodemBuf[2]) && 
			(bXmodemBuf[1] == bPktNum || bXmodemBuf[1] == (unsigned char)bPktNum-1) &&
			xmodem_check(bXmodemCheckMode, &bXmodemBuf[3], dwPktDataLen)) {
			if (bXmodemBuf[1] == bPktNum){
				register int count = dwBufSize - dwOffset;
				if (count > dwPktDataLen){
					count = dwPktDataLen;
				}
				if (count > 0) {
					memcpy (&pbBuf[dwOffset], &bXmodemBuf[3], count);
					dwOffset += count;
				}
				++bPktNum;
				bRetrans = MAXRETRANS+1;
			}
			if (--bRetrans == 0) {
				xmodem_flushinput();
				xmodem_outbyte(CAN);
				xmodem_outbyte(CAN);
				xmodem_outbyte(CAN);
				printfk("\n\rXmodem Receive Canceled Retry Too Many Times\n\r");
				return -3; /* too many bRetry error */
			}
			xmodem_outbyte(ACK);
			continue;
		}
	reject:
		xmodem_flushinput();
		xmodem_outbyte(NAK);
	}
}



/******************************************************************************
 End of the File (EOF). Do not put anything after this part!
******************************************************************************/

