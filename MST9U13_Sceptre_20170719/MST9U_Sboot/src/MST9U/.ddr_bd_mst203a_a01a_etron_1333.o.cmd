cmd_src/MST9U/ddr_bd_mst203a_a01a_etron_1333.o := aeon-gcc  -Wp,-MD,src/MST9U/.ddr_bd_mst203a_a01a_etron_1333.o.d   -std=gnu99 -Iinclude -Iout    -include include/autoconf.h  -D_GNU_SOURCE -DNDEBUG    -D"BB_VER=KBUILD_STR(1.0.0)" -DBB_BT=AUTOCONF_TIMESTAMP   -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1  -fno-pic -Os -G 0 -march=aeonR2 -EL -I/cygdrive/d/mst9u13_a0/mst9u_sboot/inc/MST9U/board -I/cygdrive/d/mst9u13_a0/mst9u_sboot/src     -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(ddr_bd_mst203a_a01a_etron_1333)"  -D"KBUILD_MODNAME=KBUILD_STR(ddr_bd_mst203a_a01a_etron_1333)" -c -o src/MST9U/ddr_bd_mst203a_a01a_etron_1333.o src/MST9U/ddr_bd_mst203a_a01a_etron_1333.c

deps_src/MST9U/ddr_bd_mst203a_a01a_etron_1333.o := \
  src/MST9U/ddr_bd_mst203a_a01a_etron_1333.c \
  src/MST9U/hwreg_MST9U.h \
    $(wildcard include/config/ddr/ssc.h) \
    $(wildcard include/config/print/msg.h) \
    $(wildcard include/config/dram/selfrefresh.h) \
    $(wildcard include/config/usb/host/dual/image.h) \
    $(wildcard include/config/usb.h) \
    $(wildcard include/config/cpu/345mhz.h) \
    $(wildcard include/config/cpu/320mhz.h) \
    $(wildcard include/config/cpu/288mhz.h) \
    $(wildcard include/config/cpu/240mhz.h) \
    $(wildcard include/config/cpu/216mhz.h) \
    $(wildcard include/config/cpu/192mhz.h) \
    $(wildcard include/config/cpu/144mhz.h) \
    $(wildcard include/config/cpu/108mhz.h) \
    $(wildcard include/config/.h) \
    $(wildcard include/config/one/shot.h) \
  src/MST9U/c_riubase.h \
  src/MST9U/c_pm_riubase.h \

src/MST9U/ddr_bd_mst203a_a01a_etron_1333.o: $(deps_src/MST9U/ddr_bd_mst203a_a01a_etron_1333.o)

$(deps_src/MST9U/ddr_bd_mst203a_a01a_etron_1333.o):
