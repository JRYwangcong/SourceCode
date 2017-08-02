cmd_src/MST9U/sboot.lds := aeon-gcc  -E -Wp,-MD,src/MST9U/.sboot.lds.d   -std=gnu99 -Iinclude -Iout    -include include/autoconf.h  -D_GNU_SOURCE -DNDEBUG    -D"BB_VER=KBUILD_STR(1.0.0)" -DBB_BT=AUTOCONF_TIMESTAMP  -P -C -Uaeon -D__ASSEMBLY__ -o src/MST9U/sboot.lds src/MST9U/sboot.lds.S ;  ln -sf src/MST9U/sboot.lds /cygdrive/d/mst9u13_a0/mst9u_sboot/sboot_ldscript

deps_src/MST9U/sboot.lds := \
  src/MST9U/sboot.lds.S \

src/MST9U/sboot.lds: $(deps_src/MST9U/sboot.lds)

$(deps_src/MST9U/sboot.lds):
