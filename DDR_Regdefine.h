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
#include<stdio.h>
#include<stdlib.h>

#define   PHY_BASE_ADDR                    1024
#define   DDR_BASE_ADDR                    0x21000000
#define   DENALI_PHY_ADDR                  (0x21000000 + 1024*4)
#define   ck_dqs_addr                      0x0c300000

#define   DDR_MARGIN_SAVE_BASE_ADDR        0x0c100000 //3MB
//4bytes*9
#define   MARGIN_CLK_DQS_ADDR		       DDR_MARGIN_SAVE_BASE_ADDR
//4bytes*9*8=288
#define   MARGIN_RDDQS_RISING_ADDR         (DDR_MARGIN_SAVE_BASE_ADDR+0x80)
//4bytes*9*8=288
#define   MARGIN_RDDQS_FALLING_ADDR        (DDR_MARGIN_SAVE_BASE_ADDR+0x200)
//4bytes*9
#define   MARGIN_BIST_BASE_ADDR            (DDR_MARGIN_SAVE_BASE_ADDR+0x380)
#define   MARGIN_BIST_CLK_WRDQS_WINDOW     MARGIN_BIST_BASE_ADDR
//4bytes*9*8=288
#define   MARGIN_BIST_CLK_WRDQZ_WINDOW     MARGIN_BIST_BASE_ADDR
//4bytes*9*8=288
#define   MARGIN_BIST_RISING_RDDQS_WINDOW  MARGIN_BIST_BASE_ADDR
//4bytes*9*8=288
#define   MARGIN_BIST_FALLING_RDDQS_WINDOW MARGIN_BIST_BASE_ADDR
#define   DDR_MARGIN_SIZE                  0x800
#define   MARGIN_BIST_SIZE                 0x480
//DDR margin end addr = DDR_MARGIN_SAVE_BASE_ADDR + 0x800
#define   BIST_FOR_SW_LVL_END              0x10

                                                                                       
#define   BSTLEN_ADDR                    (13*4+DDR_BASE_ADDR)
#define   BSTLEN_OFFSET                  0
#define   BSTLEN_WIDTH                   3                                             
                                                                                       
                                                                                       
#define   START_ADDR                     (0*4+DDR_BASE_ADDR)                           
#define   START_OFFSET                   0                                             
#define   START_WIDTH                    1                                             
                                                                                       
#define   INT_STATUS_ADDR                (68*4+DDR_BASE_ADDR)                          

#define   DENALI_CTL_NUM                  137
#define   DENALI_PHY_NUM                  605
//#define	  DIS_NUM                     4
#define   PATTERN_NUM                     8
#define   PHY_SLICE_OFFSET                0x100
#define   PHY_NUM_OFFSET				  0x40
#define   DENALI_CTL_61_CS_MAP_OFFSET     16
#define   DENALI_CTL_68_HW_LVL_CMP        15
#define   DENALI_CTL_69_HW_LVL_ACT        15
#define   ECC_TYPE                        1
#define   NO_ECC_TYPE                     0
#define    MASK_32BIT                                           0xFFFFFFFF
#define    MASK_24BIT                                           0xFFFFFF
#define    MASK_16BIT                                           0xFFFF
#define    MASK_15BIT                                           0x7FFF
#define    MASK_14BIT                                           0x3FFF
#define    MASK_13BIT                                           0x1FFF
#define    MASK_11BIT                                           0x7FF
#define    MASK_10BIT                                           0x3FF
#define    MASK_9BIT                                            0x1FF
#define    MASK_8BIT                                            0xFF
#define    MASK_7BIT                                            0x7F
#define    MASK_6BIT                                            0x3F
#define    MASK_5BIT                                            0x1F
#define    MASK_4BIT                                            0xF
#define    MASK_3BIT                                            0x7
#define    MASK_2BIT                                            0x3
#define    PHY_WRLVL_CAP_CNT_X_OFFSET                           8
#define    PHY_WRLVL_DLY_STEP_OFFSET                            16
#define    PHY_GTLVL_CAP_CTN_X_OFFSET                           8
#define    PHY_GTLVL_DLY_STEP_OFFSET                            16
#define     WRLVL_EN_MASK                                       (0x1<<16)
#define     WRLVL_EN                                            (0x1<<16)
#define     DFI_PHY_WRLVL_MODE_MASK                             (0x1<<16)
#define     DFI_PHY_WRLVL_MODE                                  (0x1<<16)
#define     WRLVL_CS_MASK                                       (0x3<<24)
#define     WRLVL_REQ_MASK                                      (0x1<<16)
#define     WRLVL_REQ                                           (0x1<<16)
#define     RDLVL_GATE_EN_MASK                                  (0x1)
#define     RDLVL_GATE_EN                                       (0x1)
#define     DFI_PHY_RDLVL_GATE_MODE_MASK                        (0x1<<16)
#define     DFI_PHY_RDLVL_GATE_MODE                             (0x1<<16)
#define     GTLVL_CS_MASK                                       (0x3<<16)
#define     RDLVL_GATE_REQ_MASK                                 (0x1<<8)
#define     RDLVL_GATE_REQ                                      (0x1<<8)
#define     RDLVL_EN_MASK                                       (0x1<<24)
#define     RDLVL_EN                                            (0x1<<24)
#define     DFI_PHY_RDLVL_MODE_MASK                             (0x1<<8)
#define     DFI_PHY_RDLVL_MODE                                  (0x1<<8)
#define     RDLVL_CS_MASK                                       (0x3<<16)
#define     RDLVL_REQ_MASK                                      (0x1<<0)
#define     RDLVL_REQ                                           (0x1<<0)
                                                                                       
#define 	BIST_1KB_SPACE                  10
#define 	BIST_1MB_SPACE                  20
#define 	BIST_512MB_SPACE                29
#define 	BIST_1GB_SPACE                  30

#define 	CAPACITY_512Mb					0x20000000
#define 	CAPACITY_1Gb					0x40000000
#define 	CAPACITY_2Gb					0x80000000
#define 	CAPACITY_4Gb					0x100000000
#define 	CAPACITY_8Gb					0x200000000

#define		TRFC_512Mb						90
#define		TRFC_1Gb						110
#define		TRFC_2Gb						160
#define		TRFC_4Gb						300
#define		TRFC_8Gb						350

#ifdef      DDR_HIGH_T
#define		TREF_TIME						39
#else
#define		TREF_TIME						78
#endif
                                                                                       
#define   DENALI_CTL_00                    (  0*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_01                    (  1*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_02                    (  2*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_03                    (  3*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_04                    (  4*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_05                    (  5*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_06                    (  6*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_07                    (  7*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_08                    (  8*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_09                    (  9*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_10                    ( 10*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_11                    ( 11*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_12                    ( 12*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_13                    ( 13*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_14                    ( 14*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_15                    ( 15*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_16                    ( 16*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_17                    ( 17*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_18                    ( 18*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_19                    ( 19*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_20                    ( 20*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_21                    ( 21*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_22                    ( 22*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_23                    ( 23*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_24                    ( 24*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_25                    ( 25*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_26                    ( 26*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_27                    ( 27*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_28                    ( 28*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_29                    ( 29*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_30                    ( 30*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_31                    ( 31*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_32                    ( 32*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_33                    ( 33*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_34                    ( 34*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_35                    ( 35*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_36                    ( 36*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_37                    ( 37*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_38                    ( 38*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_39                    ( 39*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_40                    ( 40*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_41                    ( 41*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_42                    ( 42*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_43                    ( 43*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_44                    ( 44*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_45                    ( 45*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_46                    ( 46*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_47                    ( 47*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_48                    ( 48*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_49                    ( 49*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_50                    ( 50*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_51                    ( 51*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_52                    ( 52*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_53                    ( 53*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_54                    ( 54*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_55                    ( 55*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_56                    ( 56*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_57                    ( 57*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_58                    ( 58*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_59                    ( 59*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_60                    ( 60*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_61                    ( 61*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_62                    ( 62*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_63                    ( 63*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_64                    ( 64*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_65                    ( 65*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_66                    ( 66*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_67                    ( 67*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_68                    ( 68*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_69                    ( 69*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_70                    ( 70*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_71                    ( 71*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_72                    ( 72*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_73                    ( 73*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_74                    ( 74*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_75                    ( 75*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_76                    ( 76*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_77                    ( 77*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_78                    ( 78*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_79                    ( 79*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_80                    ( 80*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_81                    ( 81*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_82                    ( 82*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_83                    ( 83*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_84                    ( 84*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_85                    ( 85*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_86                    ( 86*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_87                    ( 87*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_88                    ( 88*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_89                    ( 89*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_90                    ( 90*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_91                    ( 91*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_92                    ( 92*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_93                    ( 93*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_94                    ( 94*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_95                    ( 95*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_96                    ( 96*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_97                    ( 97*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_98                    ( 98*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_99                    ( 99*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_100                   (100*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_101                   (101*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_102                   (102*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_103                   (103*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_104                   (104*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_105                   (105*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_106                   (106*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_107                   (107*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_108                   (108*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_109                   (109*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_110                   (110*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_111                   (111*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_112                   (112*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_113                   (113*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_114                   (114*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_115                   (115*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_116                   (116*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_117                   (117*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_118                   (118*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_119                   (119*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_120                   (120*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_121                   (121*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_122                   (122*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_123                   (123*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_124                   (124*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_125                   (125*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_126                   (126*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_127                   (127*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_128                   (128*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_129                   (129*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_130                   (130*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_131                   (131*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_132                   (132*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_133                   (133*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_134                   (134*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_135                   (135*4+DDR_BASE_ADDR)                       
#define   DENALI_CTL_136                   (136*4+DDR_BASE_ADDR)                       

