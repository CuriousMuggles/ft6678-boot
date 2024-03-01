/*
 *============================================================================
 *
 * PROJECT:  DDR3 for M6678, MT41K256M16HA-125T
 * DATE   :
 * AUTHOR :  YHFT
 * REMARKS:
 * HISTORY:
 *
 *
 *    SPDX-License-Identifier: GPL-2.0+
 *
 *============================================================================
 */
#include "ddr/ddr_regdefine.h"
#include "ddr/CORE_Memory_Test.h"
#include "S29GL064S_driver.h"
#include "interface.h"
#include <c6x.h>
#include <csl_cacheAux.h>
#include <ti/csl/csl_xmc.h>
#include <ti/csl/csl_xmcAux.h>
#include <csl_types.h>
#include <csl_tsc.h>
#include "UART.h"


#define	cah1			*(unsigned int *)0x01840000
#define	cah2			*(unsigned int *)0x01840040
#define	KICK0 	        *(unsigned int *)0x02620038
#define	KICK1	        *(unsigned int *)0x0262003C
#define	MainPLLCTL0		*(unsigned int *)0x02620328
#define	MainPLLCTL1		*(unsigned int *)0x0262032C
#define	MainPLLCMD		*(unsigned int *)0x02310100
extern _c_int00(void);
#define CHIP_LEVEL_REG  0x02620000
#define KICK0_UNLOCK (0x83E70B13)
#define KICK1_UNLOCK (0x95A4F1E0)
#define KICK_LOCK    0
#define MAGIC_ADDR     0x87fffc
#define BOOT_MAGIC_ADDR(x)  (MAGIC_ADDR + (1<<28) + (x<<24))

#define DDR3_TEST_START_ADDRESS (0x80000000)
#define DDR3_TEST_END_ADDRESS   (DDR3_TEST_START_ADDRESS + 4*(0x100000))


#define DDR_TEMP_ADDRS_CODE 0xA0000000
#define FLASH_AUTO_START_BLOCK        (1)
#define FLASH_AUTO_BOOT_START_BLOCK       (0)
#define FLASH_AUTO_APP_START_BLOCK        (1)
#define FLASH_STARTUP_ADDRS_USER 0x70000000
#define APP_FLASH_LEN 0x300000

unsigned char g_dsp_num;			/*dsp芯片标识号*/
unsigned char g_mark_num;			/*模块槽位号*/

void delay_boot_ms(unsigned int N_ms)
{
	CSL_Uint64 counterVal1,counterVal2,result;

	counterVal1 = CSL_tscRead();

	while(1)
	{
		counterVal2 = CSL_tscRead();
		result = counterVal2 - counterVal1;
		if(result > (CSL_Uint64)N_ms * 1000000)
		{
			break;
		}
	}

}
void pll_wait(unsigned int i)
{
	unsigned int c = 0;
	for (c = 0; c < i; c++)
		asm("	nop 5");
}

