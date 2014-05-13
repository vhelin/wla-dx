include(host_defines)dnl
SHEBANG

IF_ARGCHECK
	ECHO(" ")
	ECHO("USAGE: ./OUTNAME {JOBS}")
	ECHO("Where {JOBS} is the number of jobs (i.e., compiler instances) to run simultaneously.")
	ECHO(" ")
	EXIT(1)
ENDIF_ARGCHECK

EXPORT(`CC', CC)
EXPORT(`LD', LD)

ECHO("")
ECHO("****************************************************************")
ECHO("*          creating opcode decoding speedup tables             *")
ECHO("****************************************************************")
ECHO("")

CHDIR(opcode_table_generator)
CP(create_tables`'.MAKE_HOST_SUFFIX, create_tables`'SH_EXTENSION)
CP(makefile.MAKE_HOST_SUFFIX, makefile)
CHMOD(u+x, create_tables`'SH_EXTENSION)
SHEXEC(create_tables`'SH_EXTENSION)
dnl RM(create_tables`'SH_EXTENSION)
CHDIR(..)

ECHO("")
ECHO("****************************************************************")
ECHO("*                   compiling few tools                        *")
ECHO("****************************************************************")
ECHO("")

define(`MAKE_TOOL_IN_DIR', `CHDIR($1)
CP(makefile.MAKE_HOST_SUFFIX, makefile)
MAKE(`', -j `$'`'1)
CP($1`'EXE_EXTENSION, ..`'PATH_SEPARATOR`'binaries)
MAKE(clean)
CHDIR(..)')dnl
MAKE_TOOL_IN_DIR(wlab)

MAKE_TOOL_IN_DIR(wlad)

ECHO("")
ECHO("****************************************************************")
ECHO("*                  compiling WLA binaries                      *")
ECHO("****************************************************************")
ECHO("")

define(`MAKE_WLA_ASM', `MAKE(clean)
CP(makefiles`'PATH_SEPARATOR`'makefile.MAKE_HOST_SUFFIX.$1, makefile)
MAKE(`', -j `$'`'1)
CP(wla-$1`'EXE_EXTENSION, binaries)')dnl
MAKE_WLA_ASM(gb)

MAKE_WLA_ASM(z80)

MAKE_WLA_ASM(6502)

MAKE_WLA_ASM(65c02)

MAKE_WLA_ASM(6510)

MAKE_WLA_ASM(65816)

MAKE_WLA_ASM(spc700)

MAKE_WLA_ASM(huc6280)

MAKE(clean)

ECHO("")
ECHO("****************************************************************")
ECHO("*                    compiling WLALINK                         *")
ECHO("****************************************************************")
ECHO("")

MAKE_TOOL_IN_DIR(wlalink)

ECHO("")
ECHO("****************************************************************")
ECHO("*        all done! check ./binaries/ for the results           *")
ECHO("****************************************************************")
ECHO("")
