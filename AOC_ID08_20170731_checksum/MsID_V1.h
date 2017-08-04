
typedef enum{	// 110419 Modify
    TSUM_F = 0x10,
    TSUM_E = 0x12,
    TSUM_G = 0x14,
    TSUM_K = 0x16,
    TSUM_L = 0x18,
    TSUM_M = 0x1A,
    TSUM_N = 0x1C,
    TSUM_P = 0x1C,
    TSUM_Q = 0x1E,
    TSUM_R = 0x20,
    TSUM_T = 0x22,

    UpdateIDLibary = 0xFE
}RefScalarType;

extern BYTE msReadID(void);
