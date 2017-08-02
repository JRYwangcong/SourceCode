#ifndef _DDR_BD_MST192A_A01A_HYNIX_
#define _DDR_BD_MST192A_A01A_HYNIX_

// -------------------------- MIU 0 --------------------------
// REG_101100, 8b: 00 [12:9]
#define MIU_0_ODT               0x1400

// REG_10111B, 8b: 36
#define MIU_0_CLK               0x4000

// REG_10111C, 8b: 38
#define MIU_0_DQ_DQS_L          0x2267

// REG_10111D, 8b: 3A
#define MIU_0_RDQSM_CMD_L       0x0002

// REG_10111E, 8b: 3C
#define MIU_0_DQS_CS_SKEW       0x1522

// REG_10111F, 8b: 3E
#define MIU_0_DQS_CS_CLK_SKEW   0x1055

// REG_101124, 8b: 48
#define MIU_0_DQ_DQS_H          0x2266

// REG_101125, 8b: 4A
#define MIU_0_RDQSM_CMD_H       0x6062

// REG_101126, 8b: 4C
#define MIU_0_RDQSM_SKEW_H      0x0022

// REG_101128, 8b: 50
#define MIU_0_CSZ_SKEW          0x1111

// REG_101129, 8b: 52  <= CMD Scan
#define MIU_0_CS                0x0021

// REG_10112D, 8b: 5A
#define MIU_0_DRV_NP            0x6666

// REG_10112E, 8b: 5C
#define MIU_0_DRV_N             0x6666

// REG_10112F, 8b: 5E
#define MIU_0_DRV_P             0x6666

// REG_101136, 8b: 6C
#define MIU_0_RDQS_L            0x7766

// REG_101137, 8b: 6E
#define MIU_0_RDQS_H            0x5566

// REG_10113A, 8b: 74
#define MIU_0_TRIG_LVL          0x6666


#endif