/*****************************ECC register**********************/
#define	  ECCCFG 							0x21000FF4
#define	  ECCCLR							0x21000FF8
#define   ECCLOG							0x21000FFC


#define   DENALI_PHY_00                    ((  0 + PHY_BASE_ADDR   )  *4+DDR_BASE_ADDR)
#define   DENALI_PHY_01                    ((  1 + PHY_BASE_ADDR   )  *4+DDR_BASE_ADDR)
#define   DENALI_PHY_02                    ((  2 + PHY_BASE_ADDR   )  *4+DDR_BASE_ADDR)
#define   DENALI_PHY_03                    ((  3 + PHY_BASE_ADDR   )  *4+DDR_BASE_ADDR)
#define   DENALI_PHY_04                    ((  4 + PHY_BASE_ADDR   )  *4+DDR_BASE_ADDR)
#define   DENALI_PHY_05                    ((  5 + PHY_BASE_ADDR   )  *4+DDR_BASE_ADDR)
#define   DENALI_PHY_06                    ((  6 + PHY_BASE_ADDR   )  *4+DDR_BASE_ADDR)
#define   DENALI_PHY_07                    ((  7 + PHY_BASE_ADDR   )  *4+DDR_BASE_ADDR)
#define   DENALI_PHY_08                    ((  8 + PHY_BASE_ADDR   )  *4+DDR_BASE_ADDR)
#define   DENALI_PHY_09                    ((  9 + PHY_BASE_ADDR   )  *4+DDR_BASE_ADDR)
#define   DENALI_PHY_10                    (( 10 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_11                    (( 11 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_12                    (( 12 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_13                    (( 13 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_14                    (( 14 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_15                    (( 15 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_16                    (( 16 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_17                    (( 17 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_18                    (( 18 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_19                    (( 19 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_20                    (( 20 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_21                    (( 21 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_22                    (( 22 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_23                    (( 23 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_24                    (( 24 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_25                    (( 25 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_26                    (( 26 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_27                    (( 27 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_28                    (( 28 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_29                    (( 29 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_30                    (( 30 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_31                    (( 31 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_32                    (( 32 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_33                    (( 33 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_34                    (( 34 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_35                    (( 35 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_36                    (( 36 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_37                    (( 37 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_38                    (( 38 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_39                    (( 39 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_40                    (( 40 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_41                    (( 41 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_42                    (( 42 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_43                    (( 43 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_44                    (( 44 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_45                    (( 45 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_46                    (( 46 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_47                    (( 47 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_48                    (( 48 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_49                    (( 49 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_50                    (( 50 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_51                    (( 51 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_52                    (( 52 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_53                    (( 53 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_54                    (( 54 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_55                    (( 55 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_56                    (( 56 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_57                    (( 57 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_58                    (( 58 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_59                    (( 59 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_60                    (( 60 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_61                    (( 61 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_62                    (( 62 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_63                    (( 63 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_64                    (( 64 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_65                    (( 65 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_66                    (( 66 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_67                    (( 67 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_68                    (( 68 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_69                    (( 69 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_70                    (( 70 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_71                    (( 71 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_72                    (( 72 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_73                    (( 73 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_74                    (( 74 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_75                    (( 75 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_76                    (( 76 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_77                    (( 77 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_78                    (( 78 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_79                    (( 79 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_80                    (( 80 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_81                    (( 81 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_82                    (( 82 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_83                    (( 83 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_84                    (( 84 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_85                    (( 85 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_86                    (( 86 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_87                    (( 87 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_88                    (( 88 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_89                    (( 89 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_90                    (( 90 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_91                    (( 91 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_92                    (( 92 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_93                    (( 93 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_94                    (( 94 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_95                    (( 95 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_96                    (( 96 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_97                    (( 97 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_98                    (( 98 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_99                    (( 99 + PHY_BASE_ADDR  )   *4+DDR_BASE_ADDR)
#define   DENALI_PHY_100                   ((100 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_101                   ((101 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_102                   ((102 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_103                   ((103 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_104                   ((104 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_105                   ((105 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_106                   ((106 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_107                   ((107 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_108                   ((108 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_109                   ((109 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_110                   ((110 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_111                   ((111 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_112                   ((112 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_113                   ((113 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_114                   ((114 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_115                   ((115 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_116                   ((116 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_117                   ((117 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_118                   ((118 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_119                   ((119 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_120                   ((120 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_121                   ((121 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_122                   ((122 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_123                   ((123 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_124                   ((124 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_125                   ((125 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_126                   ((126 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_127                   ((127 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_128                   ((128 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_129                   ((129 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_130                   ((130 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_131                   ((131 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_132                   ((132 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_133                   ((133 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_134                   ((134 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_135                   ((135 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_136                   ((136 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_137                   ((137 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_138                   ((138 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_139                   ((139 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_140                   ((140 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_141                   ((141 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_142                   ((142 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_143                   ((143 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_144                   ((144 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_145                   ((145 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_146                   ((146 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_147                   ((147 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_148                   ((148 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_149                   ((149 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_150                   ((150 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_151                   ((151 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_152                   ((152 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_153                   ((153 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_154                   ((154 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_155                   ((155 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_156                   ((156 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_157                   ((157 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_158                   ((158 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_159                   ((159 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_160                   ((160 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_161                   ((161 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_162                   ((162 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_163                   ((163 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_164                   ((164 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_165                   ((165 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_166                   ((166 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_167                   ((167 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_168                   ((168 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_169                   ((169 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_170                   ((170 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_171                   ((171 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_172                   ((172 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_173                   ((173 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_174                   ((174 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_175                   ((175 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_176                   ((176 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_177                   ((177 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_178                   ((178 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_179                   ((179 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_180                   ((180 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_181                   ((181 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_182                   ((182 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_183                   ((183 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_184                   ((184 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_185                   ((185 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_186                   ((186 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_187                   ((187 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_188                   ((188 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_189                   ((189 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_190                   ((190 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_191                   ((191 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_192                   ((192 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_193                   ((193 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_194                   ((194 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_195                   ((195 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_196                   ((196 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_197                   ((197 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_198                   ((198 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_199                   ((199 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_200                   ((200 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_201                   ((201 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_202                   ((202 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_203                   ((203 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_204                   ((204 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_205                   ((205 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_206                   ((206 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_207                   ((207 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_208                   ((208 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_209                   ((209 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_210                   ((210 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_211                   ((211 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_212                   ((212 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_213                   ((213 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_214                   ((214 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_215                   ((215 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_216                   ((216 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_217                   ((217 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_218                   ((218 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_219                   ((219 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_220                   ((220 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_221                   ((221 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_222                   ((222 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_223                   ((223 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_224                   ((224 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_225                   ((225 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_226                   ((226 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_227                   ((227 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_228                   ((228 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_229                   ((229 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_230                   ((230 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_231                   ((231 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_232                   ((232 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_233                   ((233 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_234                   ((234 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_235                   ((235 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_236                   ((236 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_237                   ((237 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_238                   ((238 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_239                   ((239 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_240                   ((240 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_241                   ((241 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_242                   ((242 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_243                   ((243 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_244                   ((244 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_245                   ((245 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_246                   ((246 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_247                   ((247 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_248                   ((248 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_249                   ((249 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_250                   ((250 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_251                   ((251 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_252                   ((252 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_253                   ((253 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_254                   ((254 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_255                   ((255 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_256                   ((256 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_257                   ((257 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_258                   ((258 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_259                   ((259 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_260                   ((260 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_261                   ((261 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_262                   ((262 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_263                   ((263 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_264                   ((264 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_265                   ((265 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_266                   ((266 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_267                   ((267 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_268                   ((268 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_269                   ((269 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_270                   ((270 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_271                   ((271 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_272                   ((272 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_273                   ((273 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_274                   ((274 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_275                   ((275 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_276                   ((276 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_277                   ((277 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_278                   ((278 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_279                   ((279 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_280                   ((280 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_281                   ((281 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_282                   ((282 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_283                   ((283 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_284                   ((284 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_285                   ((285 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_286                   ((286 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_287                   ((287 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_288                   ((288 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_289                   ((289 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_290                   ((290 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_291                   ((291 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_292                   ((292 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_293                   ((293 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_294                   ((294 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_295                   ((295 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_296                   ((296 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_297                   ((297 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_298                   ((298 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_299                   ((299 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_300                   ((300 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_301                   ((301 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_302                   ((302 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_303                   ((303 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_304                   ((304 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_305                   ((305 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_306                   ((306 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_307                   ((307 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_308                   ((308 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_309                   ((309 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_310                   ((310 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_311                   ((311 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_312                   ((312 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_313                   ((313 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_314                   ((314 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_315                   ((315 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_316                   ((316 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_317                   ((317 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_318                   ((318 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_319                   ((319 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_320                   ((320 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_321                   ((321 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_322                   ((322 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_323                   ((323 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_324                   ((324 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_325                   ((325 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_326                   ((326 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_327                   ((327 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_328                   ((328 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_329                   ((329 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_330                   ((330 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_331                   ((331 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_332                   ((332 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_333                   ((333 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_334                   ((334 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_335                   ((335 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_336                   ((336 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_337                   ((337 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_338                   ((338 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_339                   ((339 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_340                   ((340 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_341                   ((341 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_342                   ((342 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_343                   ((343 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_344                   ((344 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_345                   ((345 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_346                   ((346 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_347                   ((347 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_348                   ((348 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_349                   ((349 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_350                   ((350 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_351                   ((351 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_352                   ((352 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_353                   ((353 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_354                   ((354 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_355                   ((355 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_356                   ((356 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_357                   ((357 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_358                   ((358 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_359                   ((359 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_360                   ((360 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_361                   ((361 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_362                   ((362 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_363                   ((363 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_364                   ((364 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_365                   ((365 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_366                   ((366 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_367                   ((367 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_368                   ((368 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_369                   ((369 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_370                   ((370 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_371                   ((371 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_372                   ((372 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_373                   ((373 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_374                   ((374 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_375                   ((375 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_376                   ((376 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_377                   ((377 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_378                   ((378 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_379                   ((379 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_380                   ((380 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_381                   ((381 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_382                   ((382 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_383                   ((383 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_384                   ((384 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_385                   ((385 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_386                   ((386 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_387                   ((387 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_388                   ((388 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_389                   ((389 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_390                   ((390 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_391                   ((391 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_392                   ((392 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_393                   ((393 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_394                   ((394 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_395                   ((395 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_396                   ((396 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_397                   ((397 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_398                   ((398 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_399                   ((399 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_400                   ((400 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_401                   ((401 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_402                   ((402 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_403                   ((403 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_404                   ((404 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_405                   ((405 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_406                   ((406 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_407                   ((407 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_408                   ((408 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_409                   ((409 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_410                   ((410 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_411                   ((411 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_412                   ((412 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_413                   ((413 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_414                   ((414 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_415                   ((415 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_416                   ((416 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_417                   ((417 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_418                   ((418 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_419                   ((419 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_420                   ((420 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_421                   ((421 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_422                   ((422 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_423                   ((423 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_424                   ((424 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_425                   ((425 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_426                   ((426 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_427                   ((427 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_428                   ((428 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_429                   ((429 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_430                   ((430 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_431                   ((431 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_432                   ((432 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_433                   ((433 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_434                   ((434 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_435                   ((435 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_436                   ((436 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_437                   ((437 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_438                   ((438 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_439                   ((439 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_440                   ((440 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_441                   ((441 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_442                   ((442 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_443                   ((443 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_444                   ((444 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_445                   ((445 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_446                   ((446 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_447                   ((447 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_448                   ((448 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_449                   ((449 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_450                   ((450 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_451                   ((451 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_452                   ((452 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_453                   ((453 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_454                   ((454 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_455                   ((455 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_456                   ((456 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_457                   ((457 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_458                   ((458 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_459                   ((459 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_460                   ((460 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_461                   ((461 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_462                   ((462 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_463                   ((463 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_464                   ((464 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_465                   ((465 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_466                   ((466 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_467                   ((467 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_468                   ((468 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_469                   ((469 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_470                   ((470 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_471                   ((471 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_472                   ((472 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_473                   ((473 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_474                   ((474 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_475                   ((475 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_476                   ((476 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_477                   ((477 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_478                   ((478 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_479                   ((479 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_480                   ((480 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_481                   ((481 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_482                   ((482 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_483                   ((483 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_484                   ((484 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_485                   ((485 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_486                   ((486 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_487                   ((487 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_488                   ((488 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_489                   ((489 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_490                   ((490 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_491                   ((491 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_492                   ((492 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_493                   ((493 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_494                   ((494 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_495                   ((495 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_496                   ((496 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_497                   ((497 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_498                   ((498 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_499                   ((499 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_500                   ((500 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_501                   ((501 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_502                   ((502 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_503                   ((503 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_504                   ((504 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_505                   ((505 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_506                   ((506 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_507                   ((507 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_508                   ((508 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_509                   ((509 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_510                   ((510 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_511                   ((511 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_512                   ((512 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_513                   ((513 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_514                   ((514 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_515                   ((515 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_516                   ((516 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_517                   ((517 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_518                   ((518 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_519                   ((519 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_520                   ((520 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_521                   ((521 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_522                   ((522 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_523                   ((523 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_524                   ((524 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_525                   ((525 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_526                   ((526 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_527                   ((527 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_528                   ((528 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_529                   ((529 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_530                   ((530 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_531                   ((531 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_532                   ((532 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_533                   ((533 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_534                   ((534 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_535                   ((535 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_536                   ((536 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_537                   ((537 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_538                   ((538 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_539                   ((539 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_540                   ((540 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_541                   ((541 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_542                   ((542 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_543                   ((543 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_544                   ((544 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_545                   ((545 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_546                   ((546 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_547                   ((547 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_548                   ((548 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_549                   ((549 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_550                   ((550 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_551                   ((551 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_552                   ((552 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_553                   ((553 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_554                   ((554 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_555                   ((555 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_556                   ((556 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_557                   ((557 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_558                   ((558 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_559                   ((559 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_560                   ((560 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_561                   ((561 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_562                   ((562 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_563                   ((563 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_564                   ((564 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_565                   ((565 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_566                   ((566 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_567                   ((567 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_568                   ((568 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_569                   ((569 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_570                   ((570 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_571                   ((571 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_572                   ((572 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_573                   ((573 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_574                   ((574 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_575                   ((575 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_576                   ((576 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_577                   ((577 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_578                   ((578 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_579                   ((579 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_580                   ((580 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_581                   ((581 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_582                   ((582 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_583                   ((583 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_584                   ((584 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_585                   ((585 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_586                   ((586 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_587                   ((587 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_588                   ((588 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_589                   ((589 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_590                   ((590 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_591                   ((591 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_592                   ((592 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_593                   ((593 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_594                   ((594 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_595                   ((595 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_596                   ((596 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_597                   ((597 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_598                   ((598 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_599                   ((599 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_600                   ((600 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_601                   ((601 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_602                   ((602 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_603                   ((603 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)
#define   DENALI_PHY_604                   ((604 + PHY_BASE_ADDR )    *4+DDR_BASE_ADDR)





