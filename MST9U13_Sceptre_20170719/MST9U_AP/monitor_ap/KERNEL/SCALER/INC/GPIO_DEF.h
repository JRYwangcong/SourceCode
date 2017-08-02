#ifndef _GPIO_DEF_H
#define _GPIO_DEF_H


#define _REG_GPIO0_OUT      REG_000420  //GPIO00 ~ GPIO06 output value
#define _REG_GPIO0_OEZ      REG_000422  //GPIO00 ~ GPIO06 OEN control
#define _REG_GPIO0_IN       REG_000424  //GPIO00 ~ GPIO06 input value
#define _REG_GPIO1_OUT      REG_000426  //GPIO10 ~ GPIO11 output value
#define _REG_GPIO1_OEZ      REG_000428  //GPIO10 ~ GPIO11 OEN control
#define _REG_GPIO1_IN       REG_00042A  //GPIO10 ~ GPIO11 input value
#define _REG_GPIO2_OUT      REG_00042C  //GPIO20 ~ GPIO27 output value
#define _REG_GPIO2_OEZ      REG_00042E  //GPIO20 ~ GPIO27 OEN control
#define _REG_GPIO2_IN       REG_000432  //GPIO20 ~ GPIO27 input value
#define _REG_GPIO3_OUT      0           //GPIO30 ~ GPIO37 output value
#define _REG_GPIO3_OEZ      0           //GPIO30 ~ GPIO37 OEN control
#define _REG_GPIO3_IN       0           //GPIO30 ~ GPIO37 input value
#define _REG_GPIO4_OUT      REG_000434  //GPIO40 ~ GPIO42 output value
#define _REG_GPIO4_OEZ      REG_000436  //GPIO40 ~ GPIO42 OEN control
#define _REG_GPIO4_IN       REG_000438  //GPIO40 ~ GPIO42 input value
#define _REG_GPIO5_OUT      REG_00043A  //GPIO50 ~ GPIO56 output value
#define _REG_GPIO5_OEZ      REG_00043B  //GPIO50 ~ GPIO56 OEN control
#define _REG_GPIO5_IN       REG_00043C  //GPIO50 ~ GPIO56 input value
#define _REG_GPIO6_OUT      REG_00043D  //GPIO60 ~ GPIO67 output value
#define _REG_GPIO6_OEZ      REG_00043E  //GPIO60 ~ GPIO67 OEN control
#define _REG_GPIO6_IN       REG_00043F  //GPIO60 ~ GPIO67 input value
#define _REG_GPIO7_OUT      REG_000440  //GPIO70 ~ GPIO77 output value
#define _REG_GPIO7_OEZ      REG_000442  //GPIO70 ~ GPIO77 OEN control
#define _REG_GPIO7_IN       REG_000444  //GPIO70 ~ GPIO77 input value
#define _REG_GPIO8_OUT      REG_000446  //GPIO80 ~ GPIO87 output value
#define _REG_GPIO8_OEZ      REG_000447  //GPIO80 ~ GPIO87 OEN control
#define _REG_GPIO8_IN       REG_000448  //GPIO80 ~ GPIO87 input value
#define _REG_GPIO_X0_OUT    REG_000480  //GPIO_X00 ~ GPIO_X04 output value
#define _REG_GPIO_X0_OEZ    REG_000481  //GPIO_X00 ~ GPIO_X04 OEN control
#define _REG_GPIO_X0_IN     REG_000482  //GPIO_X00 ~ GPIO_X04 input value
#define _REG_GPIO_X1_OUT    REG_000484  //GPIO_X10 ~ GPIO_X14 output value
#define _REG_GPIO_X1_OEZ    REG_000485  //GPIO_X10 ~ GPIO_X14 OEN control
#define _REG_GPIO_X1_IN     REG_000486  //GPIO_X10 ~ GPIO_X14 input value
#define _REG_GPIO_X2_OUT    REG_000488  //GPIO_X20 ~ GPIO_X24 output value
#define _REG_GPIO_X2_OEZ    REG_000489  //GPIO_X20 ~ GPIO_X24 OEN control
#define _REG_GPIO_X2_IN     REG_00048A  //GPIO_X20 ~ GPIO_X24 input value
#define _REG_GPIO_X3_OUT    REG_00048C  //GPIO_X30 ~ GPIO_X34 output value
#define _REG_GPIO_X3_OEZ    REG_00048D  //GPIO_X30 ~ GPIO_X34 OEN control
#define _REG_GPIO_X3_IN     REG_00048E  //GPIO_X30 ~ GPIO_X34 input value
#define _REG_GPIO_X4_OUT    REG_000490  //GPIO_X40 ~ GPIO_X44 output value
#define _REG_GPIO_X4_OEZ    REG_000491  //GPIO_X40 ~ GPIO_X44 OEN control
#define _REG_GPIO_X4_IN     REG_000492  //GPIO_X40 ~ GPIO_X44 input value
#define _REG_GPIO_X5_OUT    REG_000494  //GPIO_X50 ~ GPIO_X54 output value
#define _REG_GPIO_X5_OEZ    REG_000495  //GPIO_X50 ~ GPIO_X54 OEN control
#define _REG_GPIO_X5_IN     REG_000496  //GPIO_X50 ~ GPIO_X54 input value
//MOD
#define _REG_GPIOM0_OUT     0           //GPIO_MOD0 ~ GPIO_MOD7 output value
#define _REG_GPIOM0_OEZ     0           //GPIO_MOD0 ~ GPIO_MOD7 OEN control
#define _REG_GPIOM0_IN      0           //GPIO_MOD0 ~ GPIO_MOD7 input value
#define _REG_GPIOM1_OUT     0           //GPIO_MOD8 ~ GPIO_MOD15 output value
#define _REG_GPIOM1_OEZ     0           //GPIO_MOD8 ~ GPIO_MOD15 OEN control
#define _REG_GPIOM1_IN      0           //GPIO_MOD8 ~ GPIO_MOD15 input value
#define _REG_GPIOM2_OUT     0           //GPIO_MOD16 ~ GPIO_MOD19 output value
#define _REG_GPIOM2_OEZ     0           //GPIO_MOD16 ~ GPIO_MOD19 OEN control
#define _REG_GPIOM2_IN      0           //GPIO_MOD16 ~ GPIO_MOD19 input value
// LED
#define _REG_GPIOL_SEL_L    0           //GPIO_LED0 ~ GPIO_LED15 enable ,0: enable GPIO function ,1: disable GPIO function
#define _REG_GPIOL_SEL_H    0           //GPIO_LED0 ~ GPIO_LED15 enable ,0: enable GPIO function ,1: disable GPIO function
#define _REG_GPIOL_OUT_L    0           //GPIO_LED0 ~ GPIO_LED15 output value
#define _REG_GPIOL_OUT_H    0           //GPIO_LED0 ~ GPIO_LED15 output value
#define _REG_GPIOL_OEZ_L    0           //GPIO_LED0 ~ GPIO_LED15 OEN control
#define _REG_GPIOL_OEZ_H    0           //GPIO_LED0 ~ GPIO_LED15 OEN control
#define _REG_GPIOL_IN_L     0           //GPIO_LED0 ~ GPIO_LED15 input value
#define _REG_GPIOL_IN_H     0           //GPIO_LED0 ~ GPIO_LED15 input value
// SAR
#define _REG_GPIOSAR_OUT    REG_000454  //[7:4] GPIO_SAR0 ~ GPIO_SAR3 output value
#define _REG_GPIOSAR_OEZ    REG_000455  //[3:0] GPIO_SAR0 ~ GPIO_SAR3 OEN control
#define _REG_GPIOSAR_IN     REG_000455  //[7:4] GPIO_SAR0 ~ GPIO_SAR3 input value
#define _REG_SAR_OUT        REG_000454  //[7:4] GPIO_SAR0 ~ GPIO_SAR3 output value
#define _REG_SAR_OEZ        REG_000455  //[3:0] GPIO_SAR0 ~ GPIO_SAR3 OEN control
#define _REG_SAR_IN         REG_000455  //[7:4] GPIO_SAR0 ~ GPIO_SAR3 input value
// AUDIO
#define _REG_GPIOAU_SEL_L   REG_000458  //GPIO_AU0 ~ GPIO_AU7 enable ,0: enable GPIO function ,1: disable GPIO function
#define _REG_GPIOAU_OUT_L   REG_00045A  //GPIO_AU0 ~ GPIO_AU7 output value
#define _REG_GPIOAU_OEZ_L   REG_00045C  //GPIO_AU0 ~ GPIO_AU7 OEN control
#define _REG_GPIOAU_IN_L    REG_00045E  //GPIO_AU0 ~ GPIO_AU7 input value
#define _REG_GPIOAU_SEL_H   REG_000459  //GPIO_AU8 enable ,0: enable GPIO function ,1: disable GPIO function
#define _REG_GPIOAU_OUT_H   REG_00045B  //GPIO_AU8 output value
#define _REG_GPIOAU_OEZ_H   REG_00045D  //GPIO_AU8 OEN control
#define _REG_GPIOAU_IN_H    REG_00045F  //GPIO_AU8 input value

#endif
