#ifndef _DDR_BD_MST203A_A01A_ESMT_
#define _DDR_BD_MST203A_A01A_ESMT_

// -------------------------- MIU 0 --------------------------
// REG_101100, 8b: 00 [12:9]
#define MIU_0_ODT               0x1400

// REG_101105, 8b: 0A
#define MIU_0_CLK_PHASE         0x2244

// REG_10110D, 8b: 1A
#define MIU_0_SEL_ODT_MODE      0x8000

// REG_10111B, 8b: 36
#define MIU_0_CLK               0x4000

// REG_10111C, 8b: 38
#define MIU_0_DQ_DQS_L          0x0055

// REG_10111D, 8b: 3A
#define MIU_0_RDQSM_CMD_L       0x0005

// REG_10111E, 8b: 3C
#define MIU_0_DQS_CS_SKEW       0x1611

// REG_10111F, 8b: 3E
#define MIU_0_DQS_CS_CLK_SKEW   0x0066

// REG_101124, 8b: 48
#define MIU_0_DQ_DQS_H          0x2266

// REG_101125, 8b: 4A
#define MIU_0_RDQSM_CMD_H       0x2022

// REG_101126, 8b: 4C
#define MIU_0_RDQSM_SKEW_H      0x0044

// REG_101127, 8b: 4E
#define MIU_ODT_SKEW            0x0011

// REG_101128, 8b: 50
#define MIU_0_CSZ_SKEW          0x2222

// REG_101129, 8b: 52  <= CMD Scan
#define MIU_0_CS                0x0000

// REG_10112D, 8b: 5A
#define MIU_0_DRV_NP            0x4444

// REG_10112E, 8b: 5C
#define MIU_0_DRV_N             0x4444

// REG_10112F, 8b: 5E
#define MIU_0_DRV_P             0x4444

// REG_101136, 8b: 6C
#define MIU_0_RDQS_L            0x0000

// REG_101137, 8b: 6E
#define MIU_0_RDQS_H            0x7799

// REG_10113A, 8b: 74
#define MIU_0_TRIG_LVL          0x8882



// -------------------------- MIU 1 --------------------------
// REG_101140, 8b: 80 [12:9]
#define MIU_1_ODT               0x1400

// REG_101145, 8b: 8A
#define MIU_1_CLK_PHASE         0x2244

// REG_10114D, 8b: 9A
#define MIU_1_SEL_ODT_MODE      0x8000

// REG_10115B, 8b: B6
#define MIU_1_CLK               0x4000

// REG_10115C, 8b: B8
#define MIU_1_DQ_DQS_L          0x0055

// REG_10115D, 8b: BA
#define MIU_1_RDQSM_CMD_L       0x0005

// REG_10115E, 8b: BC
#define MIU_1_DQS_CS_SKEW       0x1611

// REG_10115F, 8b: BE
#define MIU_1_DQS_CS_CLK_SKEW   0x0066

// REG_101168, 8b: D0
#define MIU_1_CSZ_SKEW          0x2222

// REG_101169, 8b: D2  <= CMD Scan
#define MIU_1_CS                0x0000

// REG_10116D, 8b: DA
#define MIU_1_DRV_NP            0x4444

// REG_10116E, 8b: DC
#define MIU_1_DRV_N             0x4444

// REG_10116F, 8b: DE
#define MIU_1_DRV_P             0x4444

// REG_101176, 8b: EC
#define MIU_1_RDQS_L            0x0000

// REG_101177, 8b: EE
#define MIU_1_RDQS_H            0x7799

// REG_10117A, 8b: F4
#define MIU_1_TRIG_LVL          0x8882

// REG_101203, 8b: 06
#define MIU_CYCLE_PERIOD        0x0540

// REG_101206, 8b: 0C
#define MIU_TWLR_TIMING         0x55A6

// REG_101207, 8b: 0E
#define MIU_TRFC_TIMING         0x2057

// REG_101208, 8b: 10
#define MIU_MODE_REG            0x0E73

// REG_101214, 8b: 28
#define MIU_ODT_DELAY           0x4020

#endif
