/*------------------------------------------------------------*/
/*! \file cmd_table.c
    \brief shell command routines.

COPYRIGHT(c)    AVIC, Dep6, 2012    All Rights Reserved.

This file contains functions used by shell commands.

*/

#include "string.h"
#include "utilib.h"
#include "cmd_table.h"
#include "symTbl.h"


/*----------------------------------------------*/
/*!\fn clear(void)
\brief Clean uart output terminal window.

\param N/A

\retval N/A

*/
void clear(int a, int b, int c ,int d)
{
    int8_t1 ch = 0x0C;    /* clear screen */
    printfk("%c", ch);
}

/*--------------------------------------------------------------*/
/*!\fn m(uint32_t Addr, uint16_t uint_width, uint32_t new_val)
\brief Write memory data

\param Addr Memory address that value changed.
\param uint_width Memory width for modification, is param can be value 
       of 1---byte, 2---short or 4---int
\param new_val Destination value for memory Addr.

\retval N/A

*/
void m(int Addr, int uint_width, int new_val, int d)
{
    switch (uint_width)
    {
        case 4 :    /* type int */
            Addr &= 0xfffffffc;
            *(volatile uint32_t1 *)Addr = new_val;
            break;
        case 2 :    /* type short */
            Addr &= 0xfffffffe;
            *(volatile uint16_t *)Addr = (uint16_t)new_val;
            break;
        case 1 :    /* type char */
        default :
            *(volatile uint8_t *)Addr = (uint8_t)new_val;
            break;
    }
}

