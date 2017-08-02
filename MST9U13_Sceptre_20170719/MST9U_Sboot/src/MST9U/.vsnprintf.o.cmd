cmd_src/MST9U/vsnprintf.o := aeon-gcc  -Wp,-MD,src/MST9U/.vsnprintf.o.d   -std=gnu99 -Iinclude -Iout    -include include/autoconf.h  -D_GNU_SOURCE -DNDEBUG    -D"BB_VER=KBUILD_STR(1.0.0)" -DBB_BT=AUTOCONF_TIMESTAMP   -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1  -fno-pic -Os -G 0 -march=aeonR2 -EL -I/cygdrive/d/mst9u13_a0/mst9u_sboot/inc/MST9U/board -I/cygdrive/d/mst9u13_a0/mst9u_sboot/src     -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(vsnprintf)"  -D"KBUILD_MODNAME=KBUILD_STR(vsnprintf)" -c -o src/MST9U/vsnprintf.o src/MST9U/vsnprintf.c

deps_src/MST9U/vsnprintf.o := \
  src/MST9U/vsnprintf.c \
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

src/MST9U/vsnprintf.o: $(deps_src/MST9U/vsnprintf.o)

$(deps_src/MST9U/vsnprintf.o):
