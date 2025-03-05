#include <stdio.h>
#include <csl_types.h>
#include <csl_tsc.h>

void usleep(unsigned int n_us)
{
	CSL_Uint64 counterVal1,counterVal2,result;

	counterVal1 = CSL_tscRead();
	while(1){
		counterVal2 = CSL_tscRead();
		result = counterVal2 - counterVal1;
		if(result > (CSL_Uint64)n_us * 1000){
			return;
		}
	}
}
void msleep(unsigned int n_ms)
{
	CSL_Uint64 counterVal1,counterVal2,result;

	counterVal1 = CSL_tscRead();
	while(1){
		counterVal2 = CSL_tscRead();
		result = counterVal2 - counterVal1;
		if(result > (CSL_Uint64)n_ms * 1000000){
			return;
		}
	}
}
void sleep(unsigned int n_s)
{
	CSL_Uint64 counterVal1,counterVal2,result;

	counterVal1 = CSL_tscRead();
	while(1){
		counterVal2 = CSL_tscRead();
		result = counterVal2 - counterVal1;
		if(result > (CSL_Uint64)n_s * 1000000000){
			return;
		}
	}
}

void pll_wait(unsigned int i)
{
	unsigned int c = 0;
	for (c = 0; c < i; c++)
		asm("	nop 5");
}
