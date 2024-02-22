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
/*
 * function: Display result of write leveling
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
static void view_wrlvl_reg(WIDTH_BIT DIS_NUM) {
	unsigned int i;

	/* phy_wrlvl_hard0_delay_X */
	printf("phy_wrlvl_hard0_delay_X\n");
	for (i = 0; i < DIS_NUM; i++) {
		printf("PHY_%d, value = 0x%x\n", 47 + PHY_NUM_OFFSET * i,
				read_reg(DENALI_PHY_47 + PHY_SLICE_OFFSET * i) & MASK_10BIT);
	}

	/* phy_wrlvl_hard1_delay_X */
	printf("phy_wrlvl_hard1_delay_X\n");
	for (i = 0; i < DIS_NUM; i++) {
		printf("PHY_%d, value = 0x%x\n", 47 + PHY_NUM_OFFSET * i,
				(read_reg(DENALI_PHY_47 + PHY_SLICE_OFFSET * i) >> 16)
						& MASK_10BIT);
	}

	printf("phy_wrlvl_status_obs_X\n");
	for (i = 0; i < DIS_NUM; i++) {
		printf("PHY_%d, value = 0x%x\n", 48 + PHY_NUM_OFFSET * i,
				read_reg(DENALI_PHY_48 + PHY_SLICE_OFFSET * i) & MASK_15BIT);
	}

	/* phy_clk_wrdqs_slave_dly */
	printf("phy_wrlvl_clk_wrdqs_slave_delay_X\n");
	for (i = 0; i < DIS_NUM; i++) {
		printf("PHY_%d, value = 0x%x\n", 4 + PHY_NUM_OFFSET * i,
				(read_reg(DENALI_PHY_04 + PHY_SLICE_OFFSET * i) >> 16)
						& MASK_10BIT);
	}

}

/*
 * function: Display result of gate leveling
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
static void view_gtlvl_reg(WIDTH_BIT DIS_NUM) {
	unsigned int i;

	/* phy_gtlvl_hard0_delay_X */
	printf("phy_gtlvl_hard0_delay_X\n");
	for (i = 0; i < DIS_NUM; i++) {
		printf("PHY_%d, value = 0x%x\n", 50 + PHY_NUM_OFFSET * i,
				read_reg(DENALI_PHY_50 + PHY_SLICE_OFFSET * i) & MASK_10BIT);

	}

	/* phy_gtlvl_hard1_delay_X */
	printf("phy_gtlvl_hard1_delay_X\n");
	for (i = 0; i < DIS_NUM; i++) {
		printf("PHY_%d, value = 0x%x\n", 50 + PHY_NUM_OFFSET * i,
				(read_reg(DENALI_PHY_50 + PHY_SLICE_OFFSET * i) >> 16)
						& MASK_10BIT);
	}

	/* phy_gtlvl_status_obs_X */
	printf("phy_gtlvl_status_obs_X\n");
	for (i = 0; i < DIS_NUM; i++) {
		printf("PHY_%d, value = 0x%x\n", 51 + PHY_NUM_OFFSET * i,
				read_reg(DENALI_PHY_51 + PHY_SLICE_OFFSET * i) & MASK_10BIT);
	}

	/* phy_rddqs_gate_slave_X*/
	printf("phy_rddqs_gate_slave_X\n");
	for (i = 0; i < DIS_NUM; i++) {
		printf("PHY_%d, value = 0x%x\n", 28 + PHY_NUM_OFFSET * i,
				(read_reg(DENALI_PHY_28 + PHY_SLICE_OFFSET * i) >> 16)
						& MASK_10BIT);
	}

	/* phy_rddqs_latency_adjust_X*/
	printf("phy_rddqs_latency_adjust_X\n");
	for (i = 0; i < DIS_NUM; i++) {
		printf("PHY_%d, value = 0x%x\n", 29 + PHY_NUM_OFFSET * i,
				read_reg(DENALI_PHY_29 + PHY_SLICE_OFFSET * i) & MASK_4BIT);
	}
}

