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
#include "DDR_Regdefine.h"
#include "driver/psc/psc.h"
#include <string.h>
#include "driver/uart/uart.h"

#define PSC_MDCTL2   	0x02350a08
#define PSC_MDSTAT2  	0x02350808

//DDR系统时钟
#define	DDRPLLCTL0		*(volatile unsigned int *)0x02620330
#define	DDRPLLCTL1		*(volatile unsigned int *)0x02620334
#define	DDRPLLCMD		*(volatile unsigned int *)0x02310160
#define	DDRPLLC_En		*(volatile unsigned int *)0x02310164
/***************PSC 寄存器*******************************************************/
#define PSC_BASE            0X02350000
#define PTCMD_REG           0X02350120
#define PTSTAT_REG	        0X02350128
#define DDR3_offset         0xa08
/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
#ifdef DEBUG_DDR
void display_ctrl_reg() {
	int i;
	for (i = 0; i < DENALI_CTL_NUM; i++) {
		printf("ctrl reg = %d, value = 0x%x\n", i,
				*(unsigned int *) (DDR_BASE_ADDR + (i << 2)));
	}

}
#endif

#ifdef DEBUG_DDR_PHY
void display_phy_reg() {
	int i;
	for (i = 0; i < DENALI_PHY_NUM; i++) {
		printf("phy reg = %d, value = 0x%x\n", i,
				*(unsigned int *)(DENALI_PHY_ADDR + (i << 2)));
	}

}
#endif

/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
unsigned int read_reg(unsigned int addr) {
	unsigned int readvalue = *(unsigned int *) addr;
	return readvalue;
}

/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
void clr_set_reg(unsigned int offset, unsigned int mask_value,
		unsigned int value) {
	unsigned int temp = 0;

	temp = *(unsigned int *) (offset);
	temp &= ~(mask_value);
	temp |= value;
	*(unsigned int *) (offset) = temp;
}

/*
 * function:  check dfi_init_complete status
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
void check_reg(unsigned int addr, unsigned int offset) {
	unsigned int *paddr = (unsigned int *) addr;

	while (((*paddr) & (0x1 << offset)) == 0) {
	}
}

/*
 * function: configuration of DDR3 CTL/PHY registers
 * input   : frequency of DDR3, support DDR_CLK_800MHZ/DDR_CLK_667MHZ/DDR_CLK_400MHZ
 * output  :
 * used    :  called by mcu_start_work()
 * remark  :
 *             row_num   : configurable, support ROW_16, ROW_15, ROW_14
 *             dram_width: configurable, support WIDTH_x16, WIDTH_x8
 * version :
 */
