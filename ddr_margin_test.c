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
#include "ddr_regdefine.h"
#include "ddr_margin_data.h"
unsigned int right_data1 = 0x0;

/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
void mcu_set_wrdqs_slave_delay(unsigned char slice_no, unsigned int dly_value) {
	clr_set_reg(DENALI_PHY_04 + PHY_SLICE_OFFSET * slice_no, (MASK_10BIT << 16),
			(dly_value << 16));

	/* afresh = 1*/
	clr_set_reg(DENALI_CTL_23, (0x1), (0x1));
	/* ctrlupd_req = 1*/
	clr_set_reg(DENALI_CTL_63, (0x1 << 16), (0x1 << 16));
}

/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */

unsigned char data_bist_for_sw_lvl(unsigned char slice_num,
		unsigned long long start, unsigned char bit_num) {
	unsigned int value48, value49, value50, value51, value52;
	unsigned int value = 0;
	unsigned char flag = 0;

	/* 0xFF : flag of all bits in slice  */
	if (bit_num == 0xFF) {
		switch (slice_num) {
		case 0:
			value48 = ~0xFF;
			value50 = ~(0xFF << 8);
			value49 = ~0x0;
			value51 = ~0x0;
			value52 = ~0x0;
			break;
		case 1:
			value48 = ~(0xFF << 8);
			value50 = ~(0xFF << 16);
			value49 = ~0x0;
			value51 = ~0x0;
			value52 = ~0x0;
			break;
		case 2:
			value48 = ~(0xFF << 16);
			value50 = ~(0xFF << 24);
			value49 = ~0x0;
			value51 = ~0x0;
			value52 = ~0x0;
			break;
		case 3:
			value48 = ~(0xFF << 24);
			value49 = ~0x0;
			value50 = ~0x0;
			value51 = ~0xFF;
			value52 = ~0x0;
			break;
		case 4:
			value49 = ~0xFF;
			value51 = ~(0xFF << 8);
			value48 = ~0x0;
			value50 = ~0x0;
			value52 = ~0x0;
			break;
		case 5:
			value49 = ~(0xFF << 8);
			value51 = ~(0xFF << 16);
			value48 = ~0x0;
			value50 = ~0x0;
			value52 = ~0x0;
			break;
		case 6:
			value49 = ~(0xFF << 16);
			value51 = ~(0xFF << 24);
			value48 = ~0x0;
			value50 = ~0x0;
			value52 = ~0x0;
			break;
		case 7:
			value48 = ~0x0;
			value49 = ~(0xFF << 24);
			value50 = ~0x0;
			value51 = ~0x0;
			value52 = ~0xFF;
			break;
		case 8:
			value48 = ~0x0;
			value49 = ~0x0;
			value50 = ~0xFF;
			value51 = ~0x0;
			value52 = ~(0xFF << 8);
			break;
		default:
			break;
		}
	} else { /* for bist_sw_rdlvl , just mask one bit */
		switch (slice_num) {
		case 0:
			value48 = ~(0x1 << bit_num);
			value50 = ~(0x1 << (8 + bit_num));
			value49 = ~0x0;
			value51 = ~0x0;
			value52 = ~0x0;
			break;
		case 1:
			value48 = ~(0x1 << (8 + bit_num));
			value50 = ~(0x1 << (16 + bit_num));
			value49 = ~0x0;
			value51 = ~0x0;
			value52 = ~0x0;
			break;
		case 2:
			value48 = ~(0x1 << (16 + bit_num));
			value50 = ~(0x1 << (24 + bit_num));
			value49 = ~0x0;
			value51 = ~0x0;
			value52 = ~0x0;
			break;
		case 3:
			value48 = ~(0x1 << (24 + bit_num));
			value49 = ~0x0;
			value50 = ~0x0;
			value51 = ~(0x1 << bit_num);
			value52 = ~0x0;
			break;
		case 4:
			value49 = ~(0x1 << bit_num);
			value51 = ~(0x1 << (8 + bit_num));
			value48 = ~0x0;
			value50 = ~0x0;
			value52 = ~0x0;
			break;
		case 5:
			value49 = ~(0x1 << (8 + bit_num));
			value51 = ~(0x1 << (16 + bit_num));
			value48 = ~0x0;
			value50 = ~0x0;
			value52 = ~0x0;
			break;
		case 6:
			value49 = ~(0x1 << (16 + bit_num));
			value51 = ~(0x1 << (24 + bit_num));
			value48 = ~0x0;
			value50 = ~0x0;
			value52 = ~0x0;
			break;
		case 7:
			value48 = ~0x0;
			value49 = ~(0x1 << (24 + bit_num));
			value50 = ~0x0;
			value51 = ~0x0;
			value52 = ~(0x1 << bit_num);
			break;
		case 8:
			value48 = ~0x0;
			value49 = ~0x0;
			value50 = ~(0x1 << bit_num);
			value51 = ~0x0;
			value52 = ~(0x1 << (8 + bit_num));
			break;
		default:
			break;
		}
	}

	//in_order_accept = 1'b1
	clr_set_reg(DENALI_CTL_62, (0x1 << 24), (0x1 << 24));

	//bist_startess = 0
	clr_set_reg(DENALI_CTL_46, MASK_32BIT, (start & MASK_32BIT));
	clr_set_reg(DENALI_CTL_47, MASK_5BIT, ((start >> 32) & MASK_5BIT));

	//addr_space
	clr_set_reg(DENALI_CTL_45, (MASK_6BIT << 0), BIST_FOR_SW_LVL_END);

	//bist_data_check = 1'b1
	clr_set_reg(DENALI_CTL_45, (0x1 << 8), (0x1 << 8));

	//bist_data_mask
	clr_set_reg(DENALI_CTL_48, MASK_32BIT, value48);
	clr_set_reg(DENALI_CTL_49, MASK_32BIT, value49);
	clr_set_reg(DENALI_CTL_50, MASK_32BIT, value50);
	clr_set_reg(DENALI_CTL_51, MASK_32BIT, value51);
	clr_set_reg(DENALI_CTL_52, MASK_16BIT, value52);

	//bist_go = 1'b1
	clr_set_reg(DENALI_CTL_44, (0x1 << 16), (0x1 << 16));

	check_reg(DENALI_CTL_68, 5);

	/* ack */
	clr_set_reg(DENALI_CTL_69, 0x7FFFFF, 0x7FFFFF);

	//evaluate results
	value = read_reg(DENALI_CTL_44);
	if ((value & (0x1 << 24)) == 0x0)
		flag = 0;
	else
		flag = 1;

	//bist_data_check = 1'b0
	clr_set_reg(DENALI_CTL_45, (0x1 << 8), (0x0 << 8));

	//bist_go = 1'b0
	clr_set_reg(DENALI_CTL_44, (0x1 << 16), (0x0 << 16));
	//in_order_accept = 1'b0
	//clr_set_reg(DENALI_CTL_62, (0x1<<24), (0x0<<24) );

	return flag;
}

