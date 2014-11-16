include(host_defines)dnl
dnl For now, making assumption that removing ?= conditional assignment is harmless...
dnl CC?=gcc
dnl LD?=gcc
dnl WLA_TARGET?=HUC6280
dnl WLA_QUIET?=0
dnl
`CC'=CC
`LD'=LD
`CFLAGS' = CFLAGS CFLAGS_HOST CFLAGS_DEFINE($(WLA_TARGET)) CFLAGS_DEFINE(QUIET)=$(WLA_QUIET)
`LDFLAGS' = LDFLAGS 
`RM' = RM
`RMFLAGS' = RMFLAGS

CFILES = main.c
OFILES = main`'OBJ_EXTENSION



all: $(OFILES) makefile
	$(`LD') $(`LDFLAGS') $(`OFILES') LDFLAGS_OUT(gen`'EXE_EXTENSION) LIBM_FLAG
dnl	STRIP(gen)

main`'OBJ_EXTENSION: main.c makefile
	$(`CC') $(`CFLAGS') main.c


$(OFILES): $(CFILES)


clean:
	$(`RM') $(`RMFLAGS') $(OFILES) core *~ gen
