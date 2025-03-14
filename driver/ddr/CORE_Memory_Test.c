/*******************************************************************************
*文件名：CORE_Memory_Test.c
*作    用： 用于实现FT-M6678存储体测试的接口函数,参考了DM6672V的代码
*版    本：Version 1.0  创建于2019.04.29
*注    意：程序中可以通过设置 PRINT_DETAILS的值来开关打印信息。
*******************************************************************************/
#include <stdio.h>
#include "CORE_Memory_Test.h"

/*********************测试选择区域*************************************************/
#define  BIT_PATTERN_FILLING_TEST                     /* 数据测试 */
#define  ADDRESS_TEST                                 /* 地址测试 */
#define  BIT_WALKING_TEST                             /* 走bit测试*/

#define  MAX_FILL_FAIL_COUNT 		(10)              /* 填充测试错误最大值限制*/
#define  MAX_ADDRESS_FAIL_COUNT 		(10)          /* 地址测试错误最大值限制*/
#define  MAX_BITWALKING_FAIL_COUNT 	(2)               /* 走bit测试错误最大值限制*/
#define  MAX_BITWALKING_RANGE 		(1024*1024*1024)  /*走bit测试最大限制*/

/*实际使用时可以将PRINT_DETAILS设置为0，屏蔽打印信息。*/
#define  PRINT_DETAILS 	0                             /* 打印调试信息选择开关*/
#if      PRINT_DETAILS
#define  PRINT	 		printf
#else
#define  PRINT			//
#endif

/* 填充测试时使用的数据源 */
unsigned long long ulDataPatternTable[] = { 
	0x0000000000000000, 
	0xffffffffffffffff, 
	0xaaaaaaaaaaaaaaaa, 
	0x5555555555555555,
	0xcccccccccccccccc, 
	0xf0f0f0f0f0f0f0f0, 
	0xff00ff00ff00ff00, 
    0xffff0000ffff0000, 
    0xffffffff00000000
};

/*******************************************************************************
*函数名：pass_fail_count(iResult)
*功    能： 测试通过判断函数。
*参    数：intiResult  测试结束后传递过来的错误次数
*返回值：无
*******************************************************************************/
void pass_fail_count(int iResult)
{
	if(iResult)
		printf("\n   存储体测试不通过! \n");
	else
		printf("\n   存储体测试通过! \n");
}

/*******************************************************************************
*函数名：KeyStone_memory_test(unsigned int uiStartAddress, unsigned int uiStopAddress, unsigned int uiStep, char * mem_name)
*功    能： 存储体测试
*参    数：uiStartAddress  存储体起始地址
*      uiStopAddress  存储体结束地址
*      uiStep         填充的数据的间隔，间隔   = uiStep*2 个字，所以1表示连续填充，正常测试选1即可。
*      mem_name       测试存储区域的名字
*返回值：无
*说    明：1、函数内部有打印信息，三种测试均有过程说明。
*      2、测试通过打印“测试通过”，否则打印“测试不通过”。
*      3、常规连续测试，uiStep设置为1即可。
*******************************************************************************/
void DSP_memory_test(unsigned int uiStartAddress,
	unsigned int uiStopAddress, unsigned int uiStep, char * mem_name)
{
	printf("\n   对   %s 区域进行测试   \n", mem_name);
	pass_fail_count(DSP_core_MEM_Test(uiStartAddress, uiStopAddress, uiStep));
}

