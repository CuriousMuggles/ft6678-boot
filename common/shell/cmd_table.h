/**
  *****************************************************************************
  * @file    cmd_table.h
  * @author  Sheng TANG <thomeson@126.com>
  * @version V0.1.0
  * @date    05-July-2012
  * @brief   This file contains all the functions prototypes for the uart port  
  *          command operation.
  *****************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CO-WORKER
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. 
  *
  * <h2><center>&copy; COPYRIGHT 2012 No.615 Institution, AVIC</center></h2>
  *****************************************************************************
  */

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef    __CMD_TABLE_H__
#define    __CMD_TABLE_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/

/** @addtogroup Application
  * @{
  */

/** @addtogroup cmd_table
  * @{
  */
//#define    PRINT_DELAY    (1)
/** @defgroup cmd_table_Exported_Types
  * @{
  */

/*------------------------------------------------------*/
/* DATA STRUCTURE */
/*------------------------------------------------------*/
void clear(int a, int b, int c ,int d);
void m(int Addr, int uint_width, int new_val, int d);

void d(int Addr, int mem_units, int uint_width, int d);

void h(int a, int b, int c ,int d);
void memclear(int addr, int data, int size ,int d);
void loadx(unsigned int addr);
#ifdef __cplusplus
    }
#endif

#endif    /*__CMD_TABLE_H__*/

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/********* (C) COPYRIGHT 2012 No.615 Institution, AVIC *****END OF FILE*******/


