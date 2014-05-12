include(host_defines)dnl
dnl For now, making assumption that removing ?= conditional assignment is harmless...
dnl CC?=gcc
dnl LD?=gcc
dnl WLA_TARGET?=HUC6280
dnl WLA_QUIET?=0

`CC'=CC
`LD'=LD
`CFLAGS' = CFLAGS HOSTFLAGS CFLAGS_DEFINE`'$(WLA_TARGET) CFLAGS_DEFINE`'QUIET=$(WLA_QUIET)
`LDFLAGS' = LDFLAGS 
`RM' = RM

CFILES = main.c
OFILES = main.o



all: $(OFILES) makefile
	$(`LD') $(`LDFLAGS') $(`OFILES') LDFLAGS_OUT gen
dnl	STRIP(gen)

main.o: main.c makefile
	$(`CC') $(`CFLAGS') main.c


$(OFILES): $(CFILES)


clean:
	$(`RM') $(OFILES) core *~ gen
