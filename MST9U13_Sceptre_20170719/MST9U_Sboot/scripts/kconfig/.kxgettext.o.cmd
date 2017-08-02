cmd_scripts/kconfig/kxgettext.o := gcc -Wp,-MD,scripts/kconfig/.kxgettext.o.d  -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer   -DKBUILD_NO_NLS    -c -o scripts/kconfig/kxgettext.o scripts/kconfig/kxgettext.c

deps_scripts/kconfig/kxgettext.o := \
  scripts/kconfig/kxgettext.c \
  /usr/include/stdlib.h \
  /usr/include/machine/ieeefp.h \
  /usr/include/_ansi.h \
  /usr/include/newlib.h \
    $(wildcard include/config/external/ccs.h) \
  /usr/include/sys/config.h \
    $(wildcard include/config/h//.h) \
    $(wildcard include/config/en2k.h) \
  /usr/include/sys/features.h \
  /usr/include/cygwin/config.h \
    $(wildcard include/config/h.h) \
    $(wildcard include/config/ernal/stat64.h) \
  /usr/lib/gcc/i686-pc-cygwin/3.4.4/include/stddef.h \
  /usr/include/sys/reent.h \
  /usr/include/_ansi.h \
  /usr/include/sys/_types.h \
  /usr/include/machine/_types.h \
  /usr/include/machine/_default_types.h \
  /usr/include/sys/lock.h \
  /usr/include/machine/stdlib.h \
  /usr/include/alloca.h \
  /usr/include/cygwin/stdlib.h \
  /usr/include/cygwin/wait.h \
  /usr/include/string.h \
  /usr/include/sys/cdefs.h \
  /usr/include/sys/string.h \
  scripts/kconfig/lkc.h \
  scripts/kconfig/expr.h \
  /usr/include/stdio.h \
  /usr/lib/gcc/i686-pc-cygwin/3.4.4/include/stdarg.h \
  /usr/include/sys/types.h \
    $(wildcard include/config//sockets.h) \
  /usr/include/machine/types.h \
  /usr/include/cygwin/types.h \
  /usr/include/stdint.h \
  /usr/include/endian.h \
    $(wildcard include/config/.h) \
  /usr/include/bits/endian.h \
  /usr/include/byteswap.h \
  /usr/include/sys/sysmacros.h \
  /usr/include/sys/stdio.h \
  /usr/lib/gcc/i686-pc-cygwin/3.4.4/include/stdbool.h \
  scripts/kconfig/lkc_proto.h \

scripts/kconfig/kxgettext.o: $(deps_scripts/kconfig/kxgettext.o)

$(deps_scripts/kconfig/kxgettext.o):
