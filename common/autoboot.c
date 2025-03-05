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
    /* boot��flash�еĵ�ַ */
    flash_ptr = (UINT32 *)srcAddr;
    /* ��ȡboot��������ڵ�ַ */
    entryAddr=*(flash_ptr++);
    while(1){
        /* ��ȡ�γߴ� */
        secsize = *(flash_ptr++);
        /* �γߴ絥λΪ�ֽڣ�Ҫת����int�ĳ��� */
        loopcnt = secsize/4 + ((secsize%4)?1:0);
        if(loopcnt == 0){
            break;
        }
        /* ��ȡ����ʼ��ַ */
        secaddr = *(flash_ptr++);
        /* ��ӴӺ˵�L2 SRAM����ַ */
        secaddr += 0x10000000 + coreNum*0x1000000;

        for(i=0;i<loopcnt;i++){
            do{
                *(volatile unsigned int *)(secaddr+4*i)=*(flash_ptr+i);
            }
            while(*( volatile unsigned int *)(secaddr+4*i)!=*(flash_ptr+i));/*�ж�һ��*/
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
    /* boot��flash�еĵ�ַ */
    flash_ptr = (UINT32 *)srcAddr;
    /* ��ȡboot��������ڵ�ַ */
    entryAddr=*(flash_ptr++);
    while(1){
        /* ��ȡ�γߴ� */
        secsize = *(flash_ptr++);
        /* �γߴ絥λΪ�ֽڣ�Ҫת����int�ĳ��� */
        loopcnt = secsize/4 + ((secsize%4)?1:0);
        if(loopcnt == 0){
            break;
        }
        /* ��ȡ����ʼ��ַ */
        secaddr = *(flash_ptr++);

        for(i=0;i<loopcnt;i++){
            do{
                *(volatile unsigned int *)(secaddr+4*i)=*(flash_ptr+i);
            }
            while(*( volatile unsigned int *)(secaddr+4*i)!=*(flash_ptr+i));/*�ж�һ��*/
        }
        flash_ptr+=loopcnt;
    }
    return entryAddr;
}