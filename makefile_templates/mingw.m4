dnl
dnl WLA-specific variables
define(`HOST_OS', WIN32)dnl
dnl A User should define values for these macros which are consistent with the
dnl host OS where the assemblers will live.
dnl You should define target_cpu, out_suffix, and opcode_prereq on the command
dnl line.
define(`TARGET_CPU', target_cpu)dnl
define(`OUT_SUFFIX', out_suffix)dnl
define(`OPCODE_PREREQ', opcode_prereq)dnl
dnl
dnl Defines which control build/make script generation
define(`IF_ARGCHECK', `divert(-1)dnl')dnl
define(`ENDIF_ARGCHECK', `divert dnl')dnl
dnl
dnl Basic commands on your host system
define(`SHEBANG', `#!/bin/bash')dnl
define(`CP', cp $1 $2)dnl
define(`CHDIR', cd $1)dnl
define(`RM', rm -f $1)dnl
define(`EXEC', ./$1)dnl How do you execute EXEs?
define(`SHEXEC', ./$1)dnl How do you execute one shell script from another?
define(`ECHO', echo $1)dnl
define(`CHMOD', `dnl')dnl
define(`MAKE', make $1)dnl $2 argument omitted- mingw make doesn't support 
dnl parallel builds.
define(`EXPORT', `export `$1'='$2)dnl
define(`INSTALL', install $1)dnl
define(`EXIT', exit $1)dnl
dnl
dnl OS-specific syntax
define(`COMMENT', `#')dnl What is the line comment character?
define(`SH_EXTENSION', `.sh')dnl What extension do shell scripts/command files have?
define(`OBJ_EXTENSION', `.o')dnl
define(`EXE_EXTENSION', `.exe')dnl Dot required here, since Unix doesn't use 
dnl extensions for exes...
define(`PATH_SEPARATOR', `/')dnl
dnl
dnl Describe your C compiler
define(`CC', gcc)dnl
define(`CFLAGS_DEFINE', `-D')dnl
define(`CLAGS_COMPILE', `-c')dnl
define(`CFLAGS_OPT', `-O3')dnl
define(`CFLAGS_ANSI', `-ansi -pedantic-errors -Wall -Wextra')dnl
define(`CFLAGS_OTHER', `')dnl
dnl
dnl Describe your Linker
define(`LD', gcc)dnl
define(`LDFLAGS_OUT', `-o')dnl
define(`LDFLAGS_OTHER', `')dnl
dnl
dnl Macros created from the above...
define(`CFLAGS', CLAGS_COMPILE CFLAGS_OPT CFLAGS_ANSI CFLAGS_OTHER)dnl
define(`LDFLAGS', LDFLAGS_OTHER)dnl
define(`HOSTFLAGS', `-DMSDOS -DWIN32')dnl
define(`WLAFLAGS', HOSTFLAGS` '-D`'TARGET_CPU)dnl
