include(host_defines)dnl
`CC' = CC
`CFLAGS' = CFLAGS
`LD' = LD
`LDFLAGS' = LDFLAGS
`WLAFLAGS' = $(`CFLAGS') WLAFLAGS
`RM' = RM

CFILES = main.c parse.c include_file.c pass_1.c pass_2.c pass_3.c pass_4.c stack.c listfile.c
HFILES = main.h parse.h include_file.h pass_1.h pass_2.h pass_3.h pass_4.h stack.h listfile.h defines.h
OFILES = main`'OBJ_EXTENSION parse`'OBJ_EXTENSION include_file`'OBJ_EXTENSION pass_1`'OBJ_EXTENSION pass_2`'OBJ_EXTENSION pass_3`'OBJ_EXTENSION pass_4`'OBJ_EXTENSION stack`'OBJ_EXTENSION listfile`'OBJ_EXTENSION


all: $(OFILES) makefile
	$(`LD') $(`LDFLAGS') $(OFILES) LDFLAGS_OUT`'`wla-'OUT_SUFFIX`'EXE_EXTENSION

main`'OBJ_EXTENSION: main.c defines.h main.h makefile
	$(`CC') $(`WLAFLAGS') main.c

parse`'OBJ_EXTENSION: parse.c defines.h parse.h makefile
	$(`CC') $(`WLAFLAGS') parse.c

include_file`'OBJ_EXTENSION: include_file.c defines.h include_file.h makefile
	$(`CC') $(`WLAFLAGS') include_file.c

pass_1`'OBJ_EXTENSION: pass_1.c defines.h pass_1.h parse.h makefile OPCODE_PREREQ
	$(`CC') $(`WLAFLAGS') pass_1.c

pass_2`'OBJ_EXTENSION: pass_2.c defines.h pass_2.h makefile
	$(`CC') $(`WLAFLAGS') pass_2.c

pass_3`'OBJ_EXTENSION: pass_3.c defines.h pass_3.h makefile
	$(`CC') $(`WLAFLAGS') pass_3.c

pass_4`'OBJ_EXTENSION: pass_4.c defines.h pass_4.h makefile
	$(`CC') $(`WLAFLAGS') pass_4.c

stack`'OBJ_EXTENSION: stack.c defines.h stack.h makefile
	$(`CC') $(`WLAFLAGS') stack.c

listfile`'OBJ_EXTENSION: listfile.c defines.h makefile
	$(`CC') $(`WLAFLAGS') listfile.c


$(OFILES): $(HFILES)


clean:
	$(`RM') $(OFILES) *~ wla-OUT_SUFFIX`'EXE_EXTENSION

install:
	make ; cp wla-gb.exe /usr/local/bin
