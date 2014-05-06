dnl
dnl WLA-specific variables
define(`HOST_OS', UNIX)dnl
dnl A User should define values for these macros which are consistent with the
dnl host OS where the assemblers will live.
dnl You should define target_cpu, out_suffix, and opcode_prereq on the command
dnl line.
define(`TARGET_CPU', target_cpu)dnl
define(`OUT_SUFFIX', out_suffix)dnl
define(`OPCODE_PREREQ', opcode_prereq)dnl
dnl
dnl Basic commands on your host system
define(`CC', gcc)dnl
define(`LD', gcc)dnl
define(`CP', cp)dnl
define(`RM', rm)dnl
define(`ECHO', echo)dnl
define(`MAKE', make)dnl
dnl
dnl OS-specific syntax
define(`OBJ_EXTENSION', `o')dnl
define(`EXE_EXTENSION', `')dnl
define(`CP_ARGUMENTS', `$1 $2')dnl
define(`SHEBANG', `#!/bin/bash')dnl
dnl
dnl Describe your C compiler's flags
define(`CFLAGS_DEFINE', `-D')dnl
define(`CLAGS_COMPILE', `-c')dnl
define(`CFLAGS_OPT', `-O3')dnl
define(`CFLAGS_ANSI', `-ansi -pedantic-errors -Wall -Wextra')dnl
define(`CFLAGS_OTHER', `')dnl
dnl
dnl Describe your Linker's flags
define(`LDFLAGS_OUT', `-o')dnl
define(`LDFLAGS_OTHER', `')dnl
dnl
dnl Macros created from the above...
define(`CFLAGS', CLAGS_COMPILE CFLAGS_OPT CFLAGS_ANSI CFLAGS_OTHER)dnl
define(`LDFLAGS', LDFLAGS_OTHER)dnl
define(`WLA_FLAGS', CFLAGS_DEFINE`'HOST_OS CFLAGS_DEFINE`'TARGET_CPU)dnl
dnl define(`CP_COMMAND', CP CP_ARGUMENTS($1, $2))dnl