/*
 * function: Display result of read leveling
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
static void view_rdlvl_reg(WIDTH_BIT DIS_NUM) {
	unsigned int i;

	/* phy_rdlvl_rddqs_dq_le_dly_obsX */
	printf("phy_rdlvl_rddqs_dq_le_dly_obsX\n");
	for (i = 0; i < DIS_NUM; i++) {
		printf("PHY_%d, value = 0x%x\n", 51 + PHY_NUM_OFFSET * i,
				(read_reg(DENALI_PHY_51 + PHY_SLICE_OFFSET * i) >> 16)
						& MASK_10BIT);
	}

	/* phy_rdlvl_rddqs_dq_te_dly_obsX */
	printf("phy_rdlvl_rddqs_dq_te_dly_obsX\n");
	for (i = 0; i < DIS_NUM; i++) {
		printf("PHY_%d, value = 0x%x\n", 52 + PHY_NUM_OFFSET * i,
				read_reg(DENALI_PHY_52 + PHY_SLICE_OFFSET * i) & MASK_10BIT);
	}

	/* phy_rdlvl_status_obs_X */
	printf("phy_rdlvl_status_obs_X\n");
	for (i = 0; i < DIS_NUM; i++) {
		printf("PHY_%d, value = 0x%x\n", 53 + PHY_NUM_OFFSET * i,
				read_reg(DENALI_PHY_53 + PHY_SLICE_OFFSET * i));
	}
}

/*
 * function: calculate system'rank number
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
static char cal_rank_num(unsigned int addr) {
	unsigned int value = 0, tmp = 0;
	unsigned int j = 0;
	char rank_num = 0;

	value = read_reg(addr);
	tmp = (value >> DENALI_CTL_61_CS_MAP_OFFSET) & 0xf;

	for (j = 0; j < 4; j++) {
		if (tmp & 0x1) {
			rank_num++;
		}
		tmp >>= 1;
	}
	return rank_num;
}

/*
 * function: wait leveling finished
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
static char DDR_wait_hdlvl_cmplt(void) {

	check_reg(DENALI_CTL_68, DENALI_CTL_68_HW_LVL_CMP);

	/* ack */
	clr_set_reg(DENALI_CTL_69, (0x1 << DENALI_CTL_69_HW_LVL_ACT),
			(0x1 << DENALI_CTL_69_HW_LVL_ACT));

	return 0;
}

/*
 * function: Entry of DDR3 write leveling
 * input   : rank number
 * output  :
 * used    :
 * remark  :
 * version :
 */
static char DDR_hard_wrlvl(unsigned int cs_map,WIDTH_BIT DIS_NUM) {

	unsigned int j = 0;
	char wrlvl_cs_map = 0x0, ret = 0;
	char ecc_type = 0x0;


	/*  phy_wrlvl_dly_step_X, default: 0x3*/    //采样步长
	if ((ecc_type != ECC_TYPE)) {
		for (j = 0; j < DIS_NUM; j++)
			clr_set_reg(DENALI_PHY_34 + PHY_SLICE_OFFSET * j,
					(MASK_4BIT << PHY_WRLVL_DLY_STEP_OFFSET),
					(0xc << PHY_WRLVL_DLY_STEP_OFFSET));
	}

	if ((ecc_type == ECC_TYPE)) {
		for (j = 0; j < DIS_NUM; j++)
			clr_set_reg(DENALI_PHY_34 + PHY_SLICE_OFFSET * j,
					(MASK_4BIT << PHY_WRLVL_DLY_STEP_OFFSET),
					(0x8 << PHY_WRLVL_DLY_STEP_OFFSET));
	}

	/*  phy_wrlvl_capture_cnt_X, default: 0x8 */   //采样次数，一般改得少
	for (j = 0; j < DIS_NUM; j++)
		clr_set_reg(DENALI_PHY_34 + PHY_SLICE_OFFSET * j,
				(MASK_6BIT << PHY_WRLVL_CAP_CNT_X_OFFSET),
				(0x4 << PHY_WRLVL_CAP_CNT_X_OFFSET));

	//******************refer to NO.12************************************//
	clr_set_reg(DENALI_PHY_33 + PHY_SLICE_OFFSET * 0, MASK_10BIT, 0x140);     //强制让log延时结果与物理拓扑一致，强制为0
	clr_set_reg(DENALI_PHY_33 + PHY_SLICE_OFFSET * 1, MASK_10BIT, 0x140);
    /*phy_wrlvl_early_force_zero_X */
	clr_set_reg(DENALI_PHY_34 + PHY_SLICE_OFFSET * 0, 0x1, 0x1);
	clr_set_reg(DENALI_PHY_34 + PHY_SLICE_OFFSET * 1, 0x1, 0x1);
	/*********************************************************************/


	/* tdfi_wrlvl_en  TDFI_WRLVL_EN = 0x3 */
	/* tdfi_wrlvl_resp = 0 already cfg in regconfig.h */
	/* tdfi_wrlvl_ww = 20 already cfg in regconfig.h */
	/* wrlvl_en */
	clr_set_reg(DENALI_CTL_105, WRLVL_EN_MASK, WRLVL_EN);
	/* dfi_phy_wrlvl_mode*/
	clr_set_reg(DENALI_CTL_106, DFI_PHY_WRLVL_MODE_MASK, DFI_PHY_WRLVL_MODE);
	/* wrlvl_cs_map */
	//wrlvl_cs_map = *(char *)(G_CS_MAP + sram_offset);
	wrlvl_cs_map = 0x1;
	clr_set_reg(DENALI_CTL_108, (MASK_4BIT << 16), (wrlvl_cs_map << 16));
	/* wrlvl_cs */
	clr_set_reg(DENALI_CTL_104, WRLVL_CS_MASK, ((cs_map & 0x3) << 24));

	if ((ecc_type != ECC_TYPE))
		clr_set_reg(DENALI_CTL_123, 0xfff, 0xfff); /* tdfi_wrlvl_max */

	/* wrlvl_req */
	clr_set_reg(DENALI_CTL_104, WRLVL_REQ_MASK, WRLVL_REQ);

	ret = DDR_wait_hdlvl_cmplt();

	/* wrlvl_req  = 0 */
	clr_set_reg(DENALI_CTL_104, WRLVL_REQ_MASK, (0x0 << 16));
	/* wrlvl_en  = 0 */
	clr_set_reg(DENALI_CTL_105, WRLVL_EN_MASK, (0x0 << 16));

	return ret;

}