static void MainPLL(unsigned int PLLM, unsigned int PLLD,  unsigned int POSTDIV2,unsigned int POSTDIV1)
{
	unsigned int temp = 0;
	unsigned int Main_PLLM, Main_PLLD, Main_POSTDIV2, Main_POSTDIV1;
	if (PLLM == 0)
		Main_PLLM = 1;
	else
		Main_PLLM = PLLM;
	if (PLLD == 0)
		Main_PLLD = 1;
	else
		Main_PLLD = PLLD;
	if (POSTDIV2 == 0)
		Main_POSTDIV2 = 1;
	else
		Main_POSTDIV2 = POSTDIV2;
	if (POSTDIV1 == 0)
		Main_POSTDIV1 = 1;
	else
		Main_POSTDIV1 = POSTDIV1;

	//CLC寄存器保护机制解锁
	/*对Kicker机制解锁时，需要通过对两个MMR对（KICK0和KICK1）寄存器写入特定数据（KICK0为0x83e70b13，KICK1为0x95a4f1e0）。
	 对任意一个Kicker MMRs写入其他数据将锁定kick机制，此时，CLC MMRs只可读。*/KICK0 = 0x83e70b13;
	KICK1 = 0x95a4f1e0;

	/*配置主PLL系统时钟*/MainPLLCMD = 0x21;  //将MainPLLC输出时钟模式切换为bypass模式
	//至少等待输入时钟CLK四个时钟周期，来保证系统时钟切换到BYPASS模式；
	pll_wait(20);

	//将MainPLLCMD中的PD写为1，关断PLL；
	MainPLLCMD = 0x2;

	/** MAINPLLCTL0 Register.*****************************************
	 * |  31...18  | 17...6   |5...0     |                         *
	 * |Reserved   |  PLLM    |PLLD      |**************************/MainPLLCTL0 =
			(Main_PLLM << 6) | Main_PLLD;	//设置MainPLL的输入控制信号

	/** MAINPLLCTL1 Register.******************************************
	 * |31...9  |  8  |   7      |   6   |   5...3 | 2...0  | 			*
	 * |Reserved|ENSAT| PLLOUTCTL| BYPASS| POSTDIV2|POSTDIV1|********/MainPLLCTL1 =
			(MainPLLCTL1 & 0xffffff40) | (Main_POSTDIV2 << 3) | Main_POSTDIV1;//主PLL输出时钟使能//配置PLL的POSTDIV2、POSTDIV1系数
	pll_wait(100);	//至少等待1us后将MainPLLCMD中的PD位写为0
	MainPLLCMD = 0x0;

	//查询MainPLLCMD中的LOCK是否为1，判断时钟是否稳定
	temp = 0x40 & MainPLLCMD;
	while (!temp) {
		temp = 0x40 & MainPLLCMD;
	}

	MainPLLCMD = 0x1;	//将MainPLL控制器时钟切换到PLL模式
	pll_wait(200);

}


unsigned int ddr3_memory_test (void)
{
    unsigned int index, value;



    /* Write a pattern */
    for (index = DDR3_TEST_START_ADDRESS; index < DDR3_TEST_END_ADDRESS; index += 4) {
        *(unsigned int*)index = index;
    }

    /* Read and check the pattern */
    for (index = DDR3_TEST_START_ADDRESS; index < DDR3_TEST_END_ADDRESS; index += 4) {

        value = *(unsigned int*)index;

        if (value  != index) {

            return 0xff;
        }
    }

    /* Write a pattern for complementary values */
    for (index = DDR3_TEST_START_ADDRESS; index < DDR3_TEST_END_ADDRESS; index += 4) {
        *(unsigned int*)index = ~index;
    }

    /* Read and check the pattern */
    for (index = DDR3_TEST_START_ADDRESS; index < DDR3_TEST_END_ADDRESS; index += 4) {

        value = *(unsigned int*)index;

        if (value  != ~index) {

            return 0xff;
        }
    }


    return 0;
}

#define PTCMD			*(unsigned int volatile *)(0x02350120)
#define PSC_MDCTL11		*(unsigned int volatile *)(0x02350a2c)//srio 0
#define PSC_MDCTL13		*(unsigned int volatile *)(0x02350a34)//srio 1
#define PSC_MDSTAT11	*(unsigned int volatile *)(0x0235082c)//srio 0
#define PSC_MDSTAT13	*(unsigned int volatile *)(0x02350834)//srio 1
unsigned int SrioClkClose(unsigned int SrioInstance){
	if(SrioInstance){
		PSC_MDCTL13 = 0x00000000;
		PTCMD = 0x1;
		while( (PSC_MDSTAT13 & 0x00000800)!=0x00000800 );
	}else{
		PSC_MDCTL11 = 0x00000000;
		PTCMD = 0x1;
		while( (PSC_MDSTAT11 & 0x00000800)!=0x00000800 );
	}
	return 0;
}

void usr_dev_init()
{
	char ch[20];

	PSC_Open_Clk("EMIF32");
	EMIF_init();
	S29GL64S_ID();

	bspSpiInit(0);
	bootActiveSet();
	/*引导APP之前，先上报BOOT的版本和编译时间*/
	setSoftwareInfo();
	softInfoToFpga();
	UART_Print("Version upload done\r\n");

	/*获取芯片标识和槽位*/
	UART_Config(BaudRate_Value);
	UART_Print("Uart config done\r\n");
	UART_Print("Get DSP slot ...");
	getSlot(&g_mark_num,&g_dsp_num);
	sprintf(ch,"done dsp %d,mark %d\r\n",g_dsp_num+1,g_mark_num);
	UART_Print(ch);

}


int flashRet = 0;
int flashAddrBlock = 0;
void Start_Boot()
{
	unsigned int DataHead[5];
	unsigned int i,j,length;

	unsigned int entryAddr;
	unsigned int size,startaddr;
	unsigned int *flash_ptr;
	unsigned int loopCnt=0;
	unsigned int address,blockNo;
	unsigned char temp;

//	unsigned int block_begin_num=0;
//	Uint16 pFlashBlockNo =0;
	void  (*exit)();

	/*核0 的时候处理措施*/
	if(DNUM == 0)
	{
		//核0将boot搬运到每个从核的boot运行段
		int i_move = 1;

		dspFlashAddrSwitch(0);
		for(i_move = 1;i_move<8;i_move++)
		{
			flash_ptr = (unsigned int *)0x70000000;
			entryAddr=*(flash_ptr++);
			while(1)
			{
				size=*(flash_ptr++); //先读该段尺寸
				loopCnt=size/4;/*size的单位为字节*/
				if(size%4!=0)
				{
					loopCnt++;
				}
				if(loopCnt == 0)
					break;
				startaddr=*(flash_ptr++);
				startaddr+=0x10000000 + i_move*0x1000000;

				for(i=0;i<loopCnt;i++)
				{
					do
					{
						*(volatile unsigned int *)(startaddr+4*i)=*(flash_ptr+i);
					}
					while(*( volatile unsigned int *)(startaddr+4*i)!=*(flash_ptr+i));/*判断一下*/
				}
				flash_ptr+=loopCnt;
			}

		}

#if 1
		temp = getBootMode();
		if(temp == BACKUP_OR_FAIL)/* 默认app启动失败，切换至备用app启动*/
		{
			blockNo = 31;
			UART_Print("switch DSP's flash to BLOCK 31 ....");
		}
		else/* 上电启动默认app */
		{
			blockNo = 1;
			UART_Print("switch DSP's flash to BLOCK 1 ....");
		}
#endif
		flashRet = dspFlashAddrSwitch(blockNo);  //默认从flash分区1启动应用软件
		if(flashRet == blockNo)  //切换FLASH地址块正确，切换到正确的地址
		{
			UART_Print("successful\r\n");
			//切换到目标flash地址,用于加载FLASH内数据并启动APP
			flash_ptr=(unsigned int *)FLASH_STARTUP_ADDRS_USER; //都是从每个分区的0x000000*2=0MB，默认从0MB地址启动APP
			for(i = 0;i < APP_FLASH_LEN;i+=4)
			{
				*(unsigned int *)(DDR_TEMP_ADDRS_CODE + i) = *(unsigned int *)(FLASH_STARTUP_ADDRS_USER + i);
			}
		}
		else  //切换地址失败，不启动任何APP
		{
			flashAddrBlock =-1;
			UART_Print("switch DSP's flash failed,please check fpga3\r\n");
		}
	}
	else
	{
		//flash_ptr = (unsigned int *)DDR_TEMP_ADDRS_CODE;
	}

	flash_ptr = (unsigned int *)DDR_TEMP_ADDRS_CODE;
	for(i=0;i<0x1000000;i+=4){
		*(unsigned int*)(0x80000000 + i) = 0;
	}

	entryAddr=*(flash_ptr++);

//	UART_Send(4,(char *)length);
//	UART_Send(4,(char *)DataHead);
	while(1)
	{
		size=*(flash_ptr++); //先读该段尺寸
		loopCnt=size/4;/*size的单位为字节*/
		if(size%4!=0)
		{
			loopCnt++;
		}
		if(loopCnt == 0)
			break;
		startaddr=*(flash_ptr++);

		for(i=0;i<loopCnt;i++)
		{
			do
			{
				*(volatile unsigned int *)(startaddr+4*i)=*(flash_ptr+i);
			}
			while(*( volatile unsigned int *)(startaddr+4*i)!=*(flash_ptr+i));/*判断一下*/
		}
		flash_ptr+=loopCnt;
	}

	UART_Print("App code copy done\r\n");

	if(DNUM == 0)
	{

//		CACHE_wbInvAllL1d(CACHE_WAIT);
//		CACHE_wbInvAllL2(CACHE_WAIT);

		*(unsigned int*)0xa0fffff0 = 0;

#if 1	//启动从核boot
		KICK0 = KICK0_UNLOCK;
		KICK1 = KICK1_UNLOCK;
#if 1
		*(uint32_t*) BOOT_MAGIC_ADDR(1) = (uint32_t)_c_int00;
//		UART_Print("-->(core 0):BOOT_MAGIC_ADDR 1...\r\n ");
		*((unsigned int *)(0x02620240 + 1*4))|=1;
	//	while(*(unsigned int*)0xa0fffff0 != 0xe33e);
	//	*(unsigned int*)0xa0fffff0 = 0;

		for(j=0;j<2000000;j++);
		*(uint32_t*) BOOT_MAGIC_ADDR(2) = (uint32_t)_c_int00;
//		UART_Print("-->(core 0)BOOT_MAGIC_ADDR 2...\r\n ");
		*((unsigned int *)(0x02620240 + 2*4))|=1;
	//	while(*(unsigned int*)0xa0fffff0 != 0xe33e);
	//	*(unsigned int*)0xa0fffff0 = 0;
		for(j=0;j<2000000;j++);
		*(uint32_t*) BOOT_MAGIC_ADDR(3) = (uint32_t)_c_int00;
//		UART_Print("-->(core 0)BOOT_MAGIC_ADDR 3...\r\n ");
		*((unsigned int *)(0x02620240 + 3*4))|=1;
	//	while(*(unsigned int*)0xa0fffff0 != 0xe33e);
	//	*(unsigned int*)0xa0fffff0 = 0;
		for(j=0;j<2000000;j++);
		*(uint32_t*) BOOT_MAGIC_ADDR(4) = (uint32_t)_c_int00;
//		UART_Print("-->(core 0)BOOT_MAGIC_ADDR 4...\r\n ");
		*((unsigned int *)(0x02620240 + 4*4))|=1;
	//	while(*(unsigned int*)0xa0fffff0 != 0xe33e);
	//	*(unsigned int*)0xa0fffff0 = 0;
		for(j=0;j<2000000;j++);
		*(uint32_t*) BOOT_MAGIC_ADDR(5) = (uint32_t)_c_int00;
//		UART_Print("-->(core 0)BOOT_MAGIC_ADDR 5...\r\n ");
		*((unsigned int *)(0x02620240 + 5*4))|=1;
	//	while(*(unsigned int*)0xa0fffff0 != 0xe33e);
	//	*(unsigned int*)0xa0fffff0 = 0;
		for(j=0;j<2000000;j++);
		*(uint32_t*) BOOT_MAGIC_ADDR(6) = (uint32_t)_c_int00;
//		UART_Print("-->(core 0)BOOT_MAGIC_ADDR 6...\r\n ");
		*((unsigned int *)(0x02620240 + 6*4))|=1;
	//	while(*(unsigned int*)0xa0fffff0 != 0xe33e);
	//	*(unsigned int*)0xa0fffff0 = 0;
		for(j=0;j<2000000;j++);
		*(uint32_t*) BOOT_MAGIC_ADDR(7) = (uint32_t)_c_int00;
//		UART_Print("-->(core 0)BOOT_MAGIC_ADDR 7...\r\n ");
		*((unsigned int *)(0x02620240 + 7*4))|=1;
	//	while(*(unsigned int*)0xa0fffff0 != 0xe33e);
	//	*(unsigned int*)0xa0fffff0 = 0;
		for(j=0;j<2000000;j++);
#endif
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
#else
		int i_boot;
		int j_boot;
		for(*(int *)0x80ffff00=1;*(int *)0x80ffff00<=7;*(int *)0x80ffff00 = *(int *)0x80ffff00 + 1 )
		{
			//*(int *)0x80ffff00 = i_boot + 1;
			KICK0 = KICK0_UNLOCK;
			KICK1 = KICK1_UNLOCK;
			*(uint32_t*) BOOT_MAGIC_ADDR(*(int *)0x80ffff00) = (uint32_t)_c_int00;
			*((unsigned int *)(0x02620240 + (*(int *)0x80ffff00)*4))|=1;
			for(j_boot=0;j_boot<10000000;j_boot++);
			KICK0 = KICK_LOCK;
			KICK1 = KICK_LOCK;
		}
#endif

	}

#if 0	//校验内存中数据
	checkSum = 0;
	for(i=0;i<0x1000000;i+=2){
			checkSum += *(unsigned short*)(0x80000000 + i);
	}
    core_NUM = DNUM;
	UART_Print("-->(core ");
	UART_Print_D((char *)&core_NUM,1);
	UART_Print("): CheckSUM: ");
	UART_Print_D((char *)&checkSum,4);
	UART_Print(" entryAddr: ");
	UART_Print_D((char *)&entryAddr,4);
	UART_Print("\r\n");
#endif
	*(unsigned int*)0xa0fffff0 = 0xe33e;
//	_mefence();

/*	unsigned int check_sum_ddr =0;
	for(j = 0;j < 0x1000000;j+= 4)
	{
		check_sum_ddr += *(unsigned int*)(0x80000000 + DNUM * 0x1000000 + j);
	}
	*(unsigned int*)(0xa3000000 + DNUM*4) = check_sum_ddr;
*/
	*(unsigned int*)(0xa3000020 + DNUM*4) = entryAddr;
	UART_Print("Jump to system\r\n");

	if(DNUM > 0)
	{
		while(*(unsigned int*)(0xa3000020)!= entryAddr);
		for(j=0;j<2000000*DNUM;j++);
	}

   *(unsigned int *)BOOT_MAGIC_ADDR(DNUM)=entryAddr;
	 exit = (void(*)())entryAddr;

	 (*exit)();//跳到应用程序入口

}

void set_MPAX()
{
	    CSL_XMC_XMPAXH 		mpaxh;		// 存储保护和地址扩展寄存器(H)
	    CSL_XMC_XMPAXL 		mpaxl;		// 存储保护和地址扩展寄存器(L)

	    int index = 5;
	    mpaxh.bAddr   = 0x80000;	// 基地址(匹配逻辑地址的高位地址)
	    mpaxh.segSize = 0x17;			// 16MB

	    // 设置XMPAXH寄存器. Writes:XMC_XMPAXH_SEGSZ,XMC_XMPAXH_BADDR.
	    CSL_XMC_setXMPAXH (index, &mpaxh);

	    // 设置该区段地址的访问权限
	    mpaxl.ux = 1;
	    mpaxl.uw = 1;
	    mpaxl.ur = 1;
	    mpaxl.sx = 1;
	    mpaxl.sw = 1;
	    mpaxl.sr = 1;
	    mpaxl.rAddr = 0x800000 + DNUM*0x1000;

	    // 设置XMPAXL寄存器.
	    CSL_XMC_setXMPAXL (index, &mpaxl);
}

/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
int main(void)
{
	unsigned int ret = 1;
	int jjj;

	//主频
	//MainPLL(32,1,1,1);    //FOUTPOSTDIV=800MHz
	if(DNUM == 0)    		//核0
	{
		cah1=0x0;
		cah2=0x0;

		CACHE_disableCaching(128);
		CACHE_disableCaching(129);
		CACHE_disableCaching(130);
		CACHE_disableCaching(131);
		CACHE_disableCaching(132);
		CACHE_disableCaching(133);
		CACHE_disableCaching(134);
		CACHE_disableCaching(135);
		CACHE_setL1PSize(CACHE_L1_0KCACHE);
		CACHE_setL1DSize(CACHE_L1_0KCACHE);
		CACHE_setL2Size(CACHE_0KCACHE);
		MainPLL(40,1,1,1);      //FOUTPOSTDIV=1GHz
		PSC_Close_Clk("SRIO0");
		PSC_Close_Clk("SRIO1");
		for(jjj=0;jjj<50000000;jjj++);
		CSL_tscEnable();
		usr_dev_init();
//		testfunc();
		delay_boot_ms(3000);

		/*
		 * @param1: input DDR CLK,         option: DDR_CLK_800MHZ,DDR_CLK_667MHZ, DDR_CLK_400MHZ
		 * @param2: input DRAM row number, option: ROW_16, ROW_15, ROW_14,ROW_13
		 * @param3: input DRAM width,      option: WIDTH_x16, WIDTH_x8
		 * @param4: input ecc type,        option: ECC_TYPE, NO_ECC_TYPE
		 */
		DDR_entry(DDR_CLK_667MHZ, ROW_16, WIDTH_x16, NO_ECC_TYPE,WIDTH_64BIT);
//		printf("\nDDR Initial Done!\n");

	#ifdef DDR3_ADDRESS_REMAP
		DDR_addrmap_demo();
	#endif

		/*
		 * @param1: memtest start address;
		 * @param2: memtest end address;
		 * 			NOTICE: unsigned int type;
		 * @param3: Don't modified
		 * @param4: Don't modified
		 */
		//DSP_memory_test(0x80000000, 0xFFFFFFFF, 1, "DDR3");
		UART_Print("DDR3 test begin\r\n");
		ret = ddr3_memory_test();
		if(ret == 0)
		{
			UART_Print("DDR3 test success\r\n");
		}
		else
		{
			UART_Print("DDR3 test failed\r\n");
		}
/*
		volatile unsigned int* lvMarPtr;
		unsigned int L_Value, H_Value;
		lvMarPtr = (volatile unsigned int*)0x08000020;
		H_Value = 0x80000017;
		L_Value = 0x8200003f;


		set_MPAX();
		*(unsigned int*)0xa0000000 = 0x2345678a;
			ret = *(unsigned int*)0x80000000;
			printf("mpax test 1 ret : 0x%x\n",ret);
			*(unsigned int*)0x80000004 = 0x56789123;
			ret = *(unsigned int*)0xa0000004;
			printf("mpax test 2 ret : 0x%x\n",ret);
*/

		Start_Boot();

		*(unsigned int*)0xc300000 = 0x12345678;
	}
	else
	{
		cah1=0x0;
		cah2=0x0;
		set_MPAX();

/*
		*(unsigned int*)0x81000000 = 0x2345678a;
		unsigned int ret_mpax = *(unsigned int*)0x80000000;
		printf("mpax test 1 ret : 0x%x\n",ret_mpax);
		*(unsigned int*)0x80000004 = 0x56789123;
		ret_mpax = *(unsigned int*)0x81000004;
		printf("mpax test 2 ret : 0x%x\n",ret_mpax);
*/

		Start_Boot();
	}
	while(1);

}