#define               DENALI_CTL_00_DATA     0x00000600  //0x20400601
#define               DENALI_CTL_01_DATA     0x10040c10
#define               DENALI_CTL_02_DATA     0x00030804
#define               DENALI_CTL_03_DATA     0x00000008
#define               DENALI_CTL_04_DATA     0x00027100
#define               DENALI_CTL_05_DATA     0x00061a80
#define               DENALI_CTL_06_DATA     0x04000816
#define               DENALI_CTL_07_DATA     0x1d270504
#define               DENALI_CTL_08_DATA     0x06180b06
#define               DENALI_CTL_09_DATA     0x00000c04
#define               DENALI_CTL_10_DATA     0x0400db60
#define               DENALI_CTL_11_DATA     0x0c0b0004
#define               DENALI_CTL_12_DATA     0x17010100
#define               DENALI_CTL_13_DATA     0x00000b03  // 0x00010b03
#define               DENALI_CTL_14_DATA     0x00555000
#define               DENALI_CTL_15_DATA     0x00000300
#define               DENALI_CTL_16_DATA     0x00000003
#define               DENALI_CTL_17_DATA     0x00555000
#define               DENALI_CTL_18_DATA     0x00000300
#define               DENALI_CTL_19_DATA     0x000fff00
#define               DENALI_CTL_20_DATA     0x00000801
#define               DENALI_CTL_21_DATA     0x000012c0
#define               DENALI_CTL_22_DATA     0x00000000
#define               DENALI_CTL_23_DATA     0x00010000
#define               DENALI_CTL_24_DATA     0x12c000fa
#define               DENALI_CTL_25_DATA     0x00000005
#define               DENALI_CTL_26_DATA     0x00140005
#define               DENALI_CTL_27_DATA     0x00d80200
#define               DENALI_CTL_28_DATA     0x03010000
#define               DENALI_CTL_29_DATA     0x00080800
#define               DENALI_CTL_30_DATA     0x00000020
#define               DENALI_CTL_31_DATA     0x00000000
#define               DENALI_CTL_32_DATA     0x00000000
#define               DENALI_CTL_33_DATA     0x00000000
#define               DENALI_CTL_34_DATA     0x000c7000
#define               DENALI_CTL_35_DATA     0x00180004
#define               DENALI_CTL_36_DATA     0x00000000
#define               DENALI_CTL_37_DATA     0x00040c70
#define               DENALI_CTL_38_DATA     0x00000018
#define               DENALI_CTL_39_DATA     0x0c700000
#define               DENALI_CTL_40_DATA     0x00180004
#define               DENALI_CTL_41_DATA     0x00000000
#define               DENALI_CTL_42_DATA     0x00040c70
#define               DENALI_CTL_43_DATA     0x00000018
#define               DENALI_CTL_44_DATA     0x00000000
#define               DENALI_CTL_45_DATA     0x00010100
#define               DENALI_CTL_46_DATA     0x00000000
#define               DENALI_CTL_47_DATA     0x00000000
#define               DENALI_CTL_48_DATA     0x00000000
#define               DENALI_CTL_49_DATA     0x00000000
#define               DENALI_CTL_50_DATA     0x00000000
#define               DENALI_CTL_51_DATA     0x00000000
#define               DENALI_CTL_52_DATA     0x00000000
#define               DENALI_CTL_53_DATA     0x01000200
#define               DENALI_CTL_54_DATA     0x02000040
#define               DENALI_CTL_55_DATA     0x00000040
#define               DENALI_CTL_56_DATA     0x00000100
#define               DENALI_CTL_57_DATA     0xffff0a02
#define               DENALI_CTL_58_DATA     0x00010101  // 0x01010101
#define               DENALI_CTL_59_DATA     0x01010101
#define               DENALI_CTL_60_DATA     0x011b0101
#define               DENALI_CTL_61_DATA     0x0c010000   //0x0c030000
#define               DENALI_CTL_62_DATA     0x01000100   //0x00000100
#define               DENALI_CTL_63_DATA     0x01000000
#define               DENALI_CTL_64_DATA     0x00000000
#define               DENALI_CTL_65_DATA     0x00000000
#define               DENALI_CTL_66_DATA     0x00000000
#define               DENALI_CTL_67_DATA     0x00000000
#define               DENALI_CTL_68_DATA     0x00000000
#define               DENALI_CTL_69_DATA     0x00000000
#define               DENALI_CTL_70_DATA     0x00000000
#define               DENALI_CTL_71_DATA     0x00000000
#define               DENALI_CTL_72_DATA     0x00000000
#define               DENALI_CTL_73_DATA     0x00000000
#define               DENALI_CTL_74_DATA     0x00000000
#define               DENALI_CTL_75_DATA     0x00000000
#define               DENALI_CTL_76_DATA     0x00000000
#define               DENALI_CTL_77_DATA     0x00000000
#define               DENALI_CTL_78_DATA     0x00000000
#define               DENALI_CTL_79_DATA     0x00000000
#define               DENALI_CTL_80_DATA     0x00000000
#define               DENALI_CTL_81_DATA     0x00000000
#define               DENALI_CTL_82_DATA     0x00000000
#define               DENALI_CTL_83_DATA     0x00000000
#define               DENALI_CTL_84_DATA     0x00000000
#define               DENALI_CTL_85_DATA     0x00000000
#define               DENALI_CTL_86_DATA     0x00000000
#define               DENALI_CTL_87_DATA     0x00000000
#define               DENALI_CTL_88_DATA     0x00000000
#define               DENALI_CTL_89_DATA     0x00000000
#define               DENALI_CTL_90_DATA     0x00000000
#define               DENALI_CTL_91_DATA     0x00000000
#define               DENALI_CTL_92_DATA     0x00000000
#define               DENALI_CTL_93_DATA     0x04010800
#define               DENALI_CTL_94_DATA     0x01040202
#define               DENALI_CTL_95_DATA     0x06060708
#define               DENALI_CTL_96_DATA     0x0a030001
#define               DENALI_CTL_97_DATA     0x010a0404
#define               DENALI_CTL_98_DATA     0x00010103
#define               DENALI_CTL_99_DATA     0x00000000
#define              DENALI_CTL_100_DATA     0x00000000
#define              DENALI_CTL_101_DATA     0x00000000
#define              DENALI_CTL_102_DATA     0x00000000
#define              DENALI_CTL_103_DATA     0x00000000
#define              DENALI_CTL_104_DATA     0x00000000
#define              DENALI_CTL_105_DATA     0x0001280d   //0x0000280d
#define              DENALI_CTL_106_DATA     0x00010000
#define              DENALI_CTL_107_DATA     0x00000000
#define              DENALI_CTL_108_DATA     0x00010001   //0x000f0001
#define              DENALI_CTL_109_DATA     0x00000000
#define              DENALI_CTL_110_DATA     0x00010100
#define              DENALI_CTL_111_DATA     0x01000000
#define              DENALI_CTL_112_DATA     0x01010000   //0x0f0f0000
#define              DENALI_CTL_113_DATA     0x0000000f
#define              DENALI_CTL_114_DATA     0x19000700
#define              DENALI_CTL_115_DATA     0x00040003
#define              DENALI_CTL_116_DATA     0x020030b0
#define              DENALI_CTL_117_DATA     0x02000200
#define              DENALI_CTL_118_DATA     0x30b00200
#define              DENALI_CTL_119_DATA     0x0000f370
#define              DENALI_CTL_120_DATA     0x00020703
#define              DENALI_CTL_121_DATA     0x00140301
#define              DENALI_CTL_122_DATA     0x00000000
#define              DENALI_CTL_123_DATA     0x00000000
#define              DENALI_CTL_124_DATA     0x00001903
#define              DENALI_CTL_125_DATA     0x00000000
#define              DENALI_CTL_126_DATA     0x01000000   //0x00000000
#define              DENALI_CTL_127_DATA     0x00000001   //0x00000000
#define              DENALI_CTL_128_DATA     0x00000000
#define              DENALI_CTL_129_DATA     0x00000000
#define              DENALI_CTL_130_DATA     0x00000000
#define              DENALI_CTL_131_DATA     0x00000000
#define              DENALI_CTL_132_DATA     0x06010000
#define              DENALI_CTL_133_DATA     0x00000100
#define              DENALI_CTL_134_DATA     0x05050624
#define              DENALI_CTL_135_DATA     0x01010604
#define              DENALI_CTL_136_DATA     0x00000000