/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */

void mcu_cfg_wrdqs_default_delay(WIDTH_BIT DIS_NUM) {

	unsigned int j, value = 0;

	for (j = 0; j < DIS_NUM; j++) {
		value = *(unsigned short *) ((unsigned long) MARGIN_CLK_DQS_ADDR
				+ (j << 2));
		clr_set_reg(DENALI_PHY_04 + PHY_SLICE_OFFSET * j, (MASK_10BIT << 16),
				(value << 16));
	}
	/* afresh = 1*/
	clr_set_reg(DENALI_CTL_23, (0x1), (0x1));
	/* ctrlupd_req = 1*/
	clr_set_reg(DENALI_CTL_63, (0x1 << 16), (0x1 << 16));
}

/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
void mcu_cfg_wrdq_default_delay(WIDTH_BIT DIS_NUM) {
	int j;

	for (j = 0; j < DIS_NUM; j++) {
		clr_set_reg(DENALI_PHY_00 + PHY_SLICE_OFFSET * j, (MASK_10BIT << 0),
				(0x280 << 0));
		clr_set_reg(DENALI_PHY_00 + PHY_SLICE_OFFSET * j, (MASK_10BIT << 16),
				(0x280 << 16));

		clr_set_reg(DENALI_PHY_01 + PHY_SLICE_OFFSET * j, (MASK_10BIT << 0),
				(0x280 << 0));
		clr_set_reg(DENALI_PHY_01 + PHY_SLICE_OFFSET * j, (MASK_10BIT << 16),
				(0x280 << 16));

		clr_set_reg(DENALI_PHY_02 + PHY_SLICE_OFFSET * j, (MASK_10BIT << 0),
				(0x280 << 0));
		clr_set_reg(DENALI_PHY_02 + PHY_SLICE_OFFSET * j, (MASK_10BIT << 16),
				(0x280 << 16));

		clr_set_reg(DENALI_PHY_03 + PHY_SLICE_OFFSET * j, (MASK_10BIT << 0),
				(0x280 << 0));
		clr_set_reg(DENALI_PHY_03 + PHY_SLICE_OFFSET * j, (MASK_10BIT << 16),
				(0x280 << 16));
	}

	/* afresh = 1*/
	clr_set_reg(DENALI_CTL_23, (0x1), (0x1));

	/* ctrlupd_req = 1*/
	clr_set_reg(DENALI_CTL_63, (0x1 << 16), (0x1 << 16));
}

