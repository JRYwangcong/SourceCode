cmd_scripts/basic/docproc := gcc -Wp,-MD,scripts/basic/.docproc.d  -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer       -o scripts/basic/docproc scripts/basic/docproc.c   

deps_scripts/basic/docproc := \
  scripts/basic/docproc.c \
  /usr/include/stdio.h \
    $(wildcard include/config/en2k.h) \
  /usr/include/_ansi.h \
  /usr/include/newlib.h \
    $(wildcard include/config/external/ccs.h) \
  /usr/include/sys/config.h \
    $(wildcard include/config/h//.h) \
  /usr/include/machine/ieeefp.h \
  /usr/include/sys/features.h \
  /usr/include/cygwin/config.h \
    $(wildcard include/config/h.h) \
    $(wildcard include/config/ernal/stat64.h) \
  /usr/lib/gcc/i686-pc-cygwin/3.4.4/include/stddef.h \
  /usr/lib/gcc/i686-pc-cygwin/3.4.4/include/stdarg.h \
  /usr/include/sys/reent.h \
  /usr/include/_ansi.h \
  /usr/include/sys/_types.h \
  /usr/include/machine/_types.h \
  /usr/include/machine/_default_types.h \
  /usr/include/sys/lock.h \
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
  /usr/include/sys/cdefs.h \
  /usr/include/stdlib.h \
  /usr/include/machine/stdlib.h \
  /usr/include/alloca.h \
  /usr/include/cygwin/stdlib.h \
  /usr/include/cygwin/wait.h \
  /usr/include/string.h \
  /usr/include/sys/string.h \
  /usr/include/ctype.h \
  /usr/include/unistd.h \
  /usr/include/sys/unistd.h \
  /usr/include/getopt.h \
  /usr/lib/gcc/i686-pc-cygwin/3.4.4/include/limits.h \
  /usr/lib/gcc/i686-pc-cygwin/3.4.4/include/syslimits.h \
  /usr/include/limits.h \
  /usr/include/features.h \
  /usr/include/sys/wait.h \
  /usr/include/sys/resource.h \
  /usr/include/sys/time.h \
  /usr/include/cygwin/sys_time.h \
  /usr/include/sys/select.h \
  /usr/include/time.h \
  /usr/include/machine/time.h \
  /usr/include/cygwin/time.h \
  /usr/include/signal.h \
  /usr/include/sys/signal.h \
  /usr/include/cygwin/signal.h \

scripts/basic/docproc: $(deps_scripts/basic/docproc)

$(deps_scripts/basic/docproc):
