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
/*
 * function:  memory test by IP
 * input   :
 * output  :
 * used    :  called by mcu_start_work()
 * remark  :  address bist range is from 0~(2.^end)
 * version :
 */
unsigned char DDR_start_address_bist(unsigned char end) {
	unsigned int value = 0;

	printf("Start address bist \n");

	//in_order_accept = 1'b1
	clr_set_reg(DENALI_CTL_62, (0x1 << 24), (0x1 << 24));
	//bist_startess = 0
	clr_set_reg(DENALI_CTL_46, MASK_32BIT, 0x0);
	clr_set_reg(DENALI_CTL_47, (0x1F << 0), (0x0));
	//addr_space
	clr_set_reg(DENALI_CTL_45, (0x3F), end);
	//bist_check = 1'b1
	clr_set_reg(DENALI_CTL_45, (0x1 << 16), (0x1 << 16));

	//bist_go = 1'b1
	clr_set_reg(DENALI_CTL_44, (0x1 << 16), (0x1 << 16));

	check_reg(DENALI_CTL_68, 5);

	/* ack */
	clr_set_reg(DENALI_CTL_69, 0x7FFFFF, 0x7FFFFF);

	//evaluate results
	value = read_reg(DENALI_CTL_44);
	if ((value & (0x1 << 25)) == 0x0)
		printf("address bist failed\n");
	else
		printf("address bist passed\n");

	//bist_check = 1'b0
	clr_set_reg(DENALI_CTL_45, (0x1 << 16), (0x0 << 16));
	//bist_go = 1'b0
	clr_set_reg(DENALI_CTL_44, (0x1 << 16), (0x0 << 16));
	//in_order_accept = 1'b0
	//clr_set_reg(DENALI_CTL_62, (0x1<<24), (0x0<<24) );

	return 0;
}

/*
 * function: memory test by IP
 * input   :
 * output  :
 * used    : called by mcu_start_work()
 * remark  :
 * version :
 */
unsigned int DDR_start_data_bist(unsigned int start, unsigned char end) {
	unsigned int i, value = 0;
	unsigned int fail_value = 0, exp_value = 0, xor_value = 0;
	unsigned int j = 0, k = 0, shift = 0;
	unsigned char error_slice_no = 0;

	unsigned int flag = 0;

	printf("Start data bist \n");

	//bist_check = 1'b0
	clr_set_reg(DENALI_CTL_45, (0x1 << 16), (0x0 << 16));
	//in_order_accept = 1'b1
	clr_set_reg(DENALI_CTL_62, (0x1 << 24), (0x1 << 24));
	//bist_startess = 0
	clr_set_reg(DENALI_CTL_46, MASK_32BIT, (start & MASK_32BIT));
	clr_set_reg(DENALI_CTL_47, (0x1F << 0), 0x0);
	//addr_space
	clr_set_reg(DENALI_CTL_45, (0x3F << 0), end);
	//bist_data_check = 1'b1
	clr_set_reg(DENALI_CTL_45, (0x1 << 8), (0x1 << 8));
	//bist_go = 1'b1
	clr_set_reg(DENALI_CTL_44, (0x1 << 16), (0x1 << 16));

	//RZQ/2 added by yz
	clr_set_reg(DENALI_CTL_35, 0x00000244, (0x1 << 6));
	check_reg(DENALI_CTL_68, 5);

	/* ack */
	clr_set_reg(DENALI_CTL_69, 0x7FFFFF, 0x7FFFFF);

	//evaluate results
	value = read_reg(DENALI_CTL_44);
	if ((value & (0x1 << 24)) == 0x0) {
		printf("data bist failed \n");

		//bist_fail
		printf("data bist failed address\n");
		printf("CTL_REG_92 = 0x%x\n", read_reg(DENALI_CTL_92));
		printf("CTL_REG_93 = 0x%x\n", read_reg(DENALI_CTL_93));

		//bist_exp_data
		printf("data bist expected data \n");
		for (i = 0; i < 9; i++)
			printf("CTL_REG_%d = 0x%x\n", (74 + i),
					read_reg(DENALI_CTL_74 + (i << 2)));

		//bist_fail_data
		printf("data bist failed data \n");
		for (i = 0; i < 9; i++)
			printf("CTL_REG_%d = 0x%x\n", (83 + i),
					read_reg(DENALI_CTL_83 + (i << 2)));

		for (i = 0; i < 9; i++) {
			exp_value = read_reg(DENALI_CTL_74 + (i << 2));
			fail_value = read_reg(DENALI_CTL_83 + (i << 2));
			if (exp_value != fail_value) {
				xor_value = exp_value ^ fail_value;
				for (j = 0; j < 4; j++) {
					shift = j << 3;
					value = (xor_value >> shift) & 0xff;
					if (value != 0x0) {
						error_slice_no = ((i % 3) * 3 + i + j) % 9;
						printf("error slice = %d\n", error_slice_no);
						for (k = 0; k < 8; k++) {
							if ((value & 0x1) == 1)
								printf("error bit = %d\n", k);
							value >>= 1;
						}
					}
				}
			}
		}

		flag = 0;
	} else {
		printf("data bist passed \n");
		flag = 1;
	}

	//bist_data_check = 1'b0
	clr_set_reg(DENALI_CTL_45, (0x1 << 8), (0x0 << 8));
	//bist_go = 1'b0
	clr_set_reg(DENALI_CTL_44, (0x1 << 16), (0x0 << 16));
	//in_order_accept = 1'b0
	//clr_set_reg( DENALI_CTL_62, (0x1<<24), (0x0<<24) );

	return flag;
}
