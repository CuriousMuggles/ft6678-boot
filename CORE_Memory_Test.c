/*******************************************************************************
*�ļ�����CORE_Memory_Test.c
*��    �ã� ����ʵ��FT-M6678�洢����ԵĽӿں���,�ο���DM6672V�Ĵ���
*��    ����Version 1.0  ������2019.04.29
*ע    �⣺�����п���ͨ������ PRINT_DETAILS��ֵ�����ش�ӡ��Ϣ��
*******************************************************************************/
#include <stdio.h>
#include "CORE_Memory_Test.h"

/*********************����ѡ������*************************************************/
#define  BIT_PATTERN_FILLING_TEST                     /* ���ݲ��� */
#define  ADDRESS_TEST                                 /* ��ַ���� */
#define  BIT_WALKING_TEST                             /* ��bit����*/

#define  MAX_FILL_FAIL_COUNT 		(10)              /* �����Դ������ֵ����*/
#define  MAX_ADDRESS_FAIL_COUNT 		(10)          /* ��ַ���Դ������ֵ����*/
#define  MAX_BITWALKING_FAIL_COUNT 	(2)               /* ��bit���Դ������ֵ����*/
#define  MAX_BITWALKING_RANGE 		(1024*1024*1024)  /*��bit�����������*/

/*ʵ��ʹ��ʱ���Խ�PRINT_DETAILS����Ϊ0�����δ�ӡ��Ϣ��*/
#define  PRINT_DETAILS 	0                             /* ��ӡ������Ϣѡ�񿪹�*/
#if      PRINT_DETAILS
#define  PRINT	 		printf
#else
#define  PRINT			//
#endif

/* ������ʱʹ�õ�����Դ */
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
*��������pass_fail_count(iResult)
*��    �ܣ� ����ͨ���жϺ�����
*��    ����intiResult  ���Խ����󴫵ݹ����Ĵ������
*����ֵ����
*******************************************************************************/
void pass_fail_count(int iResult)
{
	if(iResult)
		printf("\n   �洢����Բ�ͨ��! \n");
	else
		printf("\n   �洢�����ͨ��! \n");
}

/*******************************************************************************
*��������KeyStone_memory_test(unsigned int uiStartAddress, unsigned int uiStopAddress, unsigned int uiStep, char * mem_name)
*��    �ܣ� �洢�����
*��    ����uiStartAddress  �洢����ʼ��ַ
*      uiStopAddress  �洢�������ַ
*      uiStep         �������ݵļ�������   = uiStep*2 ���֣�����1��ʾ������䣬��������ѡ1���ɡ�
*      mem_name       ���Դ洢���������
*����ֵ����
*˵    ����1�������ڲ��д�ӡ��Ϣ�����ֲ��Ծ��й���˵����
*      2������ͨ����ӡ������ͨ�����������ӡ�����Բ�ͨ������
*      3�������������ԣ�uiStep����Ϊ1���ɡ�
*******************************************************************************/
void DSP_memory_test(unsigned int uiStartAddress,
	unsigned int uiStopAddress, unsigned int uiStep, char * mem_name)
{
	printf("\n   ��   %s ������в���   \n", mem_name);
	pass_fail_count(DSP_core_MEM_Test(uiStartAddress, uiStopAddress, uiStep));
}

