/******************************************************************************
 * Copyright (c) 2011 Texas Instruments Incorporated - http://www.ti.com
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *****************************************************************************/
/*
 *  Linker command file
 *
 */

-c
-heap  0x4000
-stack 0x4000

/* Memory Map 1 - the default */
MEMORY
{
	MSMC_BOOTASM(RWX):org=0x0800000,len = 0x100
    MSMC_BOOT (RWX) : org = 0x0802100, len = 0x1df00/*0xfe00*/
	MSMC_BOOT_CINT(RWX):org=0x0800100,len=0x2000
}

SECTIONS
{
	.boot		>		MSMC_BOOTASM

    .csl_vect   >       MSMC_BOOT
    .cppi       >       MSMC_BOOT
    .linkram    >       MSMC_BOOT
    .mac_buffer >       MSMC_BOOT
    platform_lib >      MSMC_BOOT
    .text       >       MSMC_BOOT
    .text:_c_int00       >       MSMC_BOOT_CINT
    GROUP (NEAR_DP)
    {
    .neardata
    .rodata 
    .bss
    } load > MSMC_BOOT
    .stack      >       MSMC_BOOT
    .cinit      >       MSMC_BOOT_CINT
    .cio        >       MSMC_BOOT
    .const      >       MSMC_BOOT
    .data       >       MSMC_BOOT
    .switch     >       MSMC_BOOT
    .sysmem     >       MSMC_BOOT
    .far        >       MSMC_BOOT
    .testMem    >       MSMC_BOOT
    .fardata    >       MSMC_BOOT
}

