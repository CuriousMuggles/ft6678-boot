#include <stdio.h>
#include "bspInterface.h"

/***************************************************************************
�������ƣ�INT32 bspBspVersionGet(VERSION * pVersion)
�������ܣ�BSP�汾ʱ����Ϣ��ȡ
�����������
���������pVersion����ȡ���İ汾��Ϣ
����ֵ��RET_SUCCESS-�ɹ�
        RET_ERROR-ʧ��
Լ����������
*****************************************************************************/
INT32 bspBspVersionGet(VERSION * pVersion)
{
	char version_s[5],version_c;
	
	sscanf(DSP_BOOT_VERSION,"%d.%d.%d",&pVersion->version1,&pVersion->version2,&pVersion->version3);

	sscanf(__TIME__,"%d:%d:%d",&(pVersion->hour), &(pVersion->minute), &(pVersion->second));
	
	sscanf(__DATE__,"%3s  %d %d",version_s, &(pVersion->day), &(pVersion->year));
	
	version_s[4] = '\0';
	
	if((version_s[0] == 'J')&&(version_s[1] == 'a')&&(version_s[2] == 'n'))
	{
		pVersion->month = 1;
	}
	else if((version_s[0] == 'F')&&(version_s[1] == 'e')&&(version_s[2] == 'b'))
	{
		pVersion->month = 2;
	}
	else if((version_s[0] == 'M')&&(version_s[1] == 'a')&&(version_s[2] == 'r'))
	{
		pVersion->month = 3;
	}
	else if((version_s[0] == 'A')&&(version_s[1] == 'p')&&(version_s[2] == 'r'))
	{
		pVersion->month = 4;
	}
	else if((version_s[0] == 'M')&&(version_s[1] == 'a')&&(version_s[2] == 'y'))
	{
		pVersion->month = 5;
	}
	else if((version_s[0] == 'J')&&(version_s[1] == 'u')&&(version_s[2] == 'n'))
	{
		pVersion->month = 6;
	}
	else if((version_s[0] == 'J')&&(version_s[1] == 'u')&&(version_s[2] == 'l'))
	{
		pVersion->month = 7;
	}
	else if((version_s[0] == 'A')&&(version_s[1] == 'u')&&(version_s[2] == 'g'))
	{
		pVersion->month = 8;
	}
	else if((version_s[0] == 'S')&&(version_s[1] == 'e')&&(version_s[2] == 'p'))
	{
		pVersion->month = 9;
	}
	else if((version_s[0] == 'O')&&(version_s[1] == 'c')&&(version_s[2] == 't'))
	{
		pVersion->month = 10;
	}
	else if((version_s[0] == 'N')&&(version_s[1] == 'o')&&(version_s[2] == 'v'))
	{
		pVersion->month = 11;
	}
	else if((version_s[0] == 'D')&&(version_s[1] == 'e')&&(version_s[2] == 'c'))
	{
		pVersion->month = 12;
	}
	else
	{
		return RET_ERROR;
	}
	
	return RET_SUCCESS;
}
/***************************************************************************
�������ƣ�void bspVersionInfoShow(void)
�������ܣ���ӡBSP����汾��Ϣ
�����������
�����������
����ֵ����
Լ����������
*****************************************************************************/
void bspVersionInfoShow(void)
{
	VERSION  version={0};
	INT32 i;

	bspPrintf("\r\n======================SOFT VERSION INFO=====================\r\n",0,1,2,3,4,5);
	bspBspVersionGet(&version);
	bspPrintf("BOOT  Version: [%d.%02d.%02d] ",version.version1,version.version2,version.version3,3,4,5);
	bspPrintf("Build Time: %04d-%02d-%02d  ",version.year,version.month,version.day,3,4,5);
	bspPrintf("%02d:%02d:%02d\r\n",version.hour,version.minute,version.second,3,4,5);
	bspPrintf("=============================================================\r\n",0,1,2,3,4,5);
}
