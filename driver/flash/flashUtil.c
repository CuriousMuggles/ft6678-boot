#include <stdio.h>
#include <string.h>
#include "flash.h"
#include "flash_nor.h"

#define SWAP32(x)	(((x) >> 24 & 0xff) | ((x) >> 8 & 0xff00) | ((x) << 8 & 0xff0000) | ((x) << 24 & 0xff000000))
NOR_InfoObj gNorInfo =
{
	CSL_EMIF16_data_REGS,/*flashBase;                     */
	FLASH_BUS_WIDTH_2_BYTES,/*busWidth;                   */
	FLASH_BUS_WIDTH_2_BYTES,/*chipOperatingWidth;         */
	FLASH_BUS_WIDTH_2_BYTES/*maxTotalWidth;              */
};

#define LINELENGTH (100)
#define MAX_FILEINFO_NUMBER	(10)
typedef struct{
	char 	filename[50];
	int 	flashblock;
	int 	filetype;	/*0=dsp�ļ���1=fpga1�ļ�,2=fpga2�ļ���3=fpga3�ļ�*/
	char 	finishflag;
}FILE_INFO;
FILE_INFO gFileInfo[MAX_FILEINFO_NUMBER] = {0};
unsigned int gFileInfoNumber=0;

void dspFlashProgram(int length)
{
	unsigned int base_addr = 0xc000000;
    unsigned int Data_Tmp,temp,program_addr;
    unsigned int i,j;
    unsigned int read_data;
    unsigned int flash_program_addr = 0x70000000;

    unsigned int write_cnt;
    unsigned int num_bytes = 0x2000;

    NOR_erase( &gNorInfo, flash_program_addr, length );
    NOR_writeBytes(&gNorInfo,flash_program_addr, length, base_addr);
#if 0
  	write_cnt = length/num_bytes;
  	write_cnt ++;
  	for(i = 0;i < write_cnt;i++)
  	{
  		NOR_writeBytes(&gNorInfo,flash_program_addr + i*num_bytes, num_bytes, base_addr + i*num_bytes);
  	}
#endif
}
int dspRegister(FILE_INFO *pfileinfo)
{
	char filename[LINELENGTH];
	FILE *file;
	int i = 0,ret = 0;
	unsigned int sr[4] = {0};

	/*��Ҫ��д���ļ�*/
	sprintf(filename,"%s%s","C:\\ft6678_program_tool\\",pfileinfo->filename);
	file = fopen(filename,"rb");
	if(!file){
		printf("Can't Open file ""%s""!\n",filename);
		return -1;
	}
	printf("Load file \"%s\"...\n",filename);
	for(i=0;i<4;i++){
		fscanf(file,"%x",&(sr[i]));
		sr[i] = SWAP32(sr[i]);
		printf("%x ",sr[i]);
	}
	printf(" done\n");
	/*�л�flash��ַ��*/
	ret = dspFlashAddrSwitch(pfileinfo->flashblock);
	if(ret != pfileinfo->flashblock){
		printf("dspFlashAddrSwitch %d fail,ret = %d\n",pfileinfo->flashblock,ret);
		return -1;
	}
	else{
		printf("dspFlashAddrSwitch %d success!\n",pfileinfo->flashblock);
	}
	/*��д�ļ�*/
    NOR_erase( &gNorInfo, 0x70300000, 16 );
    NOR_writeBytes(&gNorInfo,0x70300000, 16, (unsigned int)sr);
	printf("%s program success!\n\n\n",pfileinfo->filename);
	pfileinfo->finishflag = 1;

	return 0;
}
int dspProgram(FILE_INFO *pfileinfo)
{
	char string[LINELENGTH],filename[LINELENGTH];
	unsigned int DataHead[6];
	unsigned int *pbuffer,memaddr=0xc000000;
	int filelength=0;
	int i,j,k,ret;
	FILE *file;

	/*������д�ļ���ʽ*/
	sscanf(pfileinfo->filename,"%*[^.].%s",string);
	/*��Ҫ��д���ļ�*/
	sprintf(filename,"%s%s","C:\\ft6678_program_tool\\",pfileinfo->filename);
	file = fopen(filename,"rb");
	if(!file){
		printf("Can't Open file ""%s""!\n",filename);
		return -1;
	}
	printf("Load file \"%s\"...\n",filename);
	/*����д�ļ���ȡ���ڴ���*/
	if(strcmp(string,"dat") == 0){
		/*��ȡ����ͷ����ȡ��д����*/
		for(j=0;j<6;j++)
		{
			fscanf(file,"%x",&DataHead[j]);
		}
		pbuffer = (unsigned int*)memaddr;
		for(j=0;j<DataHead[4];j++){
			fscanf(file,"%x",pbuffer);
			pbuffer++;
			/*��ӡ������*/
			for(k=0;k<10;k++)
			{
				if(j==DataHead[4]/10*k)
				{
					printf("%d0%%\n",k);
				}
			}
		}
		filelength = DataHead[4]*4;
		fclose(file);
	}
	else if(strcmp(string,"bin") == 0){
		fseek(file,0,SEEK_END);
		filelength = ftell(file);
		fseek(file,0,SEEK_SET);
		fread((void*)memaddr,sizeof(char),filelength,file);
		fclose(file);
	}
	else{
		;
	}

	printf("done,size is %d Byte!\n",filelength);
	for(i=0;i<gFileInfoNumber;i++){
		if(strcmp(pfileinfo->filename,gFileInfo[i].filename) == 0){
			/*�л�flash��ַ��*/
			ret = dspFlashAddrSwitch(gFileInfo[i].flashblock);
			if(ret != gFileInfo[i].flashblock){
				printf("dspFlashAddrSwitch %d fail,ret = %d\n",gFileInfo[i].flashblock,ret);
				return -1;
			}
			else{
				printf("dspFlashAddrSwitch %d success!\n",gFileInfo[i].flashblock);
			}
			/*��д�ļ�*/
			dspFlashProgram(filelength);
			printf("%s program success!\n\n\n",gFileInfo[i].filename);
			gFileInfo[i].finishflag = 1;
		}
	}

}