/*
 * function: Entry of DDR3 gate leveling
 * input   : rank number
 * output  :
 * used    :
 * remark  :
 * version :
 */
static char DDR_hard_gtlvl(unsigned int cs_map,WIDTH_BIT DIS_NUM) {
	unsigned int j;
	char ret = 0;
	char gtlvl_cs_map = 0;

	//unsigned int mcu_freq_value = pll_to_freq(mcu_pll_value);

	/*phy_gtlvl_back_step_X default: 0x140 */

	for (j = 0; j < DIS_NUM; j++) {
		clr_set_reg(DENALI_PHY_36 + PHY_SLICE_OFFSET * j, (MASK_10BIT << 8),
				(0x40 << 8));
	}

	/* PHY_GTLVL_LAT_ADJ_START */
	for (j = 0; j < DIS_NUM; j++) {
		clr_set_reg(DENALI_PHY_38 + PHY_SLICE_OFFSET * j, (MASK_4BIT), (0x2));               //gtlvl有问题,需要改0x2\0x3\0x4(默认)

	}

	/* tdfi_rdlvl_en; tdfi_rdlvl_resp; tdfi_rdlvl_rr */

	/* rdlvl_gate_en */
	clr_set_reg(DENALI_CTL_127, RDLVL_GATE_EN_MASK, RDLVL_GATE_EN);

	/* dfi_phy_rdlvl_gate_mode */
	clr_set_reg(DENALI_CTL_110, DFI_PHY_RDLVL_GATE_MODE_MASK,
			DFI_PHY_RDLVL_GATE_MODE);

	/* gtlvl_cs_map */
	gtlvl_cs_map = 0x1;

	clr_set_reg(DENALI_CTL_112, (MASK_4BIT << 24), (gtlvl_cs_map << 24));

	/* gtlvl_cs */
	clr_set_reg(DENALI_CTL_109, GTLVL_CS_MASK, ((cs_map & 0x3) << 16));

	/* gtlvl_gate_req */
	clr_set_reg(DENALI_CTL_109, RDLVL_GATE_REQ_MASK, RDLVL_GATE_REQ);

	ret = DDR_wait_hdlvl_cmplt();

#ifdef MCU_HARD_GTLVL_DEBUG
	view_gtlvl_reg();
#endif

	/* gtlvl_gate_req  = 0*/
	clr_set_reg(DENALI_CTL_109, RDLVL_GATE_REQ_MASK, 0x0);

	/* rdlvl_gate_en  = 0*/
	clr_set_reg(DENALI_CTL_127, RDLVL_GATE_EN_MASK, 0x0);

	return ret;

}

/*
 * function:  Entry of DDR3 read leveling
 * input   :  rank number
 * output  :
 * used    :
 * remark  :
 * version :
 */
