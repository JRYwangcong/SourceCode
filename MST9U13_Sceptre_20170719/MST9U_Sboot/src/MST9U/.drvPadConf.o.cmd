cmd_src/MST9U/drvPadConf.o := aeon-gcc  -Wp,-MD,src/MST9U/.drvPadConf.o.d   -std=gnu99 -Iinclude -Iout    -include include/autoconf.h  -D_GNU_SOURCE -DNDEBUG    -D"BB_VER=KBUILD_STR(1.0.0)" -DBB_BT=AUTOCONF_TIMESTAMP   -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1  -fno-pic -Os -G 0 -march=aeonR2 -EL -I/cygdrive/d/mst9u13_a0/mst9u_sboot/inc/MST9U/board -I/cygdrive/d/mst9u13_a0/mst9u_sboot/src     -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(drvPadConf)"  -D"KBUILD_MODNAME=KBUILD_STR(drvPadConf)" -c -o src/MST9U/drvPadConf.o src/MST9U/drvPadConf.c

deps_src/MST9U/drvPadConf.o := \
  src/MST9U/drvPadConf.c \
    $(wildcard include/config/mstar/mst9u.h) \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/stdio.h \
    $(wildcard include/config//types//.h) \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/_ansi.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/newlib.h \
    $(wildcard include/config/external/ccs.h) \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/sys/config.h \
    $(wildcard include/config/h//.h) \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/machine/ieeefp.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/include/stddef.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/include/stdarg.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/sys/reent.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/_ansi.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/sys/_types.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/sys/lock.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/sys/types.h \
    $(wildcard include/config//sockets.h) \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/machine/_types.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/include/limits.h \
    $(wildcard include/config/.h) \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/machine/types.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/sys/features.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/sys/stdio.h \
  src/MST9U/datatype.h \
  /cygdrive/d/mst9u13_a0/mst9u_sboot/inc/MST9U/board/Board.h \
    $(wildcard include/config/e.h) \
    $(wildcard include/config/04.h) \
    $(wildcard include/config/08.h) \
    $(wildcard include/config/16.h) \
    $(wildcard include/config/32.h) \
    $(wildcard include/config/od.h) \
    $(wildcard include/config/mlink.h) \
    $(wildcard include/config/uart1/debug.h) \
    $(wildcard include/config/titania3/aeon/51/mau/128mb.h) \
    $(wildcard include/config/mstar/mst9u/bd/mstxxxa/dxxa/s.h) \
    $(wildcard include/config/i2c.h) \
    $(wildcard include/config/two/flash/bak.h) \
  /cygdrive/d/mst9u13_a0/mst9u_sboot/inc/MST9U/board/Enum_Device.h \
  /cygdrive/d/mst9u13_a0/mst9u_sboot/inc/MST9U/board/BD_MSTxxxA_DxxA_S.h \
    $(wildcard include/config/i2c/highspeed.h) \
    $(wildcard include/config/ddc/ram.h) \
    $(wildcard include/config/scart/video.h) \
    $(wildcard include/config/second/demod.h) \
    $(wildcard include/config/power/saving.h) \
    $(wildcard include/config/power/good/detect.h) \
    $(wildcard include/config/power/saving/sif.h) \
    $(wildcard include/config/power/saving/vdmvd.h) \
    $(wildcard include/config/power/saving/dpms.h) \
    $(wildcard include/config/pws.h) \
    $(wildcard include/config/dip/pws.h) \
    $(wildcard include/config/dip/monitor.h) \
    $(wildcard include/config/ssc.h) \
    $(wildcard include/config/lvdstorgb/converter.h) \
    $(wildcard include/config/hkmcu/icache/bypass.h) \
    $(wildcard include/config/hkmcu/code/ecc.h) \
    $(wildcard include/config/dpwm/function.h) \
    $(wildcard include/config/kpdchan/1.h) \
    $(wildcard include/config/kpdchan/2.h) \
    $(wildcard include/config/kpdchan/3.h) \
    $(wildcard include/config/kpdchan/4.h) \
    $(wildcard include/config/uart0.h) \
    $(wildcard include/config/uart0/interrupt.h) \
    $(wildcard include/config/uart1.h) \
    $(wildcard include/config/uart1/interrupt.h) \
    $(wildcard include/config/piu/uart0.h) \
    $(wildcard include/config/piu/uart0/interrupt.h) \
    $(wildcard include/config/piu/uart1.h) \
    $(wildcard include/config/piu/uart1/interrupt.h) \
  /cygdrive/d/mst9u13_a0/mst9u_sboot/inc/MST9U/board/chip/MST6932XPT.h \
    $(wildcard include/config//cdn/in/pcm2.h) \
    $(wildcard include/config//in/in/pcm.h) \
    $(wildcard include/config//out/in/pcm.h) \
    $(wildcard include/config/if/in/in/pcm.h) \
    $(wildcard include/config/if/out/in/pcm.h) \
    $(wildcard include/config//in/ddcr2.h) \
    $(wildcard include/config/2/in/gpio14.h) \
    $(wildcard include/config/3/in/gpio15.h) \
  /cygdrive/d/mst9u13_a0/mst9u_sboot/inc/MST9U/board/mmap/MMap.h \
    $(wildcard include/config/mstar/mst9u/mmap/512mb.h) \
    $(wildcard include/config/mstar/mst9u/mmap/256mb.h) \
  /cygdrive/d/mst9u13_a0/mst9u_sboot/inc/MST9U/board/mmap/mmap_512mb.h \
    $(wildcard include/config/mstar/titania/bd/t2/lg/mecury/board/atsc/1.h) \
    $(wildcard include/config/mstar/titania/bd/t2/lg/minerava/board/atsc/1.h) \
  src/MST9U/c_riubase.h \
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
    $(wildcard include/config/one/shot.h) \
  src/MST9U/c_pm_riubase.h \
  /cygdrive/d/mst9u13_a0/mst9u_sboot/inc/MST9U/board/chip/bond.h \
  /cygdrive/d/mst9u13_a0/mst9u_sboot/inc/MST9U/board/Board.h \
  src/MST9U/drvGlobal.h \
  src/MST9U/drvPadConf.h \

src/MST9U/drvPadConf.o: $(deps_src/MST9U/drvPadConf.o)

$(deps_src/MST9U/drvPadConf.o):
