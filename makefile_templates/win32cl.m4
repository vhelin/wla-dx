dnl A User should define values for these macros which are consistent with the
dnl host OS where the assemblers will live. At a minimum, your host environment
dnl should have support for directory trees, a command shell, recursive script 
dnl calling and inherited environment variables, and an ANSI-C standard library 
dnl that supports SEEK_END meaningfully (cross building should work fine). 
dnl
dnl m4 defines() which must be defined, or are safe to leave undefined/have 
dnl sane defaults are noted.
dnl
dnl WLA-specific variables. You should define target_cpu, out_suffix, and 
dnl opcode_prereq on the m4 command line.
define(`HOST_OS', )dnl Define to a unique string for your host environment.
define(`TARGET_CPU', target_cpu)dnl
define(`OUT_SUFFIX', out_suffix)dnl Most-likely aliases out_suffix.
define(`OPCODE_PREREQ', opcode_prereq)dnl Each target requires an opcode 
dnl decoding table to be generated beforehand.
dnl
dnl Defines which control build/make script generation. It is safe to divert(-1)
dnl this if your host environment does not support parallel builds.
define(`IF_ARGCHECK', `divert(-1)dnl')dnl
define(`ENDIF_ARGCHECK', `divert dnl')dnl
dnl
dnl Basic commands on your host system. Unless otherwise noted, each command
dnl expects one m4 argument ($1).
define(`SHEBANG', `@echo off')dnl A script prologue/header. No arguments required. 
dnl Safe to leave as "dnl" on non-Unix systems, but "@echo off" is recommended
dnl for DOS/Windows.
define(`CP', copy $1 $2)dnl Copy command. Expects two arguments: source ($1), dest ($2).
define(`CHDIR', cd $1)dnl Chdir command.
define(`RM', del $1)dnl Remove file command.
define(`RMFLAGS', `dnl')dnl Reserved. Safe to leave as "dnl".
define(`EXEC', .\$1)dnl How do you execute EXEs- by calling the exe name directly 
dnl or prepending the current directory (./), or some other means?
define(`SHEXEC', call .\$1)dnl How do you execute one shell script from another- by 
dnl naming the script directly, prepending the current directory (./), or some 
dnl other means?
define(`ECHO', echo $1)dnl Print to screen command.
define(`CHMOD', `dnl')dnl It is safe to leave as "dnl" on non-Unix systems.
define(`MAKE', nmake $1)dnl Make command. If your environment supports parallel make, 
dnl argument "$2" represents the number of parallel jobs. If omitted, make 
dnl doesn't support parallel builds, and only takes a target input "$1".
define(`EXPORT', `set `$1'='$2)dnl Set environment variables command. Expects
dnl two arguments- target ($1), and value ($2). Careful quoting is needed here.
define(`EXPORT_UNIQUE', EXPORT)dnl Set environment variable if not set 
dnl already. This is mainly used to override the used compiler/linker and can 
dnl be safely set to "EXPORT" (notice lack of "`'") if your environment does 
dnl not support this.
define(`INSTALL', CP($1, $2))dnl Install command. Expects two arguments, like copy. 
dnl Can safely alias copy.
define(`EXIT', exit $1)dnl Exit command. Input argument optional?
dnl
dnl OS-specific syntax
define(`COMMENT', `REM')dnl What is the line comment character for scripts?
define(`SH_EXTENSION', `.bat')dnl What extension do shell scripts/command 
dnl files have?
define(`OBJ_EXTENSION', `.obj')dnl Object file exten
define(`EXE_EXTENSION', `.exe')dnl Dot required here, since Unix doesn't use 
dnl extensions for exes...
define(`DEFAULT_DEST', `C:\bin')dnl Not currently used, but where is the final 
dnl destination of binaries?
define(`PATH_SEPARATOR', `\')dnl Redundant?
dnl
dnl Describe your C compiler
define(`CC', cl)dnl C Compiler name
define(`CFLAGS_DEFINE', `/D'$1)dnl Switch to define preprocessor define. Takes one 
dnl argument.
define(`CFLAGS_HOST', `CFLAGS_DEFINE(MSDOS) CFLAGS_DEFINE(WIN32)')dnl Use CFLAGS_DEFINE to add 
dnl preprocessor defines present in the WLA source that correspond to your 
dnl host. These defines take advantage of host-specific functionality. Ideally, 
dnl this is safe to leave undefined. Currently, such macros include:
dnl WIN32, MSDOS, UNIX, and AMIGA. Future macros may includ HAVE_SEEK_END.
define(`CLAGS_COMPILE', `/c')dnl Switch to compile only.
define(`CFLAGS_OPT', `/Ox')dnl Switch to optimize. 
define(`CFLAGS_ANSI', `/Za /W2')dnl Warnings and ANSI compliance switches.
define(`CFLAGS_OTHER', `/nologo')dnl Miscellaneous switches.
define(`CFLAGS_OUT', `')dnl Output switch AND name. Takes one argument. Not used.
dnl
dnl Describe your Linker
define(`LD', link)dnl Linker name.
define(`LDFLAGS_OTHER', `/nologo')dnl Miscellaneous switches.
define(`LDFLAGS_OUT', `/out:'$1)dnl Output switch AND name. Takes one argument.
define(`LIBM_FLAG', `')dnl If the math lib is linked separately, add it here.
dnl
dnl Macros created from the above...
define(`CFLAGS', CLAGS_COMPILE CFLAGS_OPT CFLAGS_ANSI CFLAGS_OTHER)dnl
define(`LDFLAGS', LDFLAGS_OTHER)dnl
define(`WLAFLAGS', CFLAGS_HOST CFLAGS_DEFINE(TARGET_CPU))dnl