/*******************************************************************************
*函数名：MEM_FillTest(unsigned int uiStartAddress,unsigned int uiCount,unsigned long long ulBitPattern,unsigned int uiStep)
*功    能： 填充数据测试
*参    数：uiStartAddress  存储体起始地址
*      uiCount        填充数据的次数
*      ulBitPattern   填充的数据
*      uiStep         表示测试的步长
*返回值： uiFailCount    出错的次数
*注    意： 填充测试的原理是 给整片空间填充固定的数值。
*******************************************************************************/
unsigned int MEM_FillTest(unsigned int uiStartAddress,
                        unsigned int uiCount,
                        unsigned long long ulBitPattern,
                        unsigned int uiStep)
{
    unsigned int i, uiFailCount=0;
    volatile unsigned long long *ulpAddressPointer;
    volatile unsigned long long ulReadBack;

    /*对存储体填充数据，填充的数据为ulBitPattern*/
	ulpAddressPointer = (unsigned long long *)uiStartAddress;
	for(i=0; i<uiCount; i++)
	{
        *ulpAddressPointer = ulBitPattern;
        ulpAddressPointer += (uiStep);
    }

	/* 写完之后进行数据校验 */
	ulpAddressPointer = (unsigned long long *)uiStartAddress;
	for(i=0; i<uiCount; i++)
	{
        ulReadBack = *ulpAddressPointer;
        if ( ulReadBack!= ulBitPattern)
        {
			PRINT("  存储体测试在 0x%8x 位置出错, 写进去的数值为 0x%016llx, 读出来的数值为 0x%016llx\n", (unsigned int)ulpAddressPointer, ulBitPattern, ulReadBack);
            uiFailCount++;
			if(uiFailCount>=MAX_FILL_FAIL_COUNT)/* 错误次数超过MAX_FILL_FAIL_COUNT次直接退出*/
				return uiFailCount;
        }
        ulpAddressPointer += (uiStep);
    }
	return uiFailCount;
}

/*******************************************************************************
*函数名：MEM_AddrTest(unsigned int uiStartAddress, unsigned int uiCount, int iStep)
*功    能： 地址测试
*参    数：uiStartAddress  存储体起始地址
*      uiCount        填充的次数
*      uiStep          填充的数据的间隔，间隔   = uiStep*2 个字，所以1表示连续填充
*返回值： uiFailCount     出错的次数
*注    意：地址测试的原理是 给每个功能单元填充该功能单元的地址，比如在 地址0写数据0、地址1写数据1
*******************************************************************************/
unsigned int MEM_AddrTest(unsigned int uiStartAddress, 
						unsigned int uiCount,
                        int iStep)
{
    unsigned int i, uiFailCount=0;
    volatile unsigned long long *ulpAddressPointer;
    volatile unsigned long long ulReadBack;

	ulpAddressPointer = (unsigned long long *)uiStartAddress;
	for(i=0; i<uiCount; i++)
	{
		/* 用地址值进行填充   */
        *ulpAddressPointer = _itoll(((unsigned int)ulpAddressPointer)+4, 
        	(unsigned int)ulpAddressPointer);	  
        ulpAddressPointer += (iStep);
    }

	ulpAddressPointer = (unsigned long long *)uiStartAddress;
	for(i=0; i<uiCount; i++)
	{
        ulReadBack = *ulpAddressPointer;
        if ( ulReadBack != _itoll(((unsigned int)ulpAddressPointer)+4, 
        	(unsigned int)ulpAddressPointer)) /* 数据对比 */
        {
			PRINT("  在地址 0x%8x 测试出错, 写入的值为  0x%016llx, 读回来的值为  0x%016llx\n", (unsigned int)ulpAddressPointer, _itoll(((unsigned int)ulpAddressPointer)+4, (unsigned int)ulpAddressPointer), ulReadBack);
            uiFailCount++;
			if(uiFailCount>=MAX_ADDRESS_FAIL_COUNT) /* 错误次数超过MAX_ADDRESS_FAIL_COUNT次直接退出*/
				return uiFailCount;
        }
        ulpAddressPointer += (iStep);
    }
    return uiFailCount;
}

/*******************************************************************************
*函数名：MEM_Bit_Walking(unsigned int uiStartAddress,unsigned int uiCount,unsigned int uiStep)
*功    能： 走bit测试
*参    数：uiStartAddress  存储体起始地址
*      uiCount        填充的次数
*      uiStep          填充的数据的间隔，间隔   = uiStep*2 个字，所以1表示连续填充
*返回值： uiFailCount    标示出错的次数
*说    明：测试的原理为   1、 在全0数据中单独每一位赋1填充存储体； 2、 在全1数据中单独每一位赋0填充存储体
*******************************************************************************/
unsigned int MEM_Bit_Walking(unsigned int uiStartAddress,
						unsigned int uiCount,
                        unsigned int uiStep)
{
    unsigned int uiFailCount=0;
    unsigned int j;
    unsigned int uiBitMask=1;
    for (j = 0; j < 32; j++)
    {
    	PRINT("从地址 0x%8x 处填充数据： 0x%16llx\n",uiStartAddress, _itoll(uiBitMask, uiBitMask));
    	if(MEM_FillTest(uiStartAddress, uiCount, _itoll(uiBitMask, uiBitMask), uiStep))
    		uiFailCount++;

    	PRINT("从地址 0x%8x 处填充数据： 0x%16llx\n",uiStartAddress, _itoll(~uiBitMask, ~uiBitMask));
    	if(MEM_FillTest(uiStartAddress, uiCount, _itoll(~uiBitMask, ~uiBitMask), uiStep))
    		uiFailCount++;

    	if(uiFailCount>= MAX_BITWALKING_FAIL_COUNT)/* 错误次数超过MAX_BITWALKING_FAIL_COUNT次直接退出*/
			return uiFailCount;

        uiBitMask <<= 1;
    }
	return uiFailCount;
}

