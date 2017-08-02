#ifndef _DDR_BD_
#define _DDR_BD_

#define DRAM_BUS_16B        0               // MIU0 16x
#define DRAM_BUS_32B        1               // MIU0 32x
#define DRAM_BUS_64B        2               // MIU0 32x + MIU1 32x

#if (CHIP_ID==MST9U)
#define DRAM_BUS_WIDTH      DRAM_BUS_32B
#elif (CHIP_ID==MST9U2)
#define DRAM_BUS_WIDTH      DRAM_BUS_64B
#elif (CHIP_ID==MST9U3)
#define DRAM_BUS_WIDTH      DRAM_BUS_32B
#else
#define DRAM_BUS_WIDTH      DRAM_BUS_32B
#endif

#ifndef DDR2_TYPE
#define DDR2_TYPE           DDR2_TYPE_ETRON
#endif
#if (DRAM_BUS_WIDTH == DRAM_BUS_16B)
#define DRAM_BUS_MASK       0x00
#else
#define DRAM_BUS_MASK       0x04
#endif

#if (CHIP_ID==MST9U2) // MST9U2
    #if (MIU_SPEED == DDR3_1866MHz_e)
        // 1866
        #if 0 // SEC
        #include "ddr_bd_uhd_se13u01_sec_1866.h"
        #warning "RUN MiuInit2_1866Mhz: ddr_bd_uhd_se13u01_sec_1866 "
        #else // Mstar
        #include "ddr_bd_mst192a_a01a_hynix_1866.h"
        #warning "RUN MiuInit2_1866Mhz: ddr_bd_mst192a_a01a_hynix_1866 "
        #endif

    #else
        // 2133
        #if 0

        #else // MStar
        #include "ddr_bd_mst192a_a01a_hynix_2133.h"
        #warning "RUN MiuInit2_2133Mhz: ddr_bd_mst192a_a01a_hynix_2133 "
        #endif

    #endif

#elif (CHIP_ID==MST9U3) //MST9U3

    extern void MiuInit_1333Mhz_Etron(void);
    extern void MiuInit_1333Mhz_Winbond(void);
    extern void MiuInit_1333Mhz_ESMT(void);

    #if 1   //Mstar BGA
      #define MST9U3_BGA    1
      #define MST9U3_QFP    0
      #if (MIU_SPEED == DDR2_1333MHz_e)
        #warning "RUN MiuInit_1333Mhz: BGA ddr_bd_mst203a_a01a/b_1333 "
      #else
        #warning "RUN MiuInit_1240Mhz: BGA ddr_bd_mst203a_a01a/b_1240 "
      #endif
    #else //MStar QFP
      #define MST9U3_BGA    0
      #define MST9U3_QFP    1
      #if (MIU_SPEED == DDR2_1333MHz_e)
        #warning "RUN MiuInit_1333Mhz: QFP ddr_bd_mst203b/c_a01a_1333 "
      #else
        #warning "RUN MiuInit_1240Mhz: QFP ddr_bd_mst203b/c_a01a_1240 "
      #endif
    #endif

    #if (DDR2_TYPE == DDR2_TYPE_ESMT)
    #include "ddr_bd_mst203a_a01a_esmt_1333.h"
    //#warning "RUN MiuInit_1333Mhz: ddr_bd_mst203a_a01a_esmt_1333 "
    #elif (DDR2_TYPE == DDR2_TYPE_WINBOND)
    #include "ddr_bd_mst203a_a01a_winbond_1333.h"
    //#warning "RUN MiuInit_1333Mhz: ddr_bd_mst203a_a01a_winbond_1333 "
    #elif (DDR2_TYPE == DDR2_TYPE_ETRON)
    #include "ddr_bd_mst203a_a01a_etron_1333.h"
    //#warning "RUN MiuInit_1333Mhz: ddr_bd_mst203a_a01a_etron_1333 "
    #endif

#else // MST9U
    #if (MIU_SPEED == DDR3_1866MHz_e)
    // 1866
        #if 0 // SEC
        #include "ddr_bd_uhd_mst9u01v_sec_1866.h"
        #warning "RUN MiuInit_1866Mhz: ddr_bd_uhd_mst9u01v_sec_1866 "
        #elif 0 // SEC_C_Version
        #include "ddr_bd_uhd_mst9u01v_C_sec_1866.h"
        #warning "RUN MiuInit_1866Mhz: ddr_bd_uhd_mst9u01v_C_sec_1866 "
        #elif 0 // MStar
        #include "ddr_bd_mst182a_a01a_hynix_1866.h"
        #warning "RUN MiuInit_1866Mhz: ddr_bd_mst182a_a01a_hynix_1866 "
        #else // Mstar
        #include "ddr_bd_mst182a_a01b_hynix_1866.h"
        #warning "RUN MiuInit_1866Mhz: ddr_bd_mst182a_a01b_hynix_1866 "
        #endif
    #else
    // 2133
        #if 0 // Mstar
        #include "ddr_bd_mst182a_a01a_hynix_2133.h"
        #warning "RUN MiuInit_2133Mhz: ddr_bd_mst182a_a01a_hynix_2133 "
        #else // Mstar
        #include "ddr_bd_mst182a_a01b_hynix_2133.h"
        #warning "RUN MiuInit_2133Mhz: ddr_bd_mst182a_a01b_hynix_2133 "
        #endif

    #endif

#endif

#endif