#define               DENALI_PHY_00_DATA     0x02800280
#define               DENALI_PHY_01_DATA     0x02800280
#define               DENALI_PHY_02_DATA     0x02800280
#define               DENALI_PHY_03_DATA     0x02800280
#define               DENALI_PHY_04_DATA     0x00000280
#define               DENALI_PHY_05_DATA     0x00000000
#define               DENALI_PHY_06_DATA     0x00000000
#define               DENALI_PHY_07_DATA     0x41420000
#define               DENALI_PHY_08_DATA     0x00000141
#define               DENALI_PHY_09_DATA     0x42999999
#define               DENALI_PHY_10_DATA     0x00014141
#define               DENALI_PHY_11_DATA     0x0f999999
#define               DENALI_PHY_12_DATA     0x80000101
#define               DENALI_PHY_13_DATA     0x00020880
#define               DENALI_PHY_14_DATA     0x04001000
#define               DENALI_PHY_15_DATA     0x00000042
#define               DENALI_PHY_16_DATA     0x00000000
#define               DENALI_PHY_17_DATA     0x00000000
#define               DENALI_PHY_18_DATA     0x00000000
#define               DENALI_PHY_19_DATA     0x00800000
#define               DENALI_PHY_20_DATA     0x00800080
#define               DENALI_PHY_21_DATA     0x00800080
#define               DENALI_PHY_22_DATA     0x00800080
#define               DENALI_PHY_23_DATA     0x00800080
#define               DENALI_PHY_24_DATA     0x00800080
#define               DENALI_PHY_25_DATA     0x00800080
#define               DENALI_PHY_26_DATA     0x00800080
#define               DENALI_PHY_27_DATA     0x00800080
#define               DENALI_PHY_28_DATA     0x00a00080
#define               DENALI_PHY_29_DATA     0x05000700
#define               DENALI_PHY_30_DATA     0x00000000
#define               DENALI_PHY_31_DATA     0x00000000
#define               DENALI_PHY_32_DATA     0x00000000
#define               DENALI_PHY_33_DATA     0x000001d0
#define               DENALI_PHY_34_DATA     0x0f030800
#define               DENALI_PHY_35_DATA     0x0f0c0404
#define               DENALI_PHY_36_DATA     0x00014004
#define               DENALI_PHY_37_DATA     0x00000118
#define               DENALI_PHY_38_DATA     0x04030803
#define               DENALI_PHY_39_DATA     0x00000000
#define               DENALI_PHY_40_DATA     0x000000ff
#define               DENALI_PHY_41_DATA     0x32103210
#define               DENALI_PHY_42_DATA     0x00000000
#define               DENALI_PHY_43_DATA     0x00100000
#define               DENALI_PHY_44_DATA     0x0c0000c2
#define               DENALI_PHY_45_DATA     0x003c0000
#define               DENALI_PHY_46_DATA     0x00002400
#define               DENALI_PHY_47_DATA     0x00000000
#define               DENALI_PHY_48_DATA     0x00000000
#define               DENALI_PHY_49_DATA     0x00000000
#define               DENALI_PHY_50_DATA     0x00000000
#define               DENALI_PHY_51_DATA     0x00000000
#define               DENALI_PHY_52_DATA     0x00000000
#define               DENALI_PHY_53_DATA     0x00000000
#define               DENALI_PHY_54_DATA     0x41000000
#define               DENALI_PHY_55_DATA     0x41804141
#define               DENALI_PHY_56_DATA     0x00804141
#define               DENALI_PHY_57_DATA     0x000000a0
#define               DENALI_PHY_58_DATA     0x01e00001
#define               DENALI_PHY_59_DATA     0x00000000
#define               DENALI_PHY_60_DATA     0x05030000
#define               DENALI_PHY_61_DATA     0x00000000
#define               DENALI_PHY_62_DATA     0x00000000
#define               DENALI_PHY_63_DATA     0x00000000
#define               DENALI_PHY_64_DATA     0x02800280
#define               DENALI_PHY_65_DATA     0x02800280
#define               DENALI_PHY_66_DATA     0x02800280
#define               DENALI_PHY_67_DATA     0x02800280
#define               DENALI_PHY_68_DATA     0x00000280
#define               DENALI_PHY_69_DATA     0x00000000
#define               DENALI_PHY_70_DATA     0x00000000
#define               DENALI_PHY_71_DATA     0x41420000
#define               DENALI_PHY_72_DATA     0x00000141
#define               DENALI_PHY_73_DATA     0x42999999
#define               DENALI_PHY_74_DATA     0x00014141
#define               DENALI_PHY_75_DATA     0x0f999999
#define               DENALI_PHY_76_DATA     0x80000101
#define               DENALI_PHY_77_DATA     0x00020880
#define               DENALI_PHY_78_DATA     0x04001000
#define               DENALI_PHY_79_DATA     0x00000042
#define               DENALI_PHY_80_DATA     0x00000000
#define               DENALI_PHY_81_DATA     0x00000000
#define               DENALI_PHY_82_DATA     0x00000000
#define               DENALI_PHY_83_DATA     0x00800000
#define               DENALI_PHY_84_DATA     0x00800080
#define               DENALI_PHY_85_DATA     0x00800080
#define               DENALI_PHY_86_DATA     0x00800080
#define               DENALI_PHY_87_DATA     0x00800080
#define               DENALI_PHY_88_DATA     0x00800080
#define               DENALI_PHY_89_DATA     0x00800080
#define               DENALI_PHY_90_DATA     0x00800080
#define               DENALI_PHY_91_DATA     0x00800080
#define               DENALI_PHY_92_DATA     0x00a00080
#define               DENALI_PHY_93_DATA     0x05000700
#define               DENALI_PHY_94_DATA     0x00000000
#define               DENALI_PHY_95_DATA     0x00000000
#define               DENALI_PHY_96_DATA     0x00000000
#define               DENALI_PHY_97_DATA     0x000001d0
#define               DENALI_PHY_98_DATA     0x0f030800
#define               DENALI_PHY_99_DATA     0x0f0c0404
#define              DENALI_PHY_100_DATA     0x00014004
#define              DENALI_PHY_101_DATA     0x00000118
#define              DENALI_PHY_102_DATA     0x04030803
#define              DENALI_PHY_103_DATA     0x00000000
#define              DENALI_PHY_104_DATA     0x000000ff
#define              DENALI_PHY_105_DATA     0x32103210
#define              DENALI_PHY_106_DATA     0x00000000
#define              DENALI_PHY_107_DATA     0x00100000
#define              DENALI_PHY_108_DATA     0x0c0000c5
#define              DENALI_PHY_109_DATA     0x003d0101
#define              DENALI_PHY_110_DATA     0x00032400
#define              DENALI_PHY_111_DATA     0x00000000
#define              DENALI_PHY_112_DATA     0x00000000
#define              DENALI_PHY_113_DATA     0x00000000
#define              DENALI_PHY_114_DATA     0x00000000
#define              DENALI_PHY_115_DATA     0x00000000
#define              DENALI_PHY_116_DATA     0x00000000
#define              DENALI_PHY_117_DATA     0x00000000
#define              DENALI_PHY_118_DATA     0x41000000
#define              DENALI_PHY_119_DATA     0x41804141
#define              DENALI_PHY_120_DATA     0x00804141
#define              DENALI_PHY_121_DATA     0x000000a0
#define              DENALI_PHY_122_DATA     0x01e00001
#define              DENALI_PHY_123_DATA     0x00000000
#define              DENALI_PHY_124_DATA     0x05030000
#define              DENALI_PHY_125_DATA     0x00000000
#define              DENALI_PHY_126_DATA     0x00000000
#define              DENALI_PHY_127_DATA     0x00000000
#define              DENALI_PHY_128_DATA     0x02800280
#define              DENALI_PHY_129_DATA     0x02800280
#define              DENALI_PHY_130_DATA     0x02800280
#define              DENALI_PHY_131_DATA     0x02800280
#define              DENALI_PHY_132_DATA     0x00000280
#define              DENALI_PHY_133_DATA     0x00000000
#define              DENALI_PHY_134_DATA     0x00000000
#define              DENALI_PHY_135_DATA     0x41420000
#define              DENALI_PHY_136_DATA     0x00000141
#define              DENALI_PHY_137_DATA     0x42999999
#define              DENALI_PHY_138_DATA     0x00014141
#define              DENALI_PHY_139_DATA     0x0f999999
#define              DENALI_PHY_140_DATA     0x80000101
#define              DENALI_PHY_141_DATA     0x00020880
#define              DENALI_PHY_142_DATA     0x04001000
#define              DENALI_PHY_143_DATA     0x00000042
#define              DENALI_PHY_144_DATA     0x00000000
#define              DENALI_PHY_145_DATA     0x00000000
#define              DENALI_PHY_146_DATA     0x00000000
#define              DENALI_PHY_147_DATA     0x00800000
#define              DENALI_PHY_148_DATA     0x00800080
#define              DENALI_PHY_149_DATA     0x00800080
#define              DENALI_PHY_150_DATA     0x00800080
#define              DENALI_PHY_151_DATA     0x00800080
#define              DENALI_PHY_152_DATA     0x00800080
#define              DENALI_PHY_153_DATA     0x00800080
#define              DENALI_PHY_154_DATA     0x00800080
#define              DENALI_PHY_155_DATA     0x00800080
#define              DENALI_PHY_156_DATA     0x00a00080
#define              DENALI_PHY_157_DATA     0x05000700
#define              DENALI_PHY_158_DATA     0x00000000
#define              DENALI_PHY_159_DATA     0x00000000
#define              DENALI_PHY_160_DATA     0x00000000
#define              DENALI_PHY_161_DATA     0x000001d0
#define              DENALI_PHY_162_DATA     0x0f030800
#define              DENALI_PHY_163_DATA     0x0f0c0404
#define              DENALI_PHY_164_DATA     0x00014004
#define              DENALI_PHY_165_DATA     0x00000118
#define              DENALI_PHY_166_DATA     0x04030803
#define              DENALI_PHY_167_DATA     0x00000000
#define              DENALI_PHY_168_DATA     0x000000ff
#define              DENALI_PHY_169_DATA     0x32103210
#define              DENALI_PHY_170_DATA     0x00000000
#define              DENALI_PHY_171_DATA     0x00100000
#define              DENALI_PHY_172_DATA     0x0c0000c5
#define              DENALI_PHY_173_DATA     0x003d0101
#define              DENALI_PHY_174_DATA     0x00032400
#define              DENALI_PHY_175_DATA     0x00000000
#define              DENALI_PHY_176_DATA     0x00000000
#define              DENALI_PHY_177_DATA     0x00000000
#define              DENALI_PHY_178_DATA     0x00000000
#define              DENALI_PHY_179_DATA     0x00000000
#define              DENALI_PHY_180_DATA     0x00000000
#define              DENALI_PHY_181_DATA     0x00000000
#define              DENALI_PHY_182_DATA     0x41000000
#define              DENALI_PHY_183_DATA     0x41804141
#define              DENALI_PHY_184_DATA     0x00804141
#define              DENALI_PHY_185_DATA     0x000000a0
#define              DENALI_PHY_186_DATA     0x01e00001
#define              DENALI_PHY_187_DATA     0x00000000
#define              DENALI_PHY_188_DATA     0x05030000
#define              DENALI_PHY_189_DATA     0x00000000
#define              DENALI_PHY_190_DATA     0x00000000
#define              DENALI_PHY_191_DATA     0x00000000
#define              DENALI_PHY_192_DATA     0x02800280
#define              DENALI_PHY_193_DATA     0x02800280
#define              DENALI_PHY_194_DATA     0x02800280
#define              DENALI_PHY_195_DATA     0x02800280
#define              DENALI_PHY_196_DATA     0x00000280
#define              DENALI_PHY_197_DATA     0x00000000
#define              DENALI_PHY_198_DATA     0x00000000
#define              DENALI_PHY_199_DATA     0x41420000
#define              DENALI_PHY_200_DATA     0x00000141
#define              DENALI_PHY_201_DATA     0x42999999
#define              DENALI_PHY_202_DATA     0x00014141
#define              DENALI_PHY_203_DATA     0x0f999999
#define              DENALI_PHY_204_DATA     0x80000101
#define              DENALI_PHY_205_DATA     0x00020880
#define              DENALI_PHY_206_DATA     0x04001000
#define              DENALI_PHY_207_DATA     0x00000042
#define              DENALI_PHY_208_DATA     0x00000000
#define              DENALI_PHY_209_DATA     0x00000000
#define              DENALI_PHY_210_DATA     0x00000000
#define              DENALI_PHY_211_DATA     0x00800000
#define              DENALI_PHY_212_DATA     0x00800080
#define              DENALI_PHY_213_DATA     0x00800080
#define              DENALI_PHY_214_DATA     0x00800080
#define              DENALI_PHY_215_DATA     0x00800080
#define              DENALI_PHY_216_DATA     0x00800080
#define              DENALI_PHY_217_DATA     0x00800080
#define              DENALI_PHY_218_DATA     0x00800080
#define              DENALI_PHY_219_DATA     0x00800080
#define              DENALI_PHY_220_DATA     0x00a00080
#define              DENALI_PHY_221_DATA     0x05000700
#define              DENALI_PHY_222_DATA     0x00000000
#define              DENALI_PHY_223_DATA     0x00000000
#define              DENALI_PHY_224_DATA     0x00000000
#define              DENALI_PHY_225_DATA     0x000001d0
#define              DENALI_PHY_226_DATA     0x0f030800
#define              DENALI_PHY_227_DATA     0x0f0c0404
#define              DENALI_PHY_228_DATA     0x00014004
#define              DENALI_PHY_229_DATA     0x00000118
#define              DENALI_PHY_230_DATA     0x04030803
#define              DENALI_PHY_231_DATA     0x00000000
#define              DENALI_PHY_232_DATA     0x000000ff
#define              DENALI_PHY_233_DATA     0x32103210
#define              DENALI_PHY_234_DATA     0x00000000
#define              DENALI_PHY_235_DATA     0x00100000
#define              DENALI_PHY_236_DATA     0x0c0000c5
#define              DENALI_PHY_237_DATA     0x003d0101
#define              DENALI_PHY_238_DATA     0x00032400
#define              DENALI_PHY_239_DATA     0x00000000
#define              DENALI_PHY_240_DATA     0x00000000
#define              DENALI_PHY_241_DATA     0x00000000
#define              DENALI_PHY_242_DATA     0x00000000
#define              DENALI_PHY_243_DATA     0x00000000
#define              DENALI_PHY_244_DATA     0x00000000
#define              DENALI_PHY_245_DATA     0x00000000
#define              DENALI_PHY_246_DATA     0x41000000
#define              DENALI_PHY_247_DATA     0x41804141
#define              DENALI_PHY_248_DATA     0x00804141
#define              DENALI_PHY_249_DATA     0x000000a0
#define              DENALI_PHY_250_DATA     0x01e00001
#define              DENALI_PHY_251_DATA     0x00000000
#define              DENALI_PHY_252_DATA     0x05030000
#define              DENALI_PHY_253_DATA     0x00000000
#define              DENALI_PHY_254_DATA     0x00000000
#define              DENALI_PHY_255_DATA     0x00000000
#define              DENALI_PHY_256_DATA     0x02800280
#define              DENALI_PHY_257_DATA     0x02800280
#define              DENALI_PHY_258_DATA     0x02800280
#define              DENALI_PHY_259_DATA     0x02800280
#define              DENALI_PHY_260_DATA     0x00000280
#define              DENALI_PHY_261_DATA     0x00000000
#define              DENALI_PHY_262_DATA     0x00000000
#define              DENALI_PHY_263_DATA     0x41420000
#define              DENALI_PHY_264_DATA     0x00000141
#define              DENALI_PHY_265_DATA     0x42999999
#define              DENALI_PHY_266_DATA     0x00014141
#define              DENALI_PHY_267_DATA     0x0f999999
#define              DENALI_PHY_268_DATA     0x80000101
#define              DENALI_PHY_269_DATA     0x00020880
#define              DENALI_PHY_270_DATA     0x04001000
#define              DENALI_PHY_271_DATA     0x00000042
#define              DENALI_PHY_272_DATA     0x00000000
#define              DENALI_PHY_273_DATA     0x00000000
#define              DENALI_PHY_274_DATA     0x00000000
#define              DENALI_PHY_275_DATA     0x00800000
#define              DENALI_PHY_276_DATA     0x00800080
#define              DENALI_PHY_277_DATA     0x00800080
#define              DENALI_PHY_278_DATA     0x00800080
#define              DENALI_PHY_279_DATA     0x00800080
#define              DENALI_PHY_280_DATA     0x00800080
#define              DENALI_PHY_281_DATA     0x00800080
#define              DENALI_PHY_282_DATA     0x00800080
#define              DENALI_PHY_283_DATA     0x00800080
#define              DENALI_PHY_284_DATA     0x00a00080
#define              DENALI_PHY_285_DATA     0x05000700
#define              DENALI_PHY_286_DATA     0x00000000
#define              DENALI_PHY_287_DATA     0x00000000
#define              DENALI_PHY_288_DATA     0x00000000
#define              DENALI_PHY_289_DATA     0x000001d0
#define              DENALI_PHY_290_DATA     0x0f030800
#define              DENALI_PHY_291_DATA     0x0f0c0404
#define              DENALI_PHY_292_DATA     0x00014004
#define              DENALI_PHY_293_DATA     0x00000118
#define              DENALI_PHY_294_DATA     0x04030803
#define              DENALI_PHY_295_DATA     0x00000000
#define              DENALI_PHY_296_DATA     0x000000ff
#define              DENALI_PHY_297_DATA     0x32103210
#define              DENALI_PHY_298_DATA     0x00000000
#define              DENALI_PHY_299_DATA     0x00100000
#define              DENALI_PHY_300_DATA     0x0c0000c2
#define              DENALI_PHY_301_DATA     0x003c0000
#define              DENALI_PHY_302_DATA     0x00012400
#define              DENALI_PHY_303_DATA     0x00000000
#define              DENALI_PHY_304_DATA     0x00000000
#define              DENALI_PHY_305_DATA     0x00000000
#define              DENALI_PHY_306_DATA     0x00000000
#define              DENALI_PHY_307_DATA     0x00000000
#define              DENALI_PHY_308_DATA     0x00000000
#define              DENALI_PHY_309_DATA     0x00000000
#define              DENALI_PHY_310_DATA     0x41000000
#define              DENALI_PHY_311_DATA     0x41804141
#define              DENALI_PHY_312_DATA     0x00804141
#define              DENALI_PHY_313_DATA     0x000000a0
#define              DENALI_PHY_314_DATA     0x01e00001
#define              DENALI_PHY_315_DATA     0x00000000
#define              DENALI_PHY_316_DATA     0x05030000
#define              DENALI_PHY_317_DATA     0x00000000
#define              DENALI_PHY_318_DATA     0x00000000
#define              DENALI_PHY_319_DATA     0x00000000
#define              DENALI_PHY_320_DATA     0x02800280
#define              DENALI_PHY_321_DATA     0x02800280
#define              DENALI_PHY_322_DATA     0x02800280
#define              DENALI_PHY_323_DATA     0x02800280
#define              DENALI_PHY_324_DATA     0x00000280
#define              DENALI_PHY_325_DATA     0x00000000
#define              DENALI_PHY_326_DATA     0x00000000
#define              DENALI_PHY_327_DATA     0x41420000
#define              DENALI_PHY_328_DATA     0x00000141
#define              DENALI_PHY_329_DATA     0x42999999
#define              DENALI_PHY_330_DATA     0x00014141
#define              DENALI_PHY_331_DATA     0x0f999999
#define              DENALI_PHY_332_DATA     0x80000101
#define              DENALI_PHY_333_DATA     0x00020880
#define              DENALI_PHY_334_DATA     0x04001000
#define              DENALI_PHY_335_DATA     0x00000042
#define              DENALI_PHY_336_DATA     0x00000000
#define              DENALI_PHY_337_DATA     0x00000000
#define              DENALI_PHY_338_DATA     0x00000000
#define              DENALI_PHY_339_DATA     0x00800000
#define              DENALI_PHY_340_DATA     0x00800080
#define              DENALI_PHY_341_DATA     0x00800080
#define              DENALI_PHY_342_DATA     0x00800080
#define              DENALI_PHY_343_DATA     0x00800080
#define              DENALI_PHY_344_DATA     0x00800080
#define              DENALI_PHY_345_DATA     0x00800080
#define              DENALI_PHY_346_DATA     0x00800080
#define              DENALI_PHY_347_DATA     0x00800080
#define              DENALI_PHY_348_DATA     0x00a00080
#define              DENALI_PHY_349_DATA     0x05000700
#define              DENALI_PHY_350_DATA     0x00000000
#define              DENALI_PHY_351_DATA     0x00000000
#define              DENALI_PHY_352_DATA     0x00000000
#define              DENALI_PHY_353_DATA     0x000001d0
#define              DENALI_PHY_354_DATA     0x0f030800
#define              DENALI_PHY_355_DATA     0x0f0c0404
#define              DENALI_PHY_356_DATA     0x00014004
#define              DENALI_PHY_357_DATA     0x00000118
#define              DENALI_PHY_358_DATA     0x04030803
#define              DENALI_PHY_359_DATA     0x00000000
#define              DENALI_PHY_360_DATA     0x000000ff
#define              DENALI_PHY_361_DATA     0x32103210
#define              DENALI_PHY_362_DATA     0x00000000
#define              DENALI_PHY_363_DATA     0x00100000
#define              DENALI_PHY_364_DATA     0x0c0000c4
#define              DENALI_PHY_365_DATA     0x003d0101
#define              DENALI_PHY_366_DATA     0x00032400
#define              DENALI_PHY_367_DATA     0x00000000
#define              DENALI_PHY_368_DATA     0x00000000
#define              DENALI_PHY_369_DATA     0x00000000
#define              DENALI_PHY_370_DATA     0x00000000
#define              DENALI_PHY_371_DATA     0x00000000
#define              DENALI_PHY_372_DATA     0x00000000
#define              DENALI_PHY_373_DATA     0x00000000
#define              DENALI_PHY_374_DATA     0x41000000
#define              DENALI_PHY_375_DATA     0x41804141
#define              DENALI_PHY_376_DATA     0x00804141
#define              DENALI_PHY_377_DATA     0x000000a0
#define              DENALI_PHY_378_DATA     0x01e00001
#define              DENALI_PHY_379_DATA     0x00000000
#define              DENALI_PHY_380_DATA     0x05030000
#define              DENALI_PHY_381_DATA     0x00000000
#define              DENALI_PHY_382_DATA     0x00000000
#define              DENALI_PHY_383_DATA     0x00000000
#define              DENALI_PHY_384_DATA     0x02800280
#define              DENALI_PHY_385_DATA     0x02800280
#define              DENALI_PHY_386_DATA     0x02800280
#define              DENALI_PHY_387_DATA     0x02800280
#define              DENALI_PHY_388_DATA     0x00000280
#define              DENALI_PHY_389_DATA     0x00000000
#define              DENALI_PHY_390_DATA     0x00000000
#define              DENALI_PHY_391_DATA     0x41420000
#define              DENALI_PHY_392_DATA     0x00000141
#define              DENALI_PHY_393_DATA     0x42999999
#define              DENALI_PHY_394_DATA     0x00014141
#define              DENALI_PHY_395_DATA     0x0f999999
#define              DENALI_PHY_396_DATA     0x80000101
#define              DENALI_PHY_397_DATA     0x00020880
#define              DENALI_PHY_398_DATA     0x04001000
#define              DENALI_PHY_399_DATA     0x00000042
#define              DENALI_PHY_400_DATA     0x00000000
#define              DENALI_PHY_401_DATA     0x00000000
#define              DENALI_PHY_402_DATA     0x00000000
#define              DENALI_PHY_403_DATA     0x00800000
#define              DENALI_PHY_404_DATA     0x00800080
#define              DENALI_PHY_405_DATA     0x00800080
#define              DENALI_PHY_406_DATA     0x00800080
#define              DENALI_PHY_407_DATA     0x00800080
#define              DENALI_PHY_408_DATA     0x00800080
#define              DENALI_PHY_409_DATA     0x00800080
#define              DENALI_PHY_410_DATA     0x00800080
#define              DENALI_PHY_411_DATA     0x00800080
#define              DENALI_PHY_412_DATA     0x00a00080
#define              DENALI_PHY_413_DATA     0x05000700
#define              DENALI_PHY_414_DATA     0x00000000
#define              DENALI_PHY_415_DATA     0x00000000
#define              DENALI_PHY_416_DATA     0x00000000
#define              DENALI_PHY_417_DATA     0x000001d0
#define              DENALI_PHY_418_DATA     0x0f030800
#define              DENALI_PHY_419_DATA     0x0f0c0404
#define              DENALI_PHY_420_DATA     0x00014004
#define              DENALI_PHY_421_DATA     0x00000118
#define              DENALI_PHY_422_DATA     0x04030803
#define              DENALI_PHY_423_DATA     0x00000000
#define              DENALI_PHY_424_DATA     0x000000ff
#define              DENALI_PHY_425_DATA     0x32103210
#define              DENALI_PHY_426_DATA     0x00000000
#define              DENALI_PHY_427_DATA     0x00100000
#define              DENALI_PHY_428_DATA     0x0c0000c5
#define              DENALI_PHY_429_DATA     0x003d0101
#define              DENALI_PHY_430_DATA     0x00032400
#define              DENALI_PHY_431_DATA     0x00000000
#define              DENALI_PHY_432_DATA     0x00000000
#define              DENALI_PHY_433_DATA     0x00000000
#define              DENALI_PHY_434_DATA     0x00000000
#define              DENALI_PHY_435_DATA     0x00000000
#define              DENALI_PHY_436_DATA     0x00000000
#define              DENALI_PHY_437_DATA     0x00000000
#define              DENALI_PHY_438_DATA     0x41000000
#define              DENALI_PHY_439_DATA     0x41804141
#define              DENALI_PHY_440_DATA     0x00804141
#define              DENALI_PHY_441_DATA     0x000000a0
#define              DENALI_PHY_442_DATA     0x01e00001
#define              DENALI_PHY_443_DATA     0x00000000
#define              DENALI_PHY_444_DATA     0x05030000
#define              DENALI_PHY_445_DATA     0x00000000
#define              DENALI_PHY_446_DATA     0x00000000
#define              DENALI_PHY_447_DATA     0x00000000
#define              DENALI_PHY_448_DATA     0x02800280
#define              DENALI_PHY_449_DATA     0x02800280
#define              DENALI_PHY_450_DATA     0x02800280
#define              DENALI_PHY_451_DATA     0x02800280
#define              DENALI_PHY_452_DATA     0x00000280
#define              DENALI_PHY_453_DATA     0x00000000
#define              DENALI_PHY_454_DATA     0x00000000
#define              DENALI_PHY_455_DATA     0x41420000
#define              DENALI_PHY_456_DATA     0x00000141
#define              DENALI_PHY_457_DATA     0x42999999
#define              DENALI_PHY_458_DATA     0x00014141
#define              DENALI_PHY_459_DATA     0x0f999999
#define              DENALI_PHY_460_DATA     0x80000101
#define              DENALI_PHY_461_DATA     0x00020880
#define              DENALI_PHY_462_DATA     0x04001000
#define              DENALI_PHY_463_DATA     0x00000042
#define              DENALI_PHY_464_DATA     0x00000000
#define              DENALI_PHY_465_DATA     0x00000000
#define              DENALI_PHY_466_DATA     0x00000000
#define              DENALI_PHY_467_DATA     0x00800000
#define              DENALI_PHY_468_DATA     0x00800080
#define              DENALI_PHY_469_DATA     0x00800080
#define              DENALI_PHY_470_DATA     0x00800080
#define              DENALI_PHY_471_DATA     0x00800080
#define              DENALI_PHY_472_DATA     0x00800080
#define              DENALI_PHY_473_DATA     0x00800080
#define              DENALI_PHY_474_DATA     0x00800080
#define              DENALI_PHY_475_DATA     0x00800080
#define              DENALI_PHY_476_DATA     0x00a00080
#define              DENALI_PHY_477_DATA     0x05000700
#define              DENALI_PHY_478_DATA     0x00000000
#define              DENALI_PHY_479_DATA     0x00000000
#define              DENALI_PHY_480_DATA     0x00000000
#define              DENALI_PHY_481_DATA     0x000001d0
#define              DENALI_PHY_482_DATA     0x0f030800
#define              DENALI_PHY_483_DATA     0x0f0c0404
#define              DENALI_PHY_484_DATA     0x00014004
#define              DENALI_PHY_485_DATA     0x00000118
#define              DENALI_PHY_486_DATA     0x04030803
#define              DENALI_PHY_487_DATA     0x00000000
#define              DENALI_PHY_488_DATA     0x000000ff
#define              DENALI_PHY_489_DATA     0x32103210
#define              DENALI_PHY_490_DATA     0x00000000
#define              DENALI_PHY_491_DATA     0x00100000
#define              DENALI_PHY_492_DATA     0x0c0000c4
#define              DENALI_PHY_493_DATA     0x003d0101
#define              DENALI_PHY_494_DATA     0x00032400
#define              DENALI_PHY_495_DATA     0x00000000
#define              DENALI_PHY_496_DATA     0x00000000
#define              DENALI_PHY_497_DATA     0x00000000
#define              DENALI_PHY_498_DATA     0x00000000
#define              DENALI_PHY_499_DATA     0x00000000
#define              DENALI_PHY_500_DATA     0x00000000
#define              DENALI_PHY_501_DATA     0x00000000
#define              DENALI_PHY_502_DATA     0x41000000
#define              DENALI_PHY_503_DATA     0x41804141
#define              DENALI_PHY_504_DATA     0x00804141
#define              DENALI_PHY_505_DATA     0x000000a0
#define              DENALI_PHY_506_DATA     0x01e00001
#define              DENALI_PHY_507_DATA     0x00000000
#define              DENALI_PHY_508_DATA     0x05030000
#define              DENALI_PHY_509_DATA     0x00000000
#define              DENALI_PHY_510_DATA     0x00000000
#define              DENALI_PHY_511_DATA     0x00000000
#define              DENALI_PHY_512_DATA     0x02800280
#define              DENALI_PHY_513_DATA     0x02800280
#define              DENALI_PHY_514_DATA     0x02800280
#define              DENALI_PHY_515_DATA     0x02800280
#define              DENALI_PHY_516_DATA     0x00000280
#define              DENALI_PHY_517_DATA     0x00000000
#define              DENALI_PHY_518_DATA     0x00000000
#define              DENALI_PHY_519_DATA     0x41420000
#define              DENALI_PHY_520_DATA     0x00000141
#define              DENALI_PHY_521_DATA     0x42999999
#define              DENALI_PHY_522_DATA     0x00014141
#define              DENALI_PHY_523_DATA     0x0f999999
#define              DENALI_PHY_524_DATA     0x80000101
#define              DENALI_PHY_525_DATA     0x00020880
#define              DENALI_PHY_526_DATA     0x04001000
#define              DENALI_PHY_527_DATA     0x00000042
#define              DENALI_PHY_528_DATA     0x00000000
#define              DENALI_PHY_529_DATA     0x00000000
#define              DENALI_PHY_530_DATA     0x00000000
#define              DENALI_PHY_531_DATA     0x00800000
#define              DENALI_PHY_532_DATA     0x00800080
#define              DENALI_PHY_533_DATA     0x00800080
#define              DENALI_PHY_534_DATA     0x00800080
#define              DENALI_PHY_535_DATA     0x00800080
#define              DENALI_PHY_536_DATA     0x00800080
#define              DENALI_PHY_537_DATA     0x00800080
#define              DENALI_PHY_538_DATA     0x00800080
#define              DENALI_PHY_539_DATA     0x00800080
#define              DENALI_PHY_540_DATA     0x00a00080
#define              DENALI_PHY_541_DATA     0x05000700
#define              DENALI_PHY_542_DATA     0x00000000
#define              DENALI_PHY_543_DATA     0x00000000
#define              DENALI_PHY_544_DATA     0x00000000
#define              DENALI_PHY_545_DATA     0x000001d0
#define              DENALI_PHY_546_DATA     0x0f030800
#define              DENALI_PHY_547_DATA     0x0f0c0404
#define              DENALI_PHY_548_DATA     0x00014004
#define              DENALI_PHY_549_DATA     0x00000118
#define              DENALI_PHY_550_DATA     0x04030803
#define              DENALI_PHY_551_DATA     0x00000000
#define              DENALI_PHY_552_DATA     0x000000ff
#define              DENALI_PHY_553_DATA     0x32103210
#define              DENALI_PHY_554_DATA     0x00000000
#define              DENALI_PHY_555_DATA     0x00100000
#define              DENALI_PHY_556_DATA     0x0c0000c6
#define              DENALI_PHY_557_DATA     0x003d0101
#define              DENALI_PHY_558_DATA     0x00002500
#define              DENALI_PHY_559_DATA     0x00000000
#define              DENALI_PHY_560_DATA     0x00000000
#define              DENALI_PHY_561_DATA     0x00000000
#define              DENALI_PHY_562_DATA     0x00000000
#define              DENALI_PHY_563_DATA     0x00000000
#define              DENALI_PHY_564_DATA     0x00000000
#define              DENALI_PHY_565_DATA     0x00000000
#define              DENALI_PHY_566_DATA     0x41000000
#define              DENALI_PHY_567_DATA     0x41804141
#define              DENALI_PHY_568_DATA     0x00804141
#define              DENALI_PHY_569_DATA     0x000000a0
#define              DENALI_PHY_570_DATA     0x01e00001
#define              DENALI_PHY_571_DATA     0x00000000
#define              DENALI_PHY_572_DATA     0x05030000
#define              DENALI_PHY_573_DATA     0x00000000
#define              DENALI_PHY_574_DATA     0x00000000
#define              DENALI_PHY_575_DATA     0x00000000
#define              DENALI_PHY_576_DATA     0x00000100
#define              DENALI_PHY_577_DATA     0x00000100
#define              DENALI_PHY_578_DATA     0x00000100
#define              DENALI_PHY_579_DATA     0x00000100
#define              DENALI_PHY_580_DATA     0x00000100
#define              DENALI_PHY_581_DATA     0x00000100
#define              DENALI_PHY_582_DATA     0x00000100
#define              DENALI_PHY_583_DATA     0x00000100
#define              DENALI_PHY_584_DATA     0x00000100
#define              DENALI_PHY_585_DATA     0x00000100
#define              DENALI_PHY_586_DATA     0x00000000
#define              DENALI_PHY_587_DATA     0x00000000
#define              DENALI_PHY_588_DATA     0x04001000
#define              DENALI_PHY_589_DATA     0x00000042
#define              DENALI_PHY_590_DATA     0x01220000
#define              DENALI_PHY_591_DATA     0x00030064
#define              DENALI_PHY_592_DATA     0x0000dd00
#define              DENALI_PHY_593_DATA     0x000000dd
#define              DENALI_PHY_594_DATA     0x00dd00dd
#define              DENALI_PHY_595_DATA     0x0000e28a
#define              DENALI_PHY_596_DATA     0x0000e28a
#define              DENALI_PHY_597_DATA     0x0000e28a
#define              DENALI_PHY_598_DATA     0x0000e28a
#define              DENALI_PHY_599_DATA     0x00dd00dd
#define              DENALI_PHY_600_DATA     0x00000004
#define              DENALI_PHY_601_DATA     0x00000000
#define              DENALI_PHY_602_DATA     0x0000050a
#define              DENALI_PHY_603_DATA     0x8e24c24b
#define              DENALI_PHY_604_DATA     0x01000000