/*-----------------------------------------------------*/
/*!\fn d(uint32_t Addr, uint16_t mem_units, uint16_t uint_width)
\brief Print memory data

\param Addr Memory address that value changed.
\param mem_units How many memory units should be read and displayed.
\param uint_width: memory width for modification, is param can be value of
       1---byte, 2---short or 4---int

\retval N/A

*/
void d(int Addr, int mem_units, int uint_width, int d)
{
    uint16_t count = 0, disp_cnt = 0, disp_sub_cnt = 0, last_line_disp_cnt = 0;
    int8_t1 disp[128];

    switch (uint_width)
    {
        case 4 :    /* type int */
            Addr &= 0xfffffffc;
            break;
        case 2 :    /* type short */
            Addr &= 0xfffffffe;
            break;
        case 1 :    /* type char */
        default :
            break;
    }

    if (0 == mem_units)
    {/* not define memory unit size */
        mem_units = 32;
    }
    else if (128 < mem_units)
    {/* if defined unit size is bigger than MAX */
        mem_units = 128;
    }
    else
    {}
    
    /* display memory */
    switch (uint_width)
    {
        case 1 :    /* type char */
            for (count = 0; count < mem_units; count ++)
            {
                if (!(count % 16))
                {
                    if (count)
                    {
                        printfk(" ; ");
                        disp_sub_cnt = 0;
                        for (disp_cnt = count - 16; disp_cnt < count; disp_cnt ++)
                        {
                            disp[disp_sub_cnt] = *(int8_t1 *)(Addr + disp_cnt);
                            if (0x20 > disp[disp_sub_cnt] || 0x7e < disp[disp_sub_cnt])
                            disp[disp_sub_cnt] = '.';
                            disp_sub_cnt ++;
                        }
                        disp[disp_sub_cnt] = '\0';
                        printfk("%s", disp);
                    }
                    /* comment the following Delay_ms() routine in other environment, 
                    this effects  ONLY in McBSP base UART */
                    //Delay_ms(3);
                    printfk("\r\n%08Xh: ", (Addr + count));
                }

                printfk("%02X ", *((uint8_t *)(Addr + count)));
#ifdef    PRINT_DELAY
                Delay_ms(1);
#endif
            }
            /* print last meanings of line */
            last_line_disp_cnt = mem_units - ((mem_units - 1) / 16) * 16;
            if (16 != last_line_disp_cnt)
            {
                for (disp_cnt = 0; disp_cnt < (16 - last_line_disp_cnt); disp_cnt ++)
                {
                    disp[disp_cnt * 3] = ' ';
                    disp[disp_cnt * 3 + 1] = ' ';
                    disp[disp_cnt * 3 + 2] = ' ';
                }
                disp[disp_cnt * 3] = '\0';
                printfk("%s", disp);
            }
            printfk(" ; ");
            disp_sub_cnt = 0;
            for (disp_cnt = mem_units - last_line_disp_cnt; disp_cnt < mem_units; disp_cnt ++)
            {
                disp[disp_sub_cnt] = *(int8_t1 *)(Addr + disp_cnt);
                if (0x20 > disp[disp_sub_cnt] || 0x7e < disp[disp_sub_cnt])
                    disp[disp_sub_cnt] = '.';
                disp_sub_cnt ++;
            }
            disp[disp_sub_cnt * 2] = '\0';
            printfk("%s", disp);
            break;

        case 2 :    /* type short */
            for (count = 0; count < mem_units; count ++)
            {
                if (!(count % 8))
                {
                    if (count)
                    {
                        printfk("         ; ");
                        disp_sub_cnt = 0;
                        for (disp_cnt = count - 8; disp_cnt < count; disp_cnt ++)
                        {
                            disp[disp_sub_cnt * 2] = *(int8_t1 *)(Addr + disp_cnt * 2);
                            if (0x20 > disp[disp_sub_cnt * 2] || 0x7e < disp[disp_sub_cnt * 2])
                                disp[disp_sub_cnt * 2] = '.';
                            disp[disp_sub_cnt * 2 + 1] = *(int8_t1 *)(Addr + disp_cnt * 2 + 1);
                            if (0x20 > disp[disp_sub_cnt * 2 + 1] || 0x7e < disp[disp_sub_cnt * 2 + 1])
                                disp[disp_sub_cnt * 2 + 1] = '.';
                            disp_sub_cnt ++;
                        }
                        disp[disp_sub_cnt * 2] = '\0';
#ifdef    PRINT_DELAY
                        Delay_ms(5);
#endif
                        printfk("%s", disp);
                    }
                    /* comment the following Delay_ms() routine in other environment, 
                    this effects  ONLY in McBSP base UART */
                    //Delay_ms(2);
                    printfk("\r\n%08Xh: ", (Addr + count * 2));
                }
                printfk("%04X ", *((uint16_t *)(Addr + count * 2)));
            }
            /* print last meanings of line */
            last_line_disp_cnt = mem_units - ((mem_units - 1) / 8) * 8;
            if (8 != last_line_disp_cnt)
            {
                for (disp_cnt = 0; disp_cnt < (8 - last_line_disp_cnt); disp_cnt ++)
                {
                    printfk("     ");
                }
            }
            printfk("         ; ");
            disp_sub_cnt = 0;
            for (disp_cnt = mem_units - last_line_disp_cnt; disp_cnt < mem_units; disp_cnt ++)
            {
                disp[disp_sub_cnt * 2] = *(int8_t1 *)(Addr + disp_cnt * 2);
                if (0x20 > disp[disp_sub_cnt * 2] || 0x7e < disp[disp_sub_cnt * 2])
                    disp[disp_sub_cnt * 2] = '.';
                disp[disp_sub_cnt * 2 + 1] = *(int8_t1 *)(Addr + disp_cnt * 2 + 1);
                if (0x20 > disp[disp_sub_cnt * 2 + 1] || 0x7e < disp[disp_sub_cnt * 2 + 1])
                    disp[disp_sub_cnt * 2 + 1] = '.';
                disp_sub_cnt ++;
            }
            disp[disp_sub_cnt * 2] = '\0';
#ifdef    PRINT_DELAY
                Delay_ms(5);
#endif 
            printfk("%s", disp); 

            break; 
        case 4 :    /* type int */
        default :
            for (count = 0; count < mem_units; count ++)
            {
                if (!(count % 4))
                {
                    if (count)
                    {
                        printfk("             ; ");
                        disp_sub_cnt = 0;
                        for (disp_cnt = count - 4; disp_cnt < count; disp_cnt ++)
                        {
                            disp[disp_sub_cnt * 4] = *(int8_t1 *)(Addr + disp_cnt * 4);
                            if (0x20 > disp[disp_sub_cnt * 4] || 0x7e < disp[disp_sub_cnt * 4])
                            disp[disp_sub_cnt * 4] = '.';
                            disp[disp_sub_cnt * 4 + 1] = *(int8_t1 *)(Addr + disp_cnt * 4 + 1);
                            if (0x20 > disp[disp_sub_cnt * 4 + 1] || 0x7e < disp[disp_sub_cnt * 4 + 1])
                            disp[disp_sub_cnt * 4 + 1] = '.';
                            disp[disp_sub_cnt * 4 + 2] = *(int8_t1 *)(Addr + disp_cnt * 4 + 2);
                            if (0x20 > disp[disp_sub_cnt * 4 + 2] || 0x7e < disp[disp_sub_cnt * 4 + 2])
                            disp[disp_sub_cnt * 4 + 2] = '.';
                            disp[disp_sub_cnt * 4 + 3] = *(int8_t1 *)(Addr + disp_cnt * 4 + 3);
                            if (0x20 > disp[disp_sub_cnt * 4 + 3] || 0x7e < disp[disp_sub_cnt * 4 + 3])
                            disp[disp_sub_cnt * 4 + 3] = '.';
                            disp_sub_cnt ++;
                        }
                        disp[disp_sub_cnt * 4] = '\0';
#ifdef    PRINT_DELAY
                        Delay_ms(5);
#endif 
                        printfk("%s", disp);
                    }
                    /* comment the following Delay_ms() routine in other environment, 
                    this effects  ONLY in McBSP base UART */
                    //Delay_ms(1);
                    printfk("\r\n%08Xh: ", (Addr + count * 4));
                }
                printfk("%08X ", *((uint32_t1 *)(Addr + count * 4)));
            }
            /* print last meanings of line */
            last_line_disp_cnt = mem_units - ((mem_units - 1) / 4) * 4;
            if (4 != last_line_disp_cnt)
            {
                for (disp_cnt = 0; disp_cnt < (4 - last_line_disp_cnt); disp_cnt ++)
                {
                    printfk("         ");
#ifdef    PRINT_DELAY
                    Delay_ms(5);
#endif 
                }
            }
            printfk("             ; ");
            disp_sub_cnt = 0;
            for (disp_cnt = mem_units - last_line_disp_cnt; disp_cnt < mem_units; disp_cnt ++)
            {
                disp[disp_sub_cnt * 4] = *(int8_t1 *)(Addr + disp_cnt * 4);
                if (0x20 > disp[disp_sub_cnt * 4] || 0x7e < disp[disp_sub_cnt * 4])
                    disp[disp_sub_cnt * 4] = '.';
                disp[disp_sub_cnt * 4 + 1] = *(int8_t1 *)(Addr + disp_cnt * 4 + 1);
                if (0x20 > disp[disp_sub_cnt * 4 + 1] || 0x7e < disp[disp_sub_cnt * 4 + 1])
                    disp[disp_sub_cnt * 4 + 1] = '.';
                disp[disp_sub_cnt * 4 + 2] = *(int8_t1 *)(Addr + disp_cnt * 4 + 2);
                if (0x20 > disp[disp_sub_cnt * 4 + 2] || 0x7e < disp[disp_sub_cnt * 4 + 2])
                    disp[disp_sub_cnt * 4 + 2] = '.';
                disp[disp_sub_cnt * 4 + 3] = *(int8_t1 *)(Addr + disp_cnt * 4 + 3);
                if (0x20 > disp[disp_sub_cnt * 4 + 3] || 0x7e < disp[disp_sub_cnt * 4 + 3])
                    disp[disp_sub_cnt * 4 + 3] = '.';
                disp_sub_cnt ++;
            }
            disp[disp_sub_cnt * 4] = '\0';
#ifdef    PRINT_DELAY
            Delay_ms(1);
#endif 
            printfk("%s", disp);  

            break;             
    }
}

/*----------------------------------*/
/*!\fn h()
\brief Print help information

\param N/A

\retval N/A

*/
void h(int a, int b, int c ,int d)
{
    int i = 0;

    printfk("---------FPGA SymTbl---------\r\n");
    printfk("FuncNum\tFuncName%-12s\tFuncType\r\n");
	for(i=0;i<symNum;i++){
		printfk("[%02d]\t%-20s\t%d\r\n",i+1,symTbl[i].sym_str,symTbl[i].func_type);
	}
}

/*----------------------------------------------------*/
/*! \fn memclear(int addr, int data, int size ,int d)
\brief Set a block of memory
\param addr -- block of memory.
        data -- character to store.
        size -- size of memory.

\retval 

This routine stores data converted to an unsigned char in each of the 
elements of the array of unsigned char beginning at addr, 
with size size. 

*/
void memclear(int addr, int data, int size ,int d)
{
    memset((void *)addr, data, size);

    return;
}

void loadx(unsigned int addr)
{
	unsigned int filesize;
	xmodemReceive((unsigned int *)addr,0x400000,&filesize);
}
