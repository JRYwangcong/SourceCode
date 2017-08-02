cmd_scripts/kconfig/zconf.tab.o := gcc -Wp,-MD,scripts/kconfig/.zconf.tab.o.d  -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer   -DKBUILD_NO_NLS   -Iscripts/kconfig -c -o scripts/kconfig/zconf.tab.o scripts/kconfig/zconf.tab.c

deps_scripts/kconfig/zconf.tab.o := \
  scripts/kconfig/zconf.tab.c \
    $(wildcard include/config/oca.h) \
  /usr/include/ctype.h \
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
  /usr/lib/gcc/i686-pc-cygwin/3.4.4/include/stdarg.h \
  /usr/include/stdio.h \
  /usr/lib/gcc/i686-pc-cygwin/3.4.4/include/stddef.h \
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
  /usr/lib/gcc/i686-pc-cygwin/3.4.4/include/stdbool.h \
  scripts/kconfig/lkc.h \
  scripts/kconfig/expr.h \
  scripts/kconfig/lkc_proto.h \
  scripts/kconfig/zconf.hash.c \
  scripts/kconfig/lex.zconf.c \
    $(wildcard include/config/st.h) \
    $(wildcard include/config/wrap.h) \
  /usr/include/errno.h \
  /usr/include/sys/errno.h \
  /usr/lib/gcc/i686-pc-cygwin/3.4.4/include/limits.h \
  /usr/lib/gcc/i686-pc-cygwin/3.4.4/include/syslimits.h \
  /usr/include/limits.h \
  /usr/include/features.h \
  /usr/include/unistd.h \
  /usr/include/sys/unistd.h \
  /usr/include/getopt.h \
  scripts/kconfig/util.c \
  scripts/kconfig/confdata.c \
    $(wildcard include/config/notimestamp.h) \
  /usr/include/sys/stat.h \
  /usr/include/time.h \
  /usr/include/machine/time.h \
  /usr/include/cygwin/time.h \
  /usr/include/signal.h \
  /usr/include/sys/signal.h \
  /usr/include/cygwin/signal.h \
  /usr/include/cygwin/stat.h \
  scripts/kconfig/expr.c \
  scripts/kconfig/symbol.c \
  /usr/include/regex.h \
  /usr/include/sys/utsname.h \
  scripts/kconfig/menu.c \

scripts/kconfig/zconf.tab.o: $(deps_scripts/kconfig/zconf.tab.o)

$(deps_scripts/kconfig/zconf.tab.o):
