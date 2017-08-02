cmd_src/MST9U/setSPI.o := aeon-gcc  -Wp,-MD,src/MST9U/.setSPI.o.d   -std=gnu99 -Iinclude -Iout    -include include/autoconf.h  -D_GNU_SOURCE -DNDEBUG    -D"BB_VER=KBUILD_STR(1.0.0)" -DBB_BT=AUTOCONF_TIMESTAMP   -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1  -fno-pic -Os -G 0 -march=aeonR2 -EL -I/cygdrive/d/mst9u13_a0/mst9u_sboot/inc/MST9U/board -I/cygdrive/d/mst9u13_a0/mst9u_sboot/src     -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(setSPI)"  -D"KBUILD_MODNAME=KBUILD_STR(setSPI)" -c -o src/MST9U/setSPI.o src/MST9U/setSPI.c

deps_src/MST9U/setSPI.o := \
  src/MST9U/setSPI.c \

src/MST9U/setSPI.o: $(deps_src/MST9U/setSPI.o)

$(deps_src/MST9U/setSPI.o):