static void DDR_reg_init(DDR_CLK_t ddr_freq, DDR_ROW_NUM_t row_num_input,
		DDR_DRAM_WIDTH_t dram_width_input, unsigned char ecc,WIDTH_BIT DIS_NUM) {
	int i;
	unsigned int dram_width = (dram_width_input == WIDTH_x16) ? 16 : 8;
	unsigned int row_num;
	unsigned long long sdram_capacity;
	unsigned int freq = (ddr_freq == DDR_CLK_800MHZ) ? 800 :
						(ddr_freq == DDR_CLK_667MHZ) ? 667 : 400;

	switch(row_num_input)
	{
	case ROW_15:
			row_num = 15;
			break;
	case ROW_14:
			row_num = 14;
			break;
	case ROW_13:
			row_num = 13;
			break;
	default:
			row_num = 16;
			break;
	}

	sdram_capacity = (unsigned long long) (1<< (row_num + 10 + 3)) * dram_width;

	for (i = 0; i < DENALI_CTL_NUM; i++) {
		*(unsigned int *) (DDR_BASE_ADDR + (i << 2)) =
				(unsigned int) MCU_CTRL_VALUE_800M[i];
	}

	for (i = 576; i < DENALI_PHY_NUM; i++) {
		*(unsigned int *) (DENALI_PHY_ADDR + (i << 2)) =
				(unsigned int) MCU_PHY_VALUE_800M[i];
	}

	for (i = 0; i < 575; i++) {
		*(unsigned int *) (DENALI_PHY_ADDR + (i << 2)) =
				(unsigned int) MCU_PHY_VALUE_800M[i];
	}

//  2 rank  2个片选
//     clr_set_reg(DENALI_CTL_61, MASK_4BIT << 16, 0x3 << 16);   //cs map
//	   clr_set_reg(DENALI_CTL_37, MASK_32BIT, (0xC70 | 0x46 << 16));
//	   clr_set_reg(DENALI_CTL_38, MASK_16BIT,0x3<<3);

//   3 rank  3个片选
//	 clr_set_reg(DENALI_CTL_61,MASK_4BIT<<16,0x7<<16);
#if 0
	//tref
	if (ddr_freq == DDR_CLK_800MHZ)
		clr_set_reg(DENALI_CTL_24, MASK_14BIT << 16, 0x1858 << 16);
	else if (ddr_freq == DDR_CLK_667MHZ)
		clr_set_reg(DENALI_CTL_24, MASK_14BIT << 16, 0x1452 << 16);
	else
		clr_set_reg(DENALI_CTL_24, MASK_14BIT << 16, 0xc28 << 16);
#endif


	clr_set_reg(DENALI_CTL_24, MASK_14BIT << 16, ((freq * TREF_TIME)/10) << 16);


	//row_num
	if (row_num_input == ROW_16)
		clr_set_reg(DENALI_CTL_56, MASK_3BIT << 24, 0);
	else if (row_num_input == ROW_15)
		clr_set_reg(DENALI_CTL_56, MASK_3BIT << 24, 1 << 24);
	else if (row_num_input == ROW_13)
		clr_set_reg(DENALI_CTL_56, MASK_3BIT << 24, 3 << 24);
	else
		//row_num_input == ROW_14
		clr_set_reg(DENALI_CTL_56, MASK_3BIT << 24, 2 << 24);

	//trfc
	if (sdram_capacity == CAPACITY_512Mb)           //90ns
		clr_set_reg(DENALI_CTL_24, MASK_10BIT, (freq * TRFC_512Mb) / 1000 + 1);
	else if (sdram_capacity == CAPACITY_1Gb)       //110ns
		clr_set_reg(DENALI_CTL_24, MASK_10BIT, (freq * TRFC_1Gb) / 1000 + 1);
	else if (sdram_capacity == CAPACITY_2Gb)       //160ns
		clr_set_reg(DENALI_CTL_24, MASK_10BIT, (freq * TRFC_2Gb) / 1000 + 1);
	else if (sdram_capacity == CAPACITY_4Gb)       //300ns
		clr_set_reg(DENALI_CTL_24, MASK_10BIT, (freq * TRFC_4Gb) / 1000 + 1);
	else
		//8Gb  //350ns
		clr_set_reg(DENALI_CTL_24, MASK_10BIT, (freq * TRFC_8Gb) / 1000 + 1);

	if(ecc == NO_ECC_TYPE)
	{
		clr_set_reg(DENALI_CTL_50, MASK_8BIT, 0xff);
		clr_set_reg(DENALI_CTL_52, MASK_8BIT<<8, 0xff<<8);

		/*
		clr_set_reg(DENALI_CTL_107, 0x3ffff<<8, 0x20100<<8);
		clr_set_reg(DENALI_CTL_126, 0x3ffff, 0x20100);
		*/
		//don't know reason
		clr_set_reg(DENALI_CTL_107, 0x3ffff<<8, 0x30000<<8);
	}

    if(ecc == ECC_TYPE)
    {
    	clr_set_reg(ECCCFG, 0x3, 0x3);  //ECC Int en and ECC en
    	clr_set_reg(ECCCLR, 0x3, 0x3);  //ECC Int clear and ECC Log clear

    }

	 /**************Half Datapath Mode*****************/  //2021.5.1
    if(DIS_NUM == WIDTH_32BIT)
     {
    	 clr_set_reg(DENALI_CTL_107, 0x3FFFF<<8,  	0x3FC00<<8);
    	 clr_set_reg(DENALI_CTL_126, 0x3FFFF,    	0x3FC00);

    	 clr_set_reg(DENALI_CTL_136, 0x1<<8, 0x1<<8);

     	}

	//start
	clr_set_reg(DENALI_CTL_00, 0x1, 0x1);

#ifdef DEBUG_DDR
	display_ctrl_reg();
#endif

#ifdef DEBUG_DDR_PHY
	display_phy_reg();
#endif

	check_reg(DENALI_CTL_68, 3);
	//ack
	clr_set_reg(DENALI_CTL_69, 0x1fffff, 0x1fffff);

}

