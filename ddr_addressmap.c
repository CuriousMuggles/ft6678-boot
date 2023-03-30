/*
 *============================================================================
 *
 * PROJECT:  DDR3 for M6678, MT41K256M16HA-125T
 * DATE   :
 * AUTHOR :  YHFT
 * REMARKS:  Demo of DDR3 addressmap configuration
 * HISTORY:
 *
 *
 *    SPDX-License-Identifier: GPL-2.0+
 *
 *============================================================================
 */
#include "DDR_Regdefine.h"
#include <ti/csl/csl_xmc.h>
#include <ti/csl/csl_xmcAux.h>

#define SEGSZ_1GB		0x1D
#define SEGSZ_2GB		0x1E
/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
void DDR_addrmap_High_2G(void) {

	CSL_XMC_XMPAXH  mpaxh;
	CSL_XMC_XMPAXL  mpaxl;
	unsigned int index = 3;

	*(unsigned int *) (0x0bc00948) = 0x800000bf + 0x8000000;
	*(unsigned int *) (0x0bc0094c) = 0x80000000 + 0x1E;

	//mpaxl: 0x08000018;
	CSL_XMC_getXMPAXL(index, &mpaxl);
	mpaxl.rAddr = 0x880000;
	mpaxl.sr = 1;
	mpaxl.sw = 1;
	mpaxl.sx = 1;
	mpaxl.ur = 1;
	mpaxl.uw = 1;
	mpaxl.ux = 1;
	CSL_XMC_setXMPAXL(index, &mpaxl);

	//mpaxh: 0x0800001c;
	CSL_XMC_getXMPAXH(index, &mpaxh);
	mpaxh.bAddr = 0x80000;
	mpaxh.segSize = SEGSZ_2GB;
	CSL_XMC_setXMPAXH(index, &mpaxh);

}

/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
void DDR_addrmap_Low_2G(void) {

	CSL_XMC_XMPAXH  mpaxh;
	CSL_XMC_XMPAXL  mpaxl;
	unsigned int index = 3;

	*(unsigned int *) (0x0bc00948) = 0x800000bf;
	*(unsigned int *) (0x0bc0094c) = 0x80000000 + 0x1E;
	//mpaxl: 0x08000018;
	CSL_XMC_getXMPAXL(index, &mpaxl);
	mpaxl.rAddr = 0x800000;
	mpaxl.sr = 1;
	mpaxl.sw = 1;
	mpaxl.sx = 1;
	mpaxl.ur = 1;
	mpaxl.uw = 1;
	mpaxl.ux = 1;
	CSL_XMC_setXMPAXL(index, &mpaxl);

	//mpaxh: 0x0800001c;
	CSL_XMC_getXMPAXH(index, &mpaxh);
	mpaxh.bAddr = 0x80000;
	mpaxh.segSize = SEGSZ_2GB;
	CSL_XMC_setXMPAXH(index, &mpaxh);


}

/*
 * function:
 * input   :
 * output  :
 * used    :
 * remark  :
 * version :
 */
void DDR_addrmap_demo(void) {

	CSL_XMC_XMPAXH  mpaxh;
	CSL_XMC_XMPAXL  mpaxl;
	unsigned int index = 3;

	//MPAX: addrmap=4GB-----SEGSZ:0x1F
	//      addrmap=2GB-----SEGSZ:0x1E
	//      addrmap=1GB-----SEGSZ:0x1D
	//      addrmap=512MB---SEGSZ:0x1C
	//      addrmap=256MB---SEGSZ:0x1B
	const unsigned int SEGSZ = 0x1d;
	const unsigned int SIZE_1GB = 0x4000000;
	const unsigned int SIZE_BASE = 0x0;
	unsigned int addrmap = SIZE_1GB;

	//step 1: addressmap to low 1GB;
	*(unsigned int *) (0x0bc00948) = 0x800000bf + SIZE_BASE;
	*(unsigned int *) (0x0bc0094c) = 0x80000000 + SEGSZ;
	//mpaxl: 0x08000018;
	CSL_XMC_getXMPAXL(index, &mpaxl);
	mpaxl.rAddr = 0x800000;
	mpaxl.sr = 1;
	mpaxl.sw = 1;
	mpaxl.sx = 1;
	mpaxl.ur = 1;
	mpaxl.uw = 1;
	mpaxl.ux = 1;
	CSL_XMC_setXMPAXL(index, &mpaxl);

	//mpaxh: 0x0800001c;
	CSL_XMC_getXMPAXH(index, &mpaxh);
	mpaxh.bAddr = 0x80000;
	mpaxh.segSize = SEGSZ_1GB;
	CSL_XMC_setXMPAXH(index, &mpaxh);

	//step 2: write low test addr 0x80000000 with data 0x1122334455667788;
	*(unsigned long long *) 0x80000000 = 0x1122334455667788;

	//step 3: addressmap to high 1GB;
	*(unsigned int *) (0x0bc00948) = 0x800000bf + addrmap;
	*(unsigned int *) (0x0bc0094c) = 0x80000000 + SEGSZ;
	//mpaxl: 0x08000018;
	CSL_XMC_getXMPAXL(index, &mpaxl);
	mpaxl.rAddr = 0x840000;
	mpaxl.sr = 1;
	mpaxl.sw = 1;
	mpaxl.sx = 1;
	mpaxl.ur = 1;
	mpaxl.uw = 1;
	mpaxl.ux = 1;
	CSL_XMC_setXMPAXL(index, &mpaxl);

	//mpaxh: 0x0800001c;
	CSL_XMC_getXMPAXH(index, &mpaxh);
	mpaxh.bAddr = 0x80000;
	mpaxh.segSize = SEGSZ_1GB;
	CSL_XMC_setXMPAXH(index, &mpaxh);


	//step4 : read low addr 0x80000000
	printf("1. val = 0x%llx\n", *(unsigned long long *) 0x80000000);

	//step5: write low test addr 0x80000000 with data 0x8877665544332211;
	*(unsigned long long *) 0x80000000 = 0x8877665544332211;

	//step6: addressmap to low 1GB;
	*(unsigned int *) (0x0bc00948) = 0x800000bf + SIZE_BASE;
	*(unsigned int *) (0x0bc0094c) = 0x80000000 + SEGSZ;
	//mpaxl: 0x08000018;
	CSL_XMC_getXMPAXL(index, &mpaxl);
	mpaxl.rAddr = 0x800000;
	mpaxl.sr = 1;
	mpaxl.sw = 1;
	mpaxl.sx = 1;
	mpaxl.ur = 1;
	mpaxl.uw = 1;
	mpaxl.ux = 1;
	CSL_XMC_setXMPAXL(index, &mpaxl);

	//mpaxh: 0x0800001c;
	CSL_XMC_getXMPAXH(index, &mpaxh);
	mpaxh.bAddr = 0x80000;
	mpaxh.segSize = SEGSZ_1GB;
	CSL_XMC_setXMPAXH(index, &mpaxh);

	//step7:read low addr 0x80000000
	printf("2. val = 0x%llx\n", *(unsigned long long *) 0x80000000);
}


