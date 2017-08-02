cmd_src/MSDecompress.o := aeon-gcc  -Wp,-MD,src/.MSDecompress.o.d   -std=gnu99 -Iinclude -Iout    -include include/autoconf.h  -D_GNU_SOURCE -DNDEBUG    -D"BB_VER=KBUILD_STR(1.0.0)" -DBB_BT=AUTOCONF_TIMESTAMP   -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1  -fno-pic -Os -G 0 -march=aeonR2 -EL     -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(MSDecompress)"  -D"KBUILD_MODNAME=KBUILD_STR(MSDecompress)" -c -o src/MSDecompress.o src/MSDecompress.c

deps_src/MSDecompress.o := \
  src/MSDecompress.c \
    $(wildcard include/config/mstar/mst9u.h) \
    $(wildcard include/config/a/for/crc/check.h) \
    $(wildcard include/config/sboot/use/bdma/to/check/crc.h) \
    $(wildcard include/config/a/to/check/crc.h) \
    $(wildcard include/config/a/move/code.h) \
    $(wildcard include/config/usb/host/dual/image.h) \
    $(wildcard include/config/booting/from/otp/with/pm51.h) \
    $(wildcard include/config/booting/from/ext/spi/with/pm51.h) \
    $(wildcard include/config/non/os.h) \
    $(wildcard include/config/mstar/macaw12.h) \
    $(wildcard include/config/secure/boot.h) \
    $(wildcard include/config/mstar/titania4.h) \
    $(wildcard include/config/mstar/titania7.h) \
    $(wildcard include/config/mstar/titania8.h) \
    $(wildcard include/config/mstar/titania9.h) \
    $(wildcard include/config/mstar/uranus4.h) \
    $(wildcard include/config/mstar/kronus.h) \
    $(wildcard include/config/mstar/kaiserin.h) \
    $(wildcard include/config/dram/selfrefresh.h) \
  src/MST9U/drvGlobal.h \
    $(wildcard include/config/usb.h) \
  src/MST9U/datatype.h \
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
  src/zlib.h \
  src/ms_decompress_priv.h \
    $(wildcard include/config/dram/size/mb/relocate.h) \
  src/version.h \
  src/MST9U\hwreg_MST9U.h \
    $(wildcard include/config/ddr/ssc.h) \
    $(wildcard include/config/print/msg.h) \
    $(wildcard include/config/cpu/345mhz.h) \
    $(wildcard include/config/cpu/320mhz.h) \
    $(wildcard include/config/cpu/288mhz.h) \
    $(wildcard include/config/cpu/240mhz.h) \
    $(wildcard include/config/cpu/216mhz.h) \
    $(wildcard include/config/cpu/192mhz.h) \
    $(wildcard include/config/cpu/144mhz.h) \
    $(wildcard include/config/cpu/108mhz.h) \
    $(wildcard include/config/one/shot.h) \
  src/MST9U\/c_riubase.h \
  src/MST9U\/c_pm_riubase.h \
  src/MST9U/hwreg.h \
  src/MST9U/hwreg_MST9U.h \
  src/7types.h \
  src/7alloc.h \
  src/lzmadec.h \

src/MSDecompress.o: $(deps_src/MSDecompress.o)

$(deps_src/MSDecompress.o):