int flashUtil(void)
{
	char *fileListText="C:\\ft6678_program_tool\\ProgramFileList.txt";
	char string[LINELENGTH];
	FILE *file;
	int i,ret;

	file = fopen(fileListText,"rb");
	if(!file){
		printf("Can't Open file \"C:\\ft6678_program_tool\\ProgramFileList.txt\"!\n");
		return -1;
	}
	/*��ȡҪ��д�ļ���Ϣ*/
	while(gFileInfoNumber<MAX_FILEINFO_NUMBER){
		fgets(string,LINELENGTH,file);
		/*���ַ�Ϊ#����ʾ�ļ���������д*/
		if(string[0] == '#'){
			continue;
		}
		/*���ַ�Ϊ=����ʾ��д�ļ���ȡ����*/
		else if(string[0] == '='){
			break;
		}
		/*�����ļ�������д���*/
		else{
			ret = sscanf(string,"%d:%d:%s",&gFileInfo[gFileInfoNumber].filetype,&gFileInfo[gFileInfoNumber].flashblock,gFileInfo[gFileInfoNumber].filename);
			if(ret == 3){
				gFileInfoNumber++;
				printf(">>%s",string);
			}
			else{
				printf("%s parse errors,please check file ProgramFileList.txt format is %%d:%%d:%%s\n",string);
			}
		}
	}
	fclose(file);

	for(i=0;i<gFileInfoNumber;i++)
	{
		if(gFileInfo[i].finishflag == 1){
			continue;
		}
		switch(gFileInfo[i].filetype){
			case 0:{dspProgram(&gFileInfo[i]);break;}
			case 1:{break;}
			case 2:{break;}
			case 3:{break;}
			case 4:{dspRegister(&gFileInfo[i]);break;}
			default:break;
		}
	}
}