static char DDR_hard_rdlvl(unsigned int cs_map,WIDTH_BIT DIS_NUM) {
	unsigned int j, ret;
	char rdlvl_cs_map = 0;
	/*
	 *  phy_rdlvl_capture_cnt_X     //采样次数
	 */
	for (j = 0; j < DIS_NUM; j++) {
		//rdlvl_cs_map = *(char *)(G_CS_MAP + sram_offset);
		rdlvl_cs_map = 0x1;
		//if((rdlvl_cs_map == 0xf) || (mcu_freq_value > 800) )
		clr_set_reg(DENALI_PHY_38 + PHY_SLICE_OFFSET * j, (MASK_6BIT << 8),
				(0x4 << 8));
		//else
		//clr_set_reg( DENALI_PHY_38 + PHY_SLICE_OFFSET * j, (MASK_6BIT << 8), (0x4 << 8));
	}

	/* phy_rdlvl_dly_step_X  */                             //  采样步长
	for (j = 0; j < DIS_NUM; j++)
		clr_set_reg(DENALI_PHY_38 + PHY_SLICE_OFFSET * j, (MASK_4BIT << 16),
				(0x4 << 16));                                     //rdlvl有问题，需要改0x4,一般改偶数

	/* phy_rdlvl_rddqs_dq_slv_dly_start_X */
	for (j = 0; j < DIS_NUM; j++)
		clr_set_reg(DENALI_PHY_39 + PHY_SLICE_OFFSET * j, (MASK_10BIT << 8),
				0x40 << 8);

	/* phy_rdlvl_data_mask_X */
	for (j = 0; j < DIS_NUM; j++)
		clr_set_reg(DENALI_PHY_40 + PHY_SLICE_OFFSET * j, (MASK_8BIT), 0x0);

	/* tdfi_rdlvl_en; tdfi_rdlvl_resp; tdfi_rdlvl_rr */
	/* rdlvl_en */
	clr_set_reg(DENALI_CTL_126, RDLVL_EN_MASK, RDLVL_EN);

	/* dfi_phy_rdlvl_mode */
	clr_set_reg(DENALI_CTL_110, DFI_PHY_RDLVL_MODE_MASK, DFI_PHY_RDLVL_MODE);

	/* rdlvl_cs_map */
	rdlvl_cs_map = 0x1;
	clr_set_reg(DENALI_CTL_112, (MASK_4BIT << 16), (rdlvl_cs_map << 16));

	/* rdlvl_cs */
	clr_set_reg(DENALI_CTL_109, RDLVL_CS_MASK, (cs_map & 0x3) << 16);

	/* rdlvl_req */
	clr_set_reg(DENALI_CTL_109, RDLVL_REQ_MASK, RDLVL_REQ);

	ret = DDR_wait_hdlvl_cmplt();

#ifdef MCU_HARD_RDLVL_DEBUG
	view_rdlvl_reg();
#endif

	/* rdlvl_req = 0 */
	clr_set_reg(DENALI_CTL_109, RDLVL_REQ_MASK, 0x0);
	/* rdlvl_en = 0*/
	clr_set_reg(DENALI_CTL_126, RDLVL_EN_MASK, 0x0);
	//mcu_print_string(str_hard_rdlvl_finished);

	return ret;

}

/*
 * function:   DDR3 leveling algorithm
 * input   :
 * output  :
 * used    :   called by mcu_start_work()
 * remark  :
 * version :
 */
void DDR_hard_lvling(WIDTH_BIT DIS_NUM) {
	unsigned int rank_num;
	int j, i;
	for (j = 0; j < DIS_NUM; j++) {
		/* phy_per_cs_training_mulicast_en_X = 0x0 */
		clr_set_reg(DENALI_PHY_12 + PHY_SLICE_OFFSET * j, (0x1 << 8),
				(0x0 << 8));
		/* phy_per_cs_training_en_X = 1 */
		clr_set_reg(DENALI_PHY_12 + PHY_SLICE_OFFSET * j, (0x1), (0x1));
	}

	/* caculate rank_num */
	rank_num = cal_rank_num(DENALI_CTL_61);
	/* rank_num */
	for (i = 0; i < rank_num; i++) {
		/* phy_per_cs_training_index_X : rank to be level */
		/* cs0 leveling */
		for (j = 0; j < DIS_NUM; j++)
			clr_set_reg(DENALI_PHY_12 + PHY_SLICE_OFFSET * j, (MASK_8BIT << 16),
					(i << 16));

		DDR_hard_wrlvl(i,DIS_NUM);
#ifdef DEBUG_LVL
		view_wrlvl_reg(DIS_NUM);
#endif
		printf("wrlvl finished \n");
		DDR_hard_gtlvl(i,DIS_NUM);
#ifdef DEBUG_LVL
		view_gtlvl_reg(DIS_NUM);
#endif
		printf("gtlvl finished \n");
		DDR_hard_rdlvl(i,DIS_NUM);
#ifdef DEBUG_LVL
		view_rdlvl_reg(DIS_NUM);
#endif
		printf("rdlvl finished \n");
	}

	/* per_cs_training_en_X = 0 */
	for (j = 0; j < DIS_NUM; j++)
		clr_set_reg(DENALI_PHY_12 + PHY_SLICE_OFFSET * j, (0x1), (0x0));

}
