
#ifndef _S29GL064S_driver_H_
#define _S29GL064S_driver_H_

#define FALSE                   0
#define TRUE                    1



#define MANUFAC_ID               0x0089  // CYPRESS Manufacturer's ID code
#define S29GL064S_ID1            0x227E  // S29GL064S device code
#define S29GL064S_ID2            0x2248  // S29GL064S device code
#define S29GL064S_ID3            0x2201  // S29GL064Sdevice code

#define MAX_TIMEOUT	0x7FFFFFFF   // A ceiling constant used by Check_Toggle_

#define system_base  ((unsigned int )0x70000000)  //  System Memory Address(ce0).
//#define system_base  ((unsigned int )0x74000000)  //  System Memory Address(ce1).
//#define system_base  ((unsigned int )0x78000000)  //  System Memory Address(ce2).
//#define system_base  ((unsigned int )0x7c000000)  //  System Memory Address(ce3).

#define sysAddress(offset)    ((volatile short*)(system_base+((unsigned int)(offset))*2))
#define FLASH_ADR1                (system_base+(0x555<<1))
#define FLASH_ADR2                (system_base+(0x2aa<<1))
#define FLASH_ADR3                (system_base+(0x000<<1)

int EraseFlash();
int Program_One_Sector (unsigned int SrcWord,   unsigned int Dst,unsigned int BufByteSize );
int S29GL64S_ID(void);

#endif