/*******************************************************************************
*��������MEM_FillTest(unsigned int uiStartAddress,unsigned int uiCount,unsigned long long ulBitPattern,unsigned int uiStep)
*��    �ܣ� ������ݲ���
*��    ����uiStartAddress  �洢����ʼ��ַ
*      uiCount        ������ݵĴ���
*      ulBitPattern   ��������
*      uiStep         ��ʾ���ԵĲ���
*����ֵ�� uiFailCount    ����Ĵ���
*ע    �⣺ �����Ե�ԭ���� ����Ƭ�ռ����̶�����ֵ��
*******************************************************************************/
unsigned int MEM_FillTest(unsigned int uiStartAddress,
                        unsigned int uiCount,
                        unsigned long long ulBitPattern,
                        unsigned int uiStep)
{
    unsigned int i, uiFailCount=0;
    volatile unsigned long long *ulpAddressPointer;
    volatile unsigned long long ulReadBack;

    /*�Դ洢��������ݣ���������ΪulBitPattern*/
	ulpAddressPointer = (unsigned long long *)uiStartAddress;
	for(i=0; i<uiCount; i++)
	{
        *ulpAddressPointer = ulBitPattern;
        ulpAddressPointer += (uiStep);
    }

	/* д��֮���������У�� */
	ulpAddressPointer = (unsigned long long *)uiStartAddress;
	for(i=0; i<uiCount; i++)
	{
        ulReadBack = *ulpAddressPointer;
        if ( ulReadBack!= ulBitPattern)
        {
			PRINT("  �洢������� 0x%8x λ�ó���, д��ȥ����ֵΪ 0x%016llx, ����������ֵΪ 0x%016llx\n", (unsigned int)ulpAddressPointer, ulBitPattern, ulReadBack);
            uiFailCount++;
			if(uiFailCount>=MAX_FILL_FAIL_COUNT)/* �����������MAX_FILL_FAIL_COUNT��ֱ���˳�*/
				return uiFailCount;
        }
        ulpAddressPointer += (uiStep);
    }
	return uiFailCount;
}

/*******************************************************************************
*��������MEM_AddrTest(unsigned int uiStartAddress, unsigned int uiCount, int iStep)
*��    �ܣ� ��ַ����
*��    ����uiStartAddress  �洢����ʼ��ַ
*      uiCount        ���Ĵ���
*      uiStep          �������ݵļ�������   = uiStep*2 ���֣�����1��ʾ�������
*����ֵ�� uiFailCount     ����Ĵ���
*ע    �⣺��ַ���Ե�ԭ���� ��ÿ�����ܵ�Ԫ���ù��ܵ�Ԫ�ĵ�ַ�������� ��ַ0д����0����ַ1д����1
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
		/* �õ�ֵַ�������   */
        *ulpAddressPointer = _itoll(((unsigned int)ulpAddressPointer)+4, 
        	(unsigned int)ulpAddressPointer);	  
        ulpAddressPointer += (iStep);
    }

	ulpAddressPointer = (unsigned long long *)uiStartAddress;
	for(i=0; i<uiCount; i++)
	{
        ulReadBack = *ulpAddressPointer;
        if ( ulReadBack != _itoll(((unsigned int)ulpAddressPointer)+4, 
        	(unsigned int)ulpAddressPointer)) /* ���ݶԱ� */
        {
			PRINT("  �ڵ�ַ 0x%8x ���Գ���, д���ֵΪ  0x%016llx, ��������ֵΪ  0x%016llx\n", (unsigned int)ulpAddressPointer, _itoll(((unsigned int)ulpAddressPointer)+4, (unsigned int)ulpAddressPointer), ulReadBack);
            uiFailCount++;
			if(uiFailCount>=MAX_ADDRESS_FAIL_COUNT) /* �����������MAX_ADDRESS_FAIL_COUNT��ֱ���˳�*/
				return uiFailCount;
        }
        ulpAddressPointer += (iStep);
    }
    return uiFailCount;
}

/*******************************************************************************
*��������MEM_Bit_Walking(unsigned int uiStartAddress,unsigned int uiCount,unsigned int uiStep)
*��    �ܣ� ��bit����
*��    ����uiStartAddress  �洢����ʼ��ַ
*      uiCount        ���Ĵ���
*      uiStep          �������ݵļ�������   = uiStep*2 ���֣�����1��ʾ�������
*����ֵ�� uiFailCount    ��ʾ����Ĵ���
*˵    �������Ե�ԭ��Ϊ   1�� ��ȫ0�����е���ÿһλ��1���洢�壻 2�� ��ȫ1�����е���ÿһλ��0���洢��
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
    	PRINT("�ӵ�ַ 0x%8x ��������ݣ� 0x%16llx\n",uiStartAddress, _itoll(uiBitMask, uiBitMask));
    	if(MEM_FillTest(uiStartAddress, uiCount, _itoll(uiBitMask, uiBitMask), uiStep))
    		uiFailCount++;

    	PRINT("�ӵ�ַ 0x%8x ��������ݣ� 0x%16llx\n",uiStartAddress, _itoll(~uiBitMask, ~uiBitMask));
    	if(MEM_FillTest(uiStartAddress, uiCount, _itoll(~uiBitMask, ~uiBitMask), uiStep))
    		uiFailCount++;

    	if(uiFailCount>= MAX_BITWALKING_FAIL_COUNT)/* �����������MAX_BITWALKING_FAIL_COUNT��ֱ���˳�*/
			return uiFailCount;

        uiBitMask <<= 1;
    }
	return uiFailCount;
}