/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
void mcu_cfg_rddqs_default_delay(WIDTH_BIT DIS_NUM) {
	unsigned int i, j;
	unsigned short value;

	//rising
	for (j = 0; j < DIS_NUM; j++) {
		for (i = 0; i < 8; i++) {
			value =
					*(unsigned short *) ((unsigned long) MARGIN_RDDQS_RISING_ADDR
							+ (i << 1) + (j << 4));
			clr_set_reg(DENALI_PHY_19 + (i << 2) + PHY_SLICE_OFFSET * j,
					(MASK_10BIT << 16), (value << 16));
		}
	}

	//falling
	for (j = 0; j < DIS_NUM; j++) {
		for (i = 0; i < 8; i++) {
			value =
					*(unsigned short *) ((unsigned long) MARGIN_RDDQS_FALLING_ADDR
							+ (i << 1) + (j << 4));
			clr_set_reg(DENALI_PHY_20 + (i << 2) + PHY_SLICE_OFFSET * j,
					MASK_10BIT, value);
		}
	}

	/* afresh = 1*/
	clr_set_reg(DENALI_CTL_23, (0x1), (0x1));

	/* ctrlupd_req = 1*/
	clr_set_reg(DENALI_CTL_63, (0x1 << 16), (0x1 << 16));
}

/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
void mcu_set_bit_wrdq_slave_delay(unsigned char slice_no, unsigned char bit_no,
		unsigned int dly_value) {
	switch (bit_no) {
	case 0:
		clr_set_reg(DENALI_PHY_00 + PHY_SLICE_OFFSET * slice_no,
				(MASK_10BIT << 0), (dly_value << 0));
		break;
	case 1:
		clr_set_reg(DENALI_PHY_00 + PHY_SLICE_OFFSET * slice_no,
				(MASK_10BIT << 16), (dly_value << 16));
		break;
	case 2:
		clr_set_reg(DENALI_PHY_01 + PHY_SLICE_OFFSET * slice_no,
				(MASK_10BIT << 0), (dly_value << 0));
		break;
	case 3:
		clr_set_reg(DENALI_PHY_01 + PHY_SLICE_OFFSET * slice_no,
				(MASK_10BIT << 16), (dly_value << 16));
		break;
	case 4:
		clr_set_reg(DENALI_PHY_02 + PHY_SLICE_OFFSET * slice_no,
				(MASK_10BIT << 0), (dly_value << 0));
		break;
	case 5:
		clr_set_reg(DENALI_PHY_02 + PHY_SLICE_OFFSET * slice_no,
				(MASK_10BIT << 16), (dly_value << 16));
		break;
	case 6:
		clr_set_reg(DENALI_PHY_03 + PHY_SLICE_OFFSET * slice_no,
				(MASK_10BIT << 0), (dly_value << 0));
		break;
	case 7:
		clr_set_reg(DENALI_PHY_03 + PHY_SLICE_OFFSET * slice_no,
				(MASK_10BIT << 16), (dly_value << 16));
		break;
	default:
		break;
	}

	/* afresh = 1*/
	clr_set_reg(DENALI_CTL_23, (0x1), (0x1));

	/* ctrlupd_req = 1*/
	clr_set_reg(DENALI_CTL_63, (0x1 << 16), (0x1 << 16));
}