typedef enum{
	DDR_CLK_200MHZ,
	DDR_CLK_400MHZ,
	DDR_CLK_667MHZ,
	DDR_CLK_800MHZ
}DDR_CLK_t;

typedef enum{
	ROW_16,
	ROW_15,
	ROW_14,
	ROW_13
}DDR_ROW_NUM_t;

typedef enum{
	WIDTH_x16,
	WIDTH_x8
}DDR_DRAM_WIDTH_t;

typedef enum{
	WIDTH_32BIT = 4,
	WIDTH_64BIT = 8
}WIDTH_BIT;


extern const unsigned int MCU_CTRL_VALUE_800M[];
extern const unsigned int MCU_PHY_VALUE_800M[];
extern unsigned int read_reg(unsigned int addr);
extern void clr_set_reg( unsigned int offset, unsigned int mask_value, unsigned int value);
extern void check_reg(unsigned int addr, unsigned int offset);
extern unsigned char DDR_start_address_bist(unsigned char end);
extern unsigned int  DDR_start_data_bist(unsigned int start,unsigned char end);
extern int DDR_Init(DDR_CLK_t ddr_freq, DDR_ROW_NUM_t row_num,\
		DDR_DRAM_WIDTH_t dram_width, unsigned char ecc,WIDTH_BIT DIS_NUM);
extern int DDR_TraverseTest(void);
extern void mcu_margin_test(WIDTH_BIT DIS_NUM);
extern void DDR_addrmap_demo(void);
extern void DDR_hard_lvling(WIDTH_BIT DIS_NUM);
extern int DDR_entry(DDR_CLK_t ddr_freq, DDR_ROW_NUM_t row_num,\
		   DDR_DRAM_WIDTH_t dram_width, unsigned char ecc,WIDTH_BIT DIS_NUM);
extern void DDR_regwr_enable();

//#define DEBUG_DDR
//#define DEBUG_DDR_PHY
#define DEBUG_LVL
#define DDR_BIST_CTL



//#define DDR_HIGH_T      // when T>80 C, then DDR tref should change to 3.9us

/*
 * DDR3 space round from low 2GB to high 2GB
 */
//#define DDR3_ADDRESS_REMAP

#undef DDR_MARGIN_CTL
#ifdef DDR_MARGIN_CTL
#define CLK_WRDQS_DELAY_CTL
#define CLK_WRDQZ_DELAY_CTL
#define RDDQS_DQZ_RISE_DELAY_CTL
#define RDDQS_DQZ_FALL_DELAY_CTL
#endif





