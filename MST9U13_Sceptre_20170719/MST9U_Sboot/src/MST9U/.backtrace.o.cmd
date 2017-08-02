cmd_src/MST9U/backtrace.o := aeon-gcc  -Wp,-MD,src/MST9U/.backtrace.o.d   -std=gnu99 -Iinclude -Iout    -include include/autoconf.h  -D_GNU_SOURCE -DNDEBUG    -D"BB_VER=KBUILD_STR(1.0.0)" -DBB_BT=AUTOCONF_TIMESTAMP   -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1  -fno-pic -Os -G 0 -march=aeonR2 -EL -I/cygdrive/d/mst9u13_a0/mst9u_sboot/inc/MST9U/board -I/cygdrive/d/mst9u13_a0/mst9u_sboot/src     -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(backtrace)"  -D"KBUILD_MODNAME=KBUILD_STR(backtrace)" -c -o src/MST9U/backtrace.o src/MST9U/backtrace.c

deps_src/MST9U/backtrace.o := \
  src/MST9U/backtrace.c \
  src/MST9U/stdio.h \
  /opt/aeon/lib/gcc/aeon/4.1.2/include/stdarg.h \

src/MST9U/backtrace.o: $(deps_src/MST9U/backtrace.o)

$(deps_src/MST9U/backtrace.o):