/*
 * function:
 * input   : frequency of DDR3, support DDR_CLK_800MHZ/DDR_CLK_667MHZ/DDR_CLK_400MHZ
 * output  :
 * used    : called by DDR_Init()
 * remark  :
 * version :
 */
static int DDR_start_work(DDR_CLK_t ddr_freq, DDR_ROW_NUM_t row_num,
		DDR_DRAM_WIDTH_t dram_width, unsigned char ecc,WIDTH_BIT DIS_NUM) {

	DDR_reg_init(ddr_freq, row_num, dram_width, ecc,DIS_NUM);

	DDR_hard_lvling(DIS_NUM);

#ifdef DDR_BIST_CTL
	//dimm capacity = (2.^end)
	//bist capacity = 1MB;
	DDR_start_address_bist(BIST_1MB_SPACE);
	DDR_start_data_bist(0x0, BIST_1MB_SPACE);
#endif

#ifdef DDR_MARGIN_CTL
	DDR_margin_test(DIS_NUM);
#else
	margin_usr();
#endif

	return 0;
}

/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
static void DDRpll(DDR_CLK_t ddr_freq) {
	unsigned int temp;

	//将DDRPLLC输出时钟模式切换为bypass模式
	DDRPLLC_En = 0x1;
	//等待输入时钟CLK四个时钟周期，来保证DDRCLK时钟切换到BYPASS模式；
	pll_wait(20);
	//DDRPLLCMD中的PD写为1，关断PLL
	DDRPLLCMD = 0x1;

	//设置DDRPLL的输入控制信号
	if(ddr_freq == DDR_CLK_200MHZ){
		DDRPLLCTL0 = 0x00000802;
		DDRPLLCTL1 = 0x00000002;
	}
	else if (ddr_freq == DDR_CLK_400MHZ) {
		DDRPLLCTL0 = 0x00000802;
		DDRPLLCTL1 = 0x00000001;
	} else if (ddr_freq == DDR_CLK_667MHZ) {
		DDRPLLCTL0 = 0x00001401;
		DDRPLLCTL1 = 0x00000003;
	} else { //800MHZ
		DDRPLLCTL0 = 0x00000801;
		DDRPLLCTL1 = 0x00000001;
	}

	//至少等待1us后将DDRPLLCMD中的PD写为0
	pll_wait(100);
	DDRPLLCMD = 0;
	pll_wait(100);
	//查询DDRPLLCMD中的LOCK是否为1，判断时钟是否稳定
	temp = 0x2 & DDRPLLCMD;
	while (!temp) {
		temp = 0x2 & DDRPLLCMD;
		printfk("DDRPLLCMD is 0x%08x\r\n",DDRPLLCMD);
	}
	//将DDRPLLC_En中的DDRPLLC_BYPASS写为0，将DDRPLL控制器时钟切换到PLL模式bypass，0为125M或333M时钟；*******
	DDRPLLC_En = 0;	//???
}

/*
 * function:  Entry function of M6678'DDR3 bring up
 * input   :  frequency of DDR3, support DDR_CLK_800MHZ/DDR_CLK_667MHZ/DDR_CLK_400MHZ
 * output  :
 * used    :  called by main()
 * remark  :
 * version :
 */
int DDR_entry(DDR_CLK_t ddr_freq, DDR_ROW_NUM_t row_num,
		DDR_DRAM_WIDTH_t dram_width, unsigned char ecc,WIDTH_BIT DIS_NUM) {

	//set PLL_DDR
	DDRpll(ddr_freq);
	//clock open
	PSC_Open_Clk("DDR3");
	//enable ddr register' w/r
	DDR_regwr_enable();

	DDR_start_work(ddr_freq, row_num, dram_width, ecc, DIS_NUM);

	return 0;

}

