#include <stdio.h>
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