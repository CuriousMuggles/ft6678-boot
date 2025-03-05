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
#include <c6x.h>
#include <csl_cacheAux.h>
#include <ti/csl/csl_xmc.h>
#include <ti/csl/csl_xmcAux.h>
#include "driver/ddr/ddr_regdefine.h"
#include "driver/ddr/CORE_Memory_Test.h"
#include "driver/uart/uart.h"
#include "driver/flash/flash.h"
#include "driver/flash/flash_nor.h"
#include "driver/spi/spiCmdLib.h"
#include "bspInterface.h"

extern NOR_InfoObj gNorInfo;

#define	cah1			*(unsigned int *)0x01840000
#define	cah2			*(unsigned int *)0x01840040
#define	MainPLLCTL0		*(unsigned int *)0x02620328
#define	MainPLLCTL1		*(unsigned int *)0x0262032C
#define	MainPLLCMD		*(unsigned int *)0x02310100

#define DDR3_TEST_START_ADDRESS (0x80000000)
#define DDR3_TEST_END_ADDRESS   (DDR3_TEST_START_ADDRESS + 4*(0x100000))

unsigned char g_dsp_num;			/*dsp芯片标识号*/
unsigned char g_mark_num;			/*模块槽位号*/

/*******************************************************************************
*函数名：EMIF_init
*功    能： EMIF初始化
*参    数： 无
*******************************************************************************/
void EMIF_init(void)
{
#define RCSR	     *(unsigned int*)0x20C00000   //版本代码和状态寄存器
#define AWCCR	 	 *(unsigned int*)0x20C00004   //异步等待周期配置寄存器
#define A1CR	     *(unsigned int*)0x20C00010   //CE0空间配置寄存器
#define A2CR	     *(unsigned int*)0x20C00014   //CE0空间配置寄存器
#define A3CR	     *(unsigned int*)0x20C00018   //CE0空间配置寄存器
#define A4CR	     *(unsigned int*)0x20C0001c   //CE0空间配置寄存器
	//RCSR=RCSR | 0x40000000;//EMIF功能扩展，包括32位
	//AWCCR=0xc0000080;      //CE[3:0]为异步存储
	RCSR = 0x460400;
	AWCCR=0xc0000000;
	/*EW位置0，禁止扩展等待模式*/
	A1CR=0x3FFFFFFD;       //CE0配置为16位nor flash
	A2CR=0x3FFFFFFD;       //CE1配置为16位nor flash
	A3CR=0x3FFFFFFD;       //CE2配置为16位nor flash
	A4CR=0x3FFFFFFD;       //CE3配置为8位nor flash
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
static int ddrInit(void)
{
	int ret;
/*
		* @param1: input DDR CLK,         option: DDR_CLK_800MHZ,DDR_CLK_667MHZ, DDR_CLK_400MHZ
		* @param2: input DRAM row number, option: ROW_16, ROW_15, ROW_14,ROW_13
		* @param3: input DRAM width,      option: WIDTH_x16, WIDTH_x8
		* @param4: input ecc type,        option: ECC_TYPE, NO_ECC_TYPE
		*/
	DDR_entry(DDR_CLK_800MHZ, ROW_16, WIDTH_x16, NO_ECC_TYPE,WIDTH_64BIT);

	#ifdef DDR3_ADDRESS_REMAP
		DDR_addrmap_demo();
	#endif

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
}
void usr_dev_init()
{
	char ch[20];

	UART_Config(BaudRate_Value);
	UART_Print("\r\n===========FT6678 BOOT START===========\r\n");

	bspSpiInit(0);
	PSC_Open_Clk("EMIF32");
	EMIF_init();
	if (NOR_init(&gNorInfo))
	{
		bspPrintf("FLASH Initialization failed\r\n",0,1,2,3,4,5);
	}
	else{
		bspPrintf("FLASH Initialization success\r\n",0,1,2,3,4,5);
	}

	/*引导APP之前，先上报BOOT的版本和编译时间*/
	softInfoToFpga();
	bspPrintf("Version upload done\r\n",0,1,2,3,4,5);
	bspVersionInfoShow();

	/*获取芯片标识和槽位*/
	bspPrintf("Get DSP slot ...",0,1,2,3,4,5);
	getSlot(&g_mark_num,&g_dsp_num);
	bspPrintf("done dsp %d,mark %d\r\n",g_dsp_num+1,g_mark_num,2,3,4,5);

	ddrInit();
}


void Start_Boot()
{
	unsigned int i,j;
	unsigned int entryAddr;
	unsigned int * flash_ptr;
	int flashRet = 0;
	int i_move = 1;

	void  (*exit)();

	/*核0 的时候处理措施*/
	if(DNUM == 0)
	{
		//核0将boot搬运到每个从核的boot运行段
		dspFlashAddrSwitch(0);
		for(i_move = 1;i_move<8;i_move++){
			entryAddr = reload_dat_boot(FLASH_STARTUP_ADDRS_BOOT,i_move);
		}

		flashRet = dspFlashAddrSwitch(1);  //默认从flash分区1启动应用软件
		if(flashRet == RET_SUCCESS)  //切换FLASH地址块正确，切换到正确的地址
		{
			UART_Print("switch DSP's flash successful\r\n");
			//切换到目标flash地址,用于加载FLASH内数据并启动APP
			flash_ptr=(unsigned int *)FLASH_STARTUP_ADDRS_APP;
			for(i = 0;i < APP_FLASH_LEN;i+=4){
				*(unsigned int *)(DDR_TEMP_ADDRS_CODE + i) = *(unsigned int *)(FLASH_STARTUP_ADDRS_APP + i);
			}
		}
		else  //切换地址失败，不启动任何APP
		{
			UART_Print("switch DSP's flash failed,please check fmql\r\n");
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
		bspPrintf("%d ",DNUM,2,3,4,5,6);
		if(DNUM == 7){
			UART_Print("\r\n");
		}
	}

	/* 所有核跳转到应用入口 */
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

		/* 配置主频 */
		// MainPLL(32,1,1,1);    //FOUTPOSTDIV=800MHz
		MainPLL(40,1,1,1);      //FOUTPOSTDIV=1GHz

		/* 关闭SRIO时钟 */
		PSC_Close_Clk("SRIO0");
		PSC_Close_Clk("SRIO1");
		CSL_tscEnable();

		/* 驱动初始化 */
		usr_dev_init();

		Start_Boot();

	}
	else
	{
		cah1=0x0;
		cah2=0x0;

		set_MPAX();

		Start_Boot();
	}
	while(1);

}



