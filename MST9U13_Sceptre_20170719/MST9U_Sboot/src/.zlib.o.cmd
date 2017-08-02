cmd_src/zlib.o := aeon-gcc  -Wp,-MD,src/.zlib.o.d   -std=gnu99 -Iinclude -Iout    -include include/autoconf.h  -D_GNU_SOURCE -DNDEBUG    -D"BB_VER=KBUILD_STR(1.0.0)" -DBB_BT=AUTOCONF_TIMESTAMP   -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1  -fno-pic -Os -G 0 -march=aeonR2 -EL     -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(zlib)"  -D"KBUILD_MODNAME=KBUILD_STR(zlib)" -c -o src/zlib.o src/zlib.c

deps_src/zlib.o := \
  src/zlib.c \
  src/zlib.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/string.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/_ansi.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/newlib.h \
    $(wildcard include/config/external/ccs.h) \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/sys/config.h \
    $(wildcard include/config/h//.h) \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/machine/ieeefp.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/sys/reent.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/_ansi.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/sys/_types.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/sys/lock.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/include/stddef.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/../../../../aeon/include/sys/string.h \

src/zlib.o: $(deps_src/zlib.o)

$(deps_src/zlib.o):
