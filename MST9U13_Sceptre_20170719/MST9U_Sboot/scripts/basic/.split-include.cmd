cmd_scripts/basic/split-include := gcc -Wp,-MD,scripts/basic/.split-include.d  -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer       -o scripts/basic/split-include scripts/basic/split-include.c   

deps_scripts/basic/split-include := \
  scripts/basic/split-include.c \
    $(wildcard include/config/.h) \
  /usr/include/sys/stat.h \
  /usr/include/_ansi.h \
  /usr/include/newlib.h \
    $(wildcard include/config/external/ccs.h) \
  /usr/include/sys/config.h \
    $(wildcard include/config/h//.h) \
    $(wildcard include/config/en2k.h) \
  /usr/include/machine/ieeefp.h \
  /usr/include/sys/features.h \
  /usr/include/cygwin/config.h \
    $(wildcard include/config/h.h) \
    $(wildcard include/config/ernal/stat64.h) \
  /usr/include/time.h \
  /usr/include/_ansi.h \
  /usr/include/sys/reent.h \
  /usr/include/sys/_types.h \
  /usr/include/machine/_types.h \
  /usr/include/machine/_default_types.h \
  /usr/include/sys/lock.h \
  /usr/lib/gcc/i686-pc-cygwin/3.4.4/include/stddef.h \
  /usr/include/machine/time.h \
  /usr/include/sys/types.h \
    $(wildcard include/config//sockets.h) \
  /usr/include/machine/types.h \
  /usr/include/cygwin/types.h \
  /usr/include/stdint.h \
  /usr/include/endian.h \
  /usr/include/bits/endian.h \
  /usr/include/byteswap.h \
  /usr/include/sys/sysmacros.h \
  /usr/include/cygwin/time.h \
  /usr/include/signal.h \
  /usr/include/sys/signal.h \
  /usr/include/cygwin/signal.h \
  /usr/include/cygwin/stat.h \
  /usr/include/ctype.h \
  /usr/include/errno.h \
  /usr/include/sys/errno.h \
  /usr/include/fcntl.h \
  /usr/include/sys/fcntl.h \
  /usr/include/sys/_default_fcntl.h \
  /usr/include/sys/time.h \
  /usr/include/cygwin/sys_time.h \
  /usr/include/sys/select.h \
  /usr/include/sys/cdefs.h \
  /usr/include/stdio.h \
  /usr/lib/gcc/i686-pc-cygwin/3.4.4/include/stdarg.h \
  /usr/include/sys/stdio.h \
  /usr/include/stdlib.h \
  /usr/include/machine/stdlib.h \
  /usr/include/alloca.h \
  /usr/include/cygwin/stdlib.h \
  /usr/include/cygwin/wait.h \
  /usr/include/string.h \
  /usr/include/sys/string.h \
  /usr/include/unistd.h \
  /usr/include/sys/unistd.h \
  /usr/include/getopt.h \

scripts/basic/split-include: $(deps_scripts/basic/split-include)

$(deps_scripts/basic/split-include):