/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
void mcu_set_rise_dqZ_bit_delay(unsigned char slice_no, unsigned char bit_no,
		unsigned int value) {

	clr_set_reg(DENALI_PHY_19 + (bit_no << 2) + PHY_SLICE_OFFSET * slice_no,
			(MASK_10BIT << 16), (value << 16));

	/* afresh = 1*/
	clr_set_reg(DENALI_CTL_23, (0x1), (0x1));

	/* ctrlupd_req = 1*/
	clr_set_reg(DENALI_CTL_63, (0x1 << 16), (0x1 << 16));
}

/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
void mcu_set_fall_dqZ_bit_delay(unsigned char slice_no, unsigned char bit_no,
		unsigned int value) {

	clr_set_reg(DENALI_PHY_20 + (bit_no << 2) + PHY_SLICE_OFFSET * slice_no,
			MASK_10BIT, value);

	/* afresh = 1*/
	clr_set_reg(DENALI_CTL_23, (0x1), (0x1));

	/* ctrlupd_req = 1*/
	clr_set_reg(DENALI_CTL_63, (0x1 << 16), (0x1 << 16));
}

/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
void store_wrlvl_origin_value(WIDTH_BIT DIS_NUM) {
	unsigned int value = 0;
	unsigned int j = 0;

	for (j = 0; j < DIS_NUM; j++) {
		value = read_reg(DENALI_PHY_04 + PHY_SLICE_OFFSET * j);
		value >>= 16;
		value &= 0x3ff;
		*(unsigned short *) ((unsigned long) MARGIN_CLK_DQS_ADDR + (j << 2)) =
				(unsigned short) value;
	}

}

/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
void store_rdlvl_origin_value(WIDTH_BIT DIS_NUM) {
	unsigned int value = 0;
	unsigned int j = 0, m = 0;

	//rise
	for (j = 0; j < DIS_NUM; j++) {
		for (m = 0; m < 8; m++) {
			value = read_reg(DENALI_PHY_19 + (m << 2) + PHY_SLICE_OFFSET * j);
			value >>= 16;
			value &= 0x3ff;
			*(unsigned short *) ((unsigned long) MARGIN_RDDQS_RISING_ADDR
					+ (m << 1) + (j << 4)) = (unsigned short) value;
		}
	}

	//fall
	for (j = 0; j < DIS_NUM; j++) {
		for (m = 0; m < 8; m++) {
			value = read_reg(DENALI_PHY_20 + (m << 2) + PHY_SLICE_OFFSET * j);
			value &= 0x3ff;
			*(unsigned short *) ((unsigned int) MARGIN_RDDQS_FALLING_ADDR
					+ (m << 1) + (j << 4)) = (unsigned short) value;
		}
	}
}

void clear_margin_save_space() {
	int i;
	unsigned int *paddr = (unsigned int*) DDR_MARGIN_SAVE_BASE_ADDR;
	for (i = 0; i < DDR_MARGIN_SIZE; i += 4) {
		*paddr++ = 0x0;
	}

}

void clear_bist_save_space() {
	int i;
	unsigned int *paddr = (unsigned int*) MARGIN_BIST_BASE_ADDR;
	for (i = 0; i < MARGIN_BIST_SIZE; i += 4) {
		*paddr++ = 0x0;
	}

}

