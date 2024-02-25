#include <stdio.h>
#include <string.h>
#include "S29GL064S_driver.h"

extern unsigned char g_dsp_num;
/************************EMIF �Ĵ���**********************************************************/
#define RCSR	     *(unsigned int*)0x20C00000   //�汾�����״̬�Ĵ���
#define AWCCR	 *(unsigned int*)0x20C00004   //�첽�ȴ��������üĴ���
#define A1CR	     *(unsigned int*)0x20C00010   //CE0�ռ����üĴ���
#define A2CR	     *(unsigned int*)0x20C00014   //CE0�ռ����üĴ���
#define A3CR	     *(unsigned int*)0x20C00018   //CE0�ռ����üĴ���
#define A4CR	     *(unsigned int*)0x20C0001c   //CE0�ռ����üĴ���




/*******************************************************************************
*��������EMIF_init
*��    �ܣ� EMIF��ʼ��
*��    ���� ��
*******************************************************************************/
void EMIF_init()
{
	RCSR=RCSR | 0x40000000;//EMIF������չ������32λ
	AWCCR=0xf0000080;      //CE[3:0]Ϊ�첽�洢
	/*EWλ��0����ֹ��չ�ȴ�ģʽ*/
	A1CR=0xBFFFFFFD;
	A2CR=0xBFFFFFFD;
	A3CR=0xBFFFFFFD;
	A4CR=0xBFFFFFFD;
}

void delay(int i)
{
	int j;
	for(j=0;j<=i;j++)
		asm("	nop");
}

/*******************************************************************************
*�������� Check_Toggle()
*��   �ܣ��û��ڲ���/��̵�ʱ���жϲ���/����Ƿ����
*�������:   1  ���/�����ɹ�
*          	  	   	0  ���/����ʧ��
*******************************************************************************/
int Check_Toggle()
{
	unsigned int Toggle_Status=0,Toggle_Status1=0;
	unsigned int DQ5=0;
	unsigned long TimeOut = 0;

	while(TimeOut < MAX_TIMEOUT)
	{
		Toggle_Status=(*(volatile short *)FLASH_ADR1);
		Toggle_Status1=(*(volatile short *)FLASH_ADR1);
		if((Toggle_Status & 0x00000040)==(Toggle_Status1 & 0x00000040))	{return TRUE ;}//�����ɹ�

		DQ5=Toggle_Status1 & 0x00000020;
		if(DQ5)
		{
			Toggle_Status=(*(volatile short *)FLASH_ADR1);
			Toggle_Status1=(*(volatile short *)FLASH_ADR1);
			if((Toggle_Status & 0x00000040)==(Toggle_Status1 & 0x00000040)){return TRUE ;}//�����ɹ�
			else *(volatile short *)FLASH_ADR1 = 0x00F0;		//��λflash
		}
		++TimeOut;
	}
	return FALSE;
}

/*******************************************************************************
*�������� EraseFlash()
*��   �ܣ���FLASH����ȫоƬ����
*�������:   1  �����ɹ�
*          	  	   	0  ����ʧ��
*******************************************************************************/
int EraseFlash(void)
{
    int ReturnStatus=TRUE;

	*(volatile short *)FLASH_ADR1 = 0x00AA;		//AAH
	*(volatile short *)FLASH_ADR2 = 0x0055;		//55H
	*(volatile short *)FLASH_ADR1 = 0x0080;		//80H

	*(volatile short *)FLASH_ADR1 = 0x00AA;		//AAH
	*(volatile short *)FLASH_ADR2 = 0x0055;		//55H
	*(volatile short *)FLASH_ADR1 = 0x0010;		//10H
	ReturnStatus = Check_Toggle(); // wait for TOGGLE bit ready
    return ReturnStatus;
}

/*******************************************************************************
*��������    Program_One_Sector
*��   �ܣ�    ��FLASH����д���ݣ�forѭ��ÿ��д2���ֽ�
*���������SrcWord����д��FLASH�Ļ����ַ���׵�ַ
*					Dst��д��FLASH��Ŀ�ĵ�ַ
*					BufByteSize����Ҫд��FLASH�Ĵ���=д��FLASH���ֽڳ���/2
*�������:   1  ��̳ɹ�
*          	  	   	0  ���ʧ��
*******************************************************************************/
int Program_One_Sector (unsigned int SrcWord,   unsigned int Dst,unsigned int BufByteSize )
{
    unsigned int  DestBuf = 0;
    unsigned int  SourceBuf = 0;
    unsigned int cnt=0;
    int ReturnStatus=TRUE;

    DestBuf = Dst;
    SourceBuf = SrcWord;

    for(cnt=0;cnt<BufByteSize;cnt++)
    {
    	*(volatile short *)FLASH_ADR1 = 0x00AA;	// write data 0x00AA to device addr 0x5555
    	*(volatile short *)FLASH_ADR2 = 0x0055;	// write data 0x0055 to device addr 0x2AAA
    	*(volatile short *)FLASH_ADR1 = 0x00A0; // write data 0x00A0 to device addr 0x5555
    	*(volatile short *)(system_base+DestBuf)=*((volatile short *)SourceBuf);// transfer the Byte to destination
    	ReturnStatus = Check_Toggle(); // wait for TOGGLE bit ready
    	if(!ReturnStatus)
        {
        	return ReturnStatus;
        }
		DestBuf=DestBuf+2;
		SourceBuf=SourceBuf+2;
    }
	return ReturnStatus;
}

/*******************************************************************************
*�������� S29GL64S_ID
*��   �ܣ�  ��ȡS29GL64S��ID��
*�����������
*�������:   TRUE  ��ȡ S29GL64S��ID��ȷ
*          	  	   	FALSE  ��ȡ S29GL64S��ID����
*******************************************************************************/
int S29GL64S_ID(void)
{
    unsigned short SST_id1;
    unsigned short SST_id2;
    unsigned short SST_id3;
    unsigned short SST_id4;
    int  ReturnStatus;
    //  Issue the Software Product ID code to S29GL064S
    *sysAddress(0x555) = 0x00AA;  // write data 0x00AA to device addr 0x555
    *sysAddress(0x2AA) = 0x0055;  // write data 0x0055 to device addr 0x2AA
    *sysAddress(0x555) = 0x0090;  // write data 0x0090 to device addr 0x5555
    delay(150);

    // Read the product ID from S29GL064S
    SST_id1  = *sysAddress(0x0000);   // get first ID byte
    SST_id2  = *sysAddress(0x0001);   // get second ID byte
    SST_id3  = *sysAddress(0x000e);   // get first ID byte
    SST_id4  = *sysAddress(0x000f);   // get second ID byte
    // ------------------------------------------------------------
    if ((SST_id1 == MANUFAC_ID) && (SST_id2 == S29GL064S_ID1)&&(SST_id3 == S29GL064S_ID2) && (SST_id4 == S29GL064S_ID3))
            ReturnStatus = TRUE;
    else
            ReturnStatus = FALSE;
    *sysAddress(0x000) = 0x00F0;  // RESET FLASH
    return (ReturnStatus);
}
