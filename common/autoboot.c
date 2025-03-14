#include <stdio.h>
#include <c6x.h>
#include <csl_cacheAux.h>
#include "bspInterface.h"

UINT32 reload_dat_boot(UINT32 srcAddr,UINT32 coreNum)
{   
    UINT32 * flash_ptr;
    UINT32 entryAddr = 0;
    UINT32 secaddr,secsize,loopcnt;
    UINT32 i;
    
    if(coreNum>7){
        return 0;
    }
    /* boot在flash中的地址 */
    flash_ptr = (UINT32 *)srcAddr;
    /* 获取boot的运行入口地址 */
    entryAddr=*(flash_ptr++);
    while(1){
        /* 获取段尺寸 */
        secsize = *(flash_ptr++);
        /* 段尺寸单位为字节，要转换成int的长度 */
        loopcnt = secsize/4 + ((secsize%4)?1:0);
        if(loopcnt == 0){
            break;
        }
        /* 获取段起始地址 */
        secaddr = *(flash_ptr++);
        /* 添加从核的L2 SRAM基地址 */
        secaddr += 0x10000000 + coreNum*0x1000000;

        for(i=0;i<loopcnt;i++){
            do{
                *(volatile unsigned int *)(secaddr+4*i)=*(flash_ptr+i);
            }
            while(*( volatile unsigned int *)(secaddr+4*i)!=*(flash_ptr+i));/*判断一下*/
        }
        flash_ptr+=loopcnt;
    }
    return entryAddr;
}
UINT32 reload_dat_app(UINT32 srcAddr,UINT32 coreNum)
{   
    UINT32 * flash_ptr;
    UINT32 entryAddr;
    UINT32 secaddr,secsize,loopcnt;
    UINT32 i;
    
    if(coreNum>7){
        return 0;
    }
    /* boot在flash中的地址 */
    flash_ptr = (UINT32 *)srcAddr;
    /* 获取boot的运行入口地址 */
    entryAddr=*(flash_ptr++);
    while(1){
        /* 获取段尺寸 */
        secsize = *(flash_ptr++);
        /* 段尺寸单位为字节，要转换成int的长度 */
        loopcnt = secsize/4 + ((secsize%4)?1:0);
        if(loopcnt == 0){
            break;
        }
        /* 获取段起始地址 */
        secaddr = *(flash_ptr++);

        for(i=0;i<loopcnt;i++){
            do{
                *(volatile unsigned int *)(secaddr+4*i)=*(flash_ptr+i);
            }
            while(*( volatile unsigned int *)(secaddr+4*i)!=*(flash_ptr+i));/*判断一下*/
        }
        flash_ptr+=loopcnt;
    }
    return entryAddr;
}
void autoboot(UINT32 blocknum)
{
	unsigned int i,j;
	unsigned int entryAddr;
	unsigned int * flash_ptr;
	int flashRet = 0;
	int i_move = 1;
	void  (*exit)();

	if((blocknum == 0) || (blocknum > 31)){
		printfk("autoboot block %d is invalid\r\n",blocknum);
		return ;
	}

	/*核0 的时候处理措施*/
	if(DNUM == 0)
	{
		//核0将boot搬运到每个从核的boot运行段
		dspFlashAddrSwitch(0);
		for(i_move = 1;i_move<8;i_move++){
			entryAddr = reload_dat_boot(FLASH_STARTUP_ADDRS_BOOT,i_move);
		}

		flashRet = dspFlashAddrSwitch(blocknum);  
		if(flashRet == RET_SUCCESS)  //切换FLASH地址块正确，切换到正确的地址
		{
			bspUartPrintString("switch DSP's flash successful\r\n");
			//切换到目标flash地址,用于加载FLASH内数据并启动APP
			flash_ptr=(unsigned int *)FLASH_STARTUP_ADDRS_APP;
			for(i = 0;i < APP_FLASH_LEN;i+=4){
				*(unsigned int *)(DDR_TEMP_ADDRS_CODE + i) = *(unsigned int *)(FLASH_STARTUP_ADDRS_APP + i);
			}
		}
		else  //切换地址失败，不启动任何APP
		{
			bspUartPrintString("switch DSP's flash failed,please check fmql\r\n");
		}
	}
	else
	{
		;
	}

	/* 每个核清空自己系统运行的DDR空间0x80000000~0x80FFFFFF，对应物理地址空间0x80000000~0x87FFFFFF */
	for(i=0;i<0x1000000;i+=4){
		*(unsigned int*)(0x80000000 + i) = 0;
	}
	entryAddr = reload_dat_app(DDR_TEMP_ADDRS_CODE,DNUM);

	/* 主核启动从核开始运行BOOT */
	if(DNUM == 0){
		KICK0 = KICK0_UNLOCK;
		KICK1 = KICK1_UNLOCK;
		for(i=1;i<8;i++){
			BOOT_MAGIC_ADDR(i) = (uint32_t)_c_int00;
			IPCGR(i) |= 1;
		}
		KICK0 = KICK_LOCK;
		KICK1 = KICK_LOCK;

		CACHE_enableCaching(128);
		CACHE_enableCaching(129);
		CACHE_enableCaching(130);
		CACHE_enableCaching(131);
		CACHE_enableCaching(132);
		CACHE_enableCaching(133);
		CACHE_enableCaching(134);
		CACHE_enableCaching(135);
		for(i=0;i<1000000;i++);
	}

	/* 主核在这里写同步标志 */
	*(unsigned int*)(0xa3000020 + DNUM*4) = entryAddr;
	/* 从核等待主核完成应用搬运 */
	if(DNUM > 0){
		while(*(unsigned int*)(0xa3000020)!= entryAddr);
		for(j=0;j<20000000+1000000*(DNUM-1);j++);
		printfk("%d ",DNUM);
		if(DNUM == 7){
			bspUartPrintString("\r\n");
		}
	}

	/* 所有核跳转到应用入口 */
   *(unsigned int *)BOOT_MAGIC_ADDR(DNUM)=entryAddr;
	 exit = (void(*)())entryAddr;
	 (*exit)();//跳到应用程序入口

}

INT32 abortboot(void)
{
	INT32 i,enterTimes = 0;
	INT8 ch;

	printfk("\n\rPress esc 3 times to stop autoboot\n\r");
	for(i=0;i<3;i++){
		if (bspUartRecv((unsigned char*)&ch,1,0) == 1) {
			if('\e'== ch)
				enterTimes++;
		}
		if(enterTimes==3){
			return 1;
		}
	}
	return 0;
}
