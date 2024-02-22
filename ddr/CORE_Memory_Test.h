/*******************************************************************************
*文件名：CORE_Memory_Test.h
*作    用： 用于FT-M6678 存储体测试的接口函数声明。
*版    本：Version 1.0  创建于2019.04.29
*******************************************************************************/
#ifndef DSP_CORE_TEST_H_
#define DSP_CORE_TEST_H_

extern void DSP_memory_test(unsigned int uiStartAddress,
	                    unsigned int uiStopAddress,
	                    unsigned int uiStep,
	                    char * mem_name);
extern void pass_fail_count(int iResult);

extern unsigned int MEM_FillTest(unsigned int uiStartAddress,
                        unsigned int uiCount,
                        unsigned long long ulBitPattern,
                        unsigned int uiStep);
extern unsigned int MEM_AddrTest(unsigned int uiStartAddress,
						unsigned int uiCount,
                        int iStep);
extern unsigned int MEM_Bit_Walking(unsigned int uiStartAddress,
						unsigned int uiCount,
                        unsigned int uiStep);
extern int DSP_core_MEM_Test(unsigned int uiStartAddress,
		                unsigned int uiStopAddress,
		                unsigned int uiStep);

#endif /* DSP_CORE_TEST_H_ */
