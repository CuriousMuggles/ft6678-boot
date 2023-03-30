#include <ti/csl/csl_xmc.h>
#include <ti/csl/csl_xmcAux.h>
#include <stdio.h>

#define SEGSZ_4MB		0x15

void DDR_regwr_enable()
{
	CSL_XMC_XMPAXH  mpaxh;
	unsigned int index = 2;

	/*
	 * index 2 of mpaxl'value omitted;
	 * if using other index, mpaxl'value should be set
	 * mpaxh:0x08000014
	 * mpaxl:0x08000010
	 */
	CSL_XMC_getXMPAXH(index, &mpaxh);
	mpaxh.segSize = SEGSZ_4MB;
	CSL_XMC_setXMPAXH(index, &mpaxh);

}