/*******************************************************************************
*��������DSP_core_MEM_Test(unsigned int uiStartAddress, unsigned int uiStopAddress, unsigned int uiStep)
*��    �ܣ� DSP�ں˷��ʴ洢�����
*��    ����uiStartAddress   �洢����ʼ��ַ��
*      uiStopAddress   �洢�������ַ��
*      uiStep          �������ݵļ�������   = uiStep*2 ���֣�����1��ʾ�������
*����ֵ��uiTotalFailCount ������ܴ���
*˵    �����ýӿں�����Ҫ������������ 1������������  2����ַ����  3����bit����
*******************************************************************************/
int DSP_core_MEM_Test(unsigned int uiStartAddress, unsigned int uiStopAddress, unsigned int uiStep)
{
    unsigned int uiCount, uiFailCount=0, uiTotalFailCount=0;
    int j;

    /*uiCountΪ�������Ĵ���*/
    uiCount = ((uiStopAddress - uiStartAddress)/8)/uiStep;

#ifdef  BIT_PATTERN_FILLING_TEST   /* ����������  */
    PRINT(" 1�����������Կ�ʼ��  \n" );
    /* �����ݱ��е�������䵽���Կռ�  */
    for (j = 0; j < sizeof(ulDataPatternTable)/8; j++)
    {
        uiFailCount = MEM_FillTest(uiStartAddress, uiCount, ulDataPatternTable[j], uiStep);
	    if (uiFailCount)
	    {
	    	PRINT(" ������%d �δ������������ 0x%016llxʱ��  \n",uiFailCount, ulDataPatternTable[j]);
	    	uiTotalFailCount+=uiFailCount;
	    }
		else
		    PRINT(" �ӵ�ַ  0x%8x ��  ��ַ0x%8x �������  0x%16llx ͨ���� \n",uiStartAddress,uiStopAddress, ulDataPatternTable[j]);
    }
    PRINT(" 1�����������Խ�����  \n" );
#endif

#ifdef  ADDRESS_TEST
    PRINT(" \n 2�� ��ַ���Կ�ʼ��  \n" );
    /* �ӵ͵��߽��е�ַ����  */
    uiFailCount = MEM_AddrTest(uiStartAddress, uiCount, uiStep);
    if (uiFailCount)
    {
    	PRINT("  �ڵ�ַ����ʱ���� %d �δ��� \n",uiFailCount);
        uiTotalFailCount+=uiFailCount;
    }
	else
	   	PRINT("  �ӵ�ַ 0x%8x ��  ��ַ 0x%8x ��ַ���Գɹ� ��\n",uiStartAddress,uiStopAddress);
    PRINT(" 2����ַ���Խ�����  \n" );
#endif

#ifdef  BIT_WALKING_TEST
    PRINT("\n 3�� ��bit���Կ�ʼ��  \n" );
    /* ��bit����  */
    if(uiCount>MAX_BITWALKING_RANGE)
			uiCount= MAX_BITWALKING_RANGE;     /* �������ʱ��������˴��Բ��Թ�ģ���������ƣ��û����Զ����޸� */
    uiFailCount = MEM_Bit_Walking(uiStartAddress,uiCount, uiStep);
    if (uiFailCount)
    {
	   	PRINT(" ���Թ��� %d �γ��ִ���   \n",uiFailCount);
        uiTotalFailCount+=uiFailCount;
    }    
	else
	  	PRINT("    �ӵ�ַ 0x%8x ��  ��ַ 0x%8x ��ַ���Գɹ� �� \n",uiStartAddress,uiStopAddress);
    PRINT(" 3�� ��bit���Խ�����  \n" );
#endif
  	return uiTotalFailCount;
}