void fill_right_value1() {
	unsigned int aa = 0;
	for (aa = 0; aa < 8; aa++) {
		right_data1 = *(unsigned int *) (0x0c300000 + aa * 4);
		clr_set_reg(DENALI_PHY_04 + PHY_SLICE_OFFSET * aa, (MASK_10BIT << 16),
				(right_data1 << 16));
		//printf("right_data1 is 0x%x\n", right_data1);
	}
}
void fill_right_value2() {
	unsigned int sil = 0;
	unsigned int bit = 0;
	for (sil = 0; sil < 8; sil++) {
		for (bit = 0; bit < 8; bit++) {
			right_data1 =
					*(unsigned int *) (0x0c300300 + bit * 4 + sil * 4 * 8);
			mcu_set_bit_wrdq_slave_delay(sil, bit, right_data1);
			//printf("right_data1 is 0x%x\n", right_data1);
		}
	}
}
void fill_right_value3() {
	unsigned int sil = 0;
	unsigned int bit = 0;
	for (sil = 0; sil < 8; sil++) {
		for (bit = 0; bit < 8; bit++) {
			right_data1 =
					*(unsigned int *) (0x0c300600 + bit * 4 + sil * 4 * 8);
			clr_set_reg(DENALI_PHY_19 + (bit << 2) + PHY_SLICE_OFFSET * sil,
					(MASK_10BIT << 16), (right_data1 << 16));
			//printf("right_data1 is 0x%x\n", right_data1);
		}
	}
}
void fill_right_value4() {
	unsigned int sil = 0;
	unsigned int bit = 0;
	for (sil = 0; sil < 8; sil++) {
		for (bit = 0; bit < 8; bit++) {
			right_data1 =
					*(unsigned int *) (0x0c300900 + bit * 4 + sil * 4 * 8);
			clr_set_reg(DENALI_PHY_20 + (bit << 2) + PHY_SLICE_OFFSET * sil,
					MASK_10BIT, right_data1);
			//printf("right_data1 is 0x%x\n", right_data1);
		}
	}
}

void fill_cmp() {
	/* afresh = 1*/
	clr_set_reg(DENALI_CTL_23, (0x1), (0x1));

	/* ctrlupd_req = 1*/
	clr_set_reg(DENALI_CTL_63, (0x1 << 16), (0x1 << 16));
}

void margin_usr()
{
	memcpy((int*)0x0c300000, ddr_margin_data, 0xa00);
	fill_right_value1();
	fill_right_value2();
	fill_right_value3();
	fill_right_value4();
	fill_cmp();

}
/*
 * function:  mcu bist margin 
 * input   :
 * output  :
 * used    : called by mcu_start_work()
 * remark  :
 * version :
 */
