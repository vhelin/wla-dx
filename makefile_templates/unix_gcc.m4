define(`HOST_OS', UNIX)dnl
define(`CC', gcc)dnl
define(`LD', gcc)dnl
define(`CP', cp)dnl
define(`RM', rm)dnl
define(`CFLAGS_DEFINE', `-D')
define(`CFLAGS_OPT', `-O3')dnl
define(`CFLAGS_ANSI', `-ansi -pedantic-errors -Wall -Wextra')dnl
define(`CFLAGS_OTHER', `')dnl
define(`LDFLAGS', `')dnl
define(`TARGET_CPU', target_cpu)dnl
dnl
dnl Macros created from the above...
define(`CFLAGS', `-c' CFLAGS_OPT CFLAGS_ANSI CFLAGS_OTHER)dnl
define(`WLA_FLAGS', CFLAGS_DEFINE`'HOST_OS CFLAGS_DEFINE`'TARGET_CPU)dnl
