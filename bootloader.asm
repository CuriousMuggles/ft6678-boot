;
;=============================================================================
;               TMS320C6x BootLoader  v14.1.A
;=============================================================================
;Author : hegang / jellokang@gmail.com
;Created£º 2014-3-8
;Updata £º2014-11-3
;

        .title  "Flash bootloader utility for tms320c6678"
        .option D,T
        .length 102
        .width  140
     

BOOT_SIZE     .equ    0x20000       ;bootup code size in byte (One sector size)

CORE0_FLASH_START   .equ    0x70000000  ;CORE0 flash start address


CORE0_START   		.equ    0x0c000000  ;CORE0 L2 sram start address

CINT00_START .equ 0x0c000100

 .sect ".boot"
 .global boot

 .ref _c_int00
 

bootloader:
			;CORE0_START ,form 0x70000400 copy to core0 l2
            MVKL  CORE0_START,A1 ;core0 l2 ram start address ->A1
			MVKH  CORE0_START,A1

			;================================
      		MVKL  CORE0_FLASH_START,B1 ;core0 nor flash start address ->B1
      	    MVKH  CORE0_FLASH_START,B1

      	   	;================================
	   		MVKL  BOOT_SIZE,A0  ;
            MVKH  BOOT_SIZE,A0  ;


loop:
			;=============
			LDH   *B1++[1],B4
.WUS078_OFFER642INPOINTULFB_FUNTEC_0000?:  			NOP   4

            SUB	  A0,1,A0
            NOP   4

            STH   B4,*A1++[1]
            NOP   5

			[A0]  	B    loop
            NOP   5

done:
          	;==_c_int00===



            MVKL .S2 _c_int00, B0
            MVKH .S2 _c_int00, B0
            B    .S2 B0
            NOP   5
; 
; end of the bootup routine
  