/*******************************************************************************
*函数名：DSP_core_MEM_Test(unsigned int uiStartAddress, unsigned int uiStopAddress, unsigned int uiStep)
*功    能： DSP内核访问存储体测试
*参    数：uiStartAddress   存储体起始地址，
*      uiStopAddress   存储体结束地址，
*      uiStep          填充的数据的间隔，间隔   = uiStep*2 个字，所以1表示连续填充
*返回值：uiTotalFailCount 出错的总次数
*说    明：该接口函数主要包含三个部分 1、数据填充测试  2、地址测试  3、走bit测试
*******************************************************************************/
int DSP_core_MEM_Test(unsigned int uiStartAddress, unsigned int uiStopAddress, unsigned int uiStep)
{
    unsigned int uiCount, uiFailCount=0, uiTotalFailCount=0;
    int j;

    /*uiCount为数据填充的次数*/
    uiCount = ((uiStopAddress - uiStartAddress)/8)/uiStep;

#ifdef  BIT_PATTERN_FILLING_TEST   /* 数据填充测试  */
    PRINT(" 1、数据填充测试开始！  \n" );
    /* 将数据表中的数据填充到测试空间  */
    for (j = 0; j < sizeof(ulDataPatternTable)/8; j++)
    {
        uiFailCount = MEM_FillTest(uiStartAddress, uiCount, ulDataPatternTable[j], uiStep);
	    if (uiFailCount)
	    {
	    	PRINT(" 出现了%d 次错误，在填充数据 0x%016llx时。  \n",uiFailCount, ulDataPatternTable[j]);
	    	uiTotalFailCount+=uiFailCount;
	    }
		else
		    PRINT(" 从地址  0x%8x 到  地址0x%8x 填充数据  0x%16llx 通过！ \n",uiStartAddress,uiStopAddress, ulDataPatternTable[j]);
    }
    PRINT(" 1、数据填充测试结束！  \n" );
#endif

#ifdef  ADDRESS_TEST
    PRINT(" \n 2、 地址测试开始！  \n" );
    /* 从低到高进行地址测试  */
    uiFailCount = MEM_AddrTest(uiStartAddress, uiCount, uiStep);
    if (uiFailCount)
    {
    	PRINT("  在地址测试时出现 %d 次错误！ \n",uiFailCount);
        uiTotalFailCount+=uiFailCount;
    }
	else
	   	PRINT("  从地址 0x%8x 到  地址 0x%8x 地址测试成功 ！\n",uiStartAddress,uiStopAddress);
    PRINT(" 2、地址测试结束！  \n" );
#endif

#ifdef  BIT_WALKING_TEST
    PRINT("\n 3、 走bit测试开始！  \n" );
    /* 走bit测试  */
    if(uiCount>MAX_BITWALKING_RANGE)
			uiCount= MAX_BITWALKING_RANGE;     /* 避免测试时间过长，此处对测试规模进行了限制，用户可自定义修改 */
    uiFailCount = MEM_Bit_Walking(uiStartAddress,uiCount, uiStep);
    if (uiFailCount)
    {
	   	PRINT(" 测试共有 %d 次出现错误   \n",uiFailCount);
        uiTotalFailCount+=uiFailCount;
    }    
	else
	  	PRINT("    从地址 0x%8x 到  地址 0x%8x 地址测试成功 ！ \n",uiStartAddress,uiStopAddress);
    PRINT(" 3、 走bit测试结束！  \n" );
#endif
  	return uiTotalFailCount;
}