#ifdef DDR_MARGIN_CTL
void DDR_margin_test(WIDTH_BIT DIS_NUM) {
	unsigned char repeat_cnt = 0, slice_cnt = 0, bit_cnt = 0;
	unsigned char flag = 0;
	unsigned int m = 0;
	unsigned short dly_value = 0, tmp_value = 0;
	unsigned short max_dly = 0x0, min_dly = 0x400;

	unsigned short print_value = 0x0;
	unsigned short print_value_v = 0x0;
	unsigned int right_data = 0x0;

	clear_margin_save_space();
	store_wrlvl_origin_value(DIS_NUM);
	store_rdlvl_origin_value(DIS_NUM);

	printf("\t\t********** Margin test begin **********\n");
#ifdef CLK_WRDQS_DELAY_CTL 
	clear_bist_save_space();
	flag = DDR_start_data_bist(0x0, BIST_1MB_SPACE);
	if (!flag) {
		printf("margin test failed\n");
		//while(1);
	}

	printf("clk_wrdqs_window\n");
	for (repeat_cnt = 0; repeat_cnt < 3; repeat_cnt++) {
		for (slice_cnt = 0; slice_cnt < DIS_NUM; slice_cnt++) {
			min_dly = 0x200;
			max_dly = 0x0;
			for (dly_value = 0x0; dly_value < 0x200; dly_value += 0x8) {
				mcu_set_wrdqs_slave_delay(slice_cnt, dly_value);
				flag = data_bist_for_sw_lvl(slice_cnt, 0x0, 0xff);
				if (flag == 0x1) {
					if (min_dly == 0x200)
						min_dly = dly_value;
					else
						max_dly = dly_value;
				}
			}

			tmp_value =
					*(unsigned short *) ((unsigned long) MARGIN_BIST_CLK_WRDQS_WINDOW
							+ (slice_cnt << 2));
			if ((tmp_value == 0x0) || (tmp_value < min_dly))
				*(unsigned short *) ((unsigned long) MARGIN_BIST_CLK_WRDQS_WINDOW
						+ (slice_cnt << 2)) = min_dly;

			tmp_value =
					*(unsigned short *) ((unsigned long) MARGIN_BIST_CLK_WRDQS_WINDOW
							+ (slice_cnt << 2) + 2);
			if ((tmp_value == 0x0) || (tmp_value > max_dly))
				*(unsigned short *) ((unsigned long) MARGIN_BIST_CLK_WRDQS_WINDOW
						+ (slice_cnt << 2) + 2) = max_dly;

			mcu_cfg_wrdqs_default_delay(DIS_NUM);
		}
	}

	printf("\n");
	m = 0;
	for (slice_cnt = 0; slice_cnt < DIS_NUM; slice_cnt++) {

		print_value =
				*(unsigned short *) ((unsigned long) MARGIN_BIST_CLK_WRDQS_WINDOW
						+ (slice_cnt << 2));
		print_value_v =
				*(unsigned short *) ((unsigned long) MARGIN_BIST_CLK_WRDQS_WINDOW
						+ (slice_cnt << 2) + 2);

		printf("slice num = %d, margin 0x%x ~~ 0x%x\n", slice_cnt, print_value,
				print_value_v);
		right_data = (print_value + print_value_v) / 2;
		printf("right_data is 0x%x\n", right_data);
		*(unsigned int *) (ck_dqs_addr + slice_cnt * 4) = right_data;
		//right_data1=*(unsigned int *)(0x0c300000+slice_cnt*4);
		//printf("right_data1 is 0x%x\n", right_data1);
		if (print_value_v < print_value) {
			m++;
		}
	}
	if (m > 0)
		printf("warning:Unknow error,will restart!");

	printf("</window>\n");
#endif

#ifdef CLK_WRDQZ_DELAY_CTL 
	clear_bist_save_space();
	flag = DDR_start_data_bist(0x0, BIST_1MB_SPACE);
	if (!flag) {
		printf("margin test failed\n");
		while (1)
			;
	}

	//disaplay
	printf("clk_wrdqz_window \n");
	for (slice_cnt = 0; slice_cnt < DIS_NUM; slice_cnt++) {
		for (repeat_cnt = 0; repeat_cnt < 3; repeat_cnt++) {
			for (bit_cnt = 0; bit_cnt < 8; bit_cnt++) {
				min_dly = 0x300;
				max_dly = 0x200;
				for (dly_value = 0x200; dly_value < 0x300; dly_value += 0x8) {
					mcu_set_bit_wrdq_slave_delay(slice_cnt, bit_cnt, dly_value);
					flag = data_bist_for_sw_lvl(slice_cnt, 0x0, bit_cnt);
					if (flag == 0x1) {
						if (min_dly == 0x300)
							min_dly = dly_value;
						else
							max_dly = dly_value;
					}
				}

				tmp_value =
						*(unsigned short *) ((unsigned long) MARGIN_BIST_CLK_WRDQZ_WINDOW
								+ (bit_cnt << 2));
				if ((tmp_value == 0x0) || (tmp_value < min_dly))
					*(unsigned short *) ((unsigned long) MARGIN_BIST_CLK_WRDQZ_WINDOW
							+ (bit_cnt << 2)) = min_dly;

				tmp_value =
						*(unsigned short *) ((unsigned long) MARGIN_BIST_CLK_WRDQZ_WINDOW
								+ (bit_cnt << 2) + 2);
				if ((tmp_value == 0x0) || (tmp_value > max_dly))
					*(unsigned short *) ((unsigned long) MARGIN_BIST_CLK_WRDQZ_WINDOW
							+ (bit_cnt << 2) + 2) = max_dly;

				mcu_cfg_wrdq_default_delay(DIS_NUM);
			}
		}
	}
	//disaplay
	printf("\n");
	for (slice_cnt = 0; slice_cnt < DIS_NUM; slice_cnt++) {
		m = 0;
		printf("slice num = %d\n", slice_cnt);
		for (bit_cnt = 0; bit_cnt < 8; bit_cnt++) {
			print_value =
					*(unsigned short *) ((unsigned long) MARGIN_BIST_CLK_WRDQZ_WINDOW
							+ (slice_cnt << 2));
			print_value_v =
					*(unsigned short *) ((unsigned long) MARGIN_BIST_CLK_WRDQZ_WINDOW
							+ (slice_cnt << 2) + 2);

			printf("bit num = %d, margin 0x%x ~~ 0x%x\n", bit_cnt, print_value,
					print_value_v);
			right_data = (print_value + print_value_v) / 2;
			printf("right_data is 0x%x\n", right_data);
			*(unsigned int *) (ck_dqs_addr + 0x300 + bit_cnt * 4
					+ slice_cnt * 4 * 8) = right_data;
			if (print_value_v < print_value) {
				m++;
			}

		}
		if (m > 0)
			printf("warning:Unknow error,will restart!\n");
	}

#endif

#ifdef RDDQS_DQZ_RISE_DELAY_CTL
	clear_bist_save_space();
	flag = DDR_start_data_bist(0x0, BIST_1MB_SPACE);
	if (!flag) {
		printf("margin test failed\n");
		while (1)
			;
	}

	//display
	printf("rising rddqs window\n");
	for (slice_cnt = 0; slice_cnt < DIS_NUM; slice_cnt++) {
		for (repeat_cnt = 0; repeat_cnt < 3; repeat_cnt++) {
			for (bit_cnt = 0; bit_cnt < 8; bit_cnt++) {
				min_dly = 0x100;
				max_dly = 0x30;
				for (dly_value = 0x30; dly_value < 0x100; dly_value += 0x8) {
					mcu_set_rise_dqZ_bit_delay(slice_cnt, bit_cnt, dly_value);
					flag = data_bist_for_sw_lvl(slice_cnt, 0x0, bit_cnt);
					if (flag == 0x1) {
						if (min_dly == 0x100)
							min_dly = dly_value;
						else
							max_dly = dly_value;
					}
				}
				tmp_value =
						*(unsigned short *) ((unsigned long) MARGIN_BIST_RISING_RDDQS_WINDOW
								+ (bit_cnt << 2));
				if ((tmp_value == 0x0) || (tmp_value < min_dly)) {
					*(unsigned short *) ((unsigned long) MARGIN_BIST_RISING_RDDQS_WINDOW
							+ (bit_cnt << 2)) = min_dly;
				}
				tmp_value =
						*(unsigned short *) ((unsigned long) MARGIN_BIST_RISING_RDDQS_WINDOW
								+ (bit_cnt << 2) + 2);
				if ((tmp_value == 0x0) || (tmp_value > max_dly)) {
					*(unsigned short *) ((unsigned long) MARGIN_BIST_RISING_RDDQS_WINDOW
							+ (bit_cnt << 2) + 2) = max_dly;
				}

				mcu_cfg_rddqs_default_delay(DIS_NUM);
			}
		}
	}

	//display
	printf("\n");
	for (slice_cnt = 0; slice_cnt < DIS_NUM; slice_cnt++) {
		m = 0;
		printf("slice num = %d\n", slice_cnt);
		for (bit_cnt = 0; bit_cnt < 8; bit_cnt++) {
			print_value =
					*(unsigned short *) ((unsigned long) MARGIN_BIST_RISING_RDDQS_WINDOW
							+ (slice_cnt << 2));
			print_value_v =
					*(unsigned short *) ((unsigned long) MARGIN_BIST_RISING_RDDQS_WINDOW
							+ (slice_cnt << 2) + 2);

			printf("bit num = %d, margin 0x%x ~~ 0x%x\n", bit_cnt, print_value,
					print_value_v);
			right_data = (print_value + print_value_v) / 2;
			printf("right_data is 0x%x\n", right_data);
			*(unsigned int *) (ck_dqs_addr + 0x600 + bit_cnt * 4
					+ slice_cnt * 4 * 8) = right_data;
			if (print_value_v < print_value)
				m++;
			if (m > 0)
				printf("warning:Unknow error,will restart!");
		}
	}

#endif

#ifdef RDDQS_DQZ_FALL_DELAY_CTL
	clear_bist_save_space();
	flag = DDR_start_data_bist(0x0, BIST_1MB_SPACE);
	if (!flag) {
		printf("margin test failed\n");
		while (1)
			;
	}
	//display
	printf("falling rddqs window\n");
	for (slice_cnt = 0; slice_cnt < DIS_NUM; slice_cnt++) {
		for (repeat_cnt = 0; repeat_cnt < 3; repeat_cnt++) {
			for (bit_cnt = 0; bit_cnt < 8; bit_cnt++) {
				min_dly = 0x120;
				max_dly = 0x30;
				for (dly_value = 0x30; dly_value < 0x120; dly_value += 0x8) {
					mcu_set_fall_dqZ_bit_delay(slice_cnt, bit_cnt, dly_value);
					flag = data_bist_for_sw_lvl(slice_cnt, 0x0, bit_cnt);
					if (flag == 0x1) {
						if (min_dly == 0x120)
							min_dly = dly_value;
						else
							max_dly = dly_value;
					}
				}

				tmp_value =
						*(unsigned short *) ((unsigned long) MARGIN_BIST_FALLING_RDDQS_WINDOW
								+ (bit_cnt << 2));
				if ((tmp_value == 0x0) || (tmp_value < min_dly)) {
					*(unsigned short *) ((unsigned long) MARGIN_BIST_FALLING_RDDQS_WINDOW
							+ (bit_cnt << 2)) = min_dly;
				}

				tmp_value =
						*(unsigned short *) ((unsigned long) MARGIN_BIST_FALLING_RDDQS_WINDOW
								+ (bit_cnt << 2) + 2);
				if ((tmp_value == 0x0) || (tmp_value > max_dly)) {
					*(unsigned short *) ((unsigned long) MARGIN_BIST_FALLING_RDDQS_WINDOW
							+ (bit_cnt << 2) + 2) = max_dly;
				}

				mcu_cfg_rddqs_default_delay(DIS_NUM);
			}
		}
	}
	//display

	printf("\n");

	for (slice_cnt = 0; slice_cnt < DIS_NUM; slice_cnt++) {
		m = 0;
		printf("slice num = %d\n", slice_cnt);
		for (bit_cnt = 0; bit_cnt < 8; bit_cnt++) {
			print_value =
					*(unsigned short *) ((unsigned long) MARGIN_BIST_FALLING_RDDQS_WINDOW
							+ (slice_cnt << 2));
			print_value_v =
					*(unsigned short *) ((unsigned long) MARGIN_BIST_FALLING_RDDQS_WINDOW
							+ (slice_cnt << 2) + 2);

			printf("bit num = %d, margin 0x%x ~~ 0x%x\n", bit_cnt, print_value,
					print_value_v);
			right_data = (print_value + print_value_v) / 2;
			printf("right_data is 0x%x\n", right_data);
			*(unsigned int *) (ck_dqs_addr + 0x900 + bit_cnt * 4
					+ slice_cnt * 4 * 8) = right_data;
			if (print_value_v < print_value)
				m++;
		}
		if (m > 0)
			printf("warning:Unknow error,will restart!");
	}

#endif
	fill_right_value1();
	fill_right_value2();
	fill_right_value3();
	fill_right_value4();
	fill_cmp();
	printf("DDR margin test finished \n");
	//while(1);
}
#endif
