#!/bin/sh

OUTDIR=mk
test ! -d mk && OUTDIR=.

# Calls the actual generator
gen(){
    (
    export RM RM_GLOB EXT
    export CC COMPILE_DEF CFLAGS CFLAGS_MISC CFLAGS_OPT
    export LD LDFLAGS LDFLAGS_MISC LDFLAGS_SRCS LDLIBS LDLIBS_GEN
    export CC_TEMPLATE LD_TEMPLATE
    export NO_POSIX BACKSLASH_SRC_DIR BACKSLASH_BIN_DIR
    export GEN_EXEC FOOTER_FILE
    test -f mk/gen_mk.sh && ./mk/gen_mk.sh \
            || ( test -f ./gen_mk.sh && ./gen_mk.sh )
    )
}

SETUPS=posix
setup_posix() {
    # See gen_mk.sh for more information about the variables
    # This is meant to set everything to their default values,
    # so just keep it empty
    RM=
    RM_GLOB=
    EXT=
    CC=
    CC_TEMPLATE=
    COMPILE_DEF=
    CFLAGS=
    CFLAGS_MISC=
    CFLAGS_OPT=
    LD=
    LD_TEMPLATE=
    LDFLAGS_SRCS=
    LDFLAGS=
    LDFLAGS_MISC=
    LDLIBS=
    LDLIBS_GEN=
    NO_POSIX=
    BACKSLASH_SRC_DIR=
    BACKSLASH_BIN_DIR=
    GEN_EXEC=
    FOOTER_FILE=
    test -z "$1" || "$*"
}

SETUPS="$SETUPS gcc"
setup_gcc() {
    setup_posix
    CC=gcc
    CFLAGS_OPT=-O3
    CFLAGS_MISC="-Wall -ansi -pedantic"
    LDLIBS="-l m"
    LDLIBS_GEN="-l m"
    test -z "$1" || "$*"
}

SETUPS="$SETUPS mingw"
setup_mingw() {
    setup_gcc
    RM=del
    EXT=.exe
    CFLAGS_MISC="$CFLAGS_MISC -DMSDOS=1 -DWIN32=1"
    BACKSLASH_SRC_DIR=1
    BACKSLASH_BIN_DIR=1
    test -z "$1" || "$*"
}

SETUPS="$SETUPS dosdjgpp"
setup_dosdjgpp() {
    setup_mingw
    CFLAGS_OPT=-O2 # It's DOS, it's slow to compile on
    CFLAGS_MISC="$(echo "$CFLAGS_MISC" | sed 's/-DWIN32=1//')"
    test -z "$1" || "$*"
}

SETUPS="$SETUPS win32cl"
setup_win32cl() {
    setup_posix
    RM=del
    EXT=.exe
    CC=cl
    CC_TEMPLATE="\$(CC) \$(DEBUGFLAGS) {flags} \$(CFLAGS_ALL) /c {in} /Fo: {out}"
    COMPILE_DEF="/D%s"
    CFLAGS_MISC="/nologo /Za /DMSDOS=1 /DWIN32=1"
    CFLAGS_OPT=/Ox
    LD=link
    LD_TEMPLATE="\$(LD) \$(LDFLAGS_ALL) {in} {libs} /out:{out}"
    LDFLAGS_MISC=/nologo
    LDLIBS=" "
    LDLIBS_GEN=" "
    BACKSLASH_SRC_DIR=1
    BACKSLASH_BIN_DIR=1
    GEN_EXEC="\$?"
    test -z "$1" || "$*"
}

SETUPS="$SETUPS amiga"
setup_amiga() {
    setup_posix
    RM=delete
    EXT=".\$(CPU)"
    CC=sc
    COMPILE_DEF="define %s"
    CFLAGS_MISC="define AMIGA=1"
    LD=slink
    LD_TEMPLATE="\$(LD) \$(LDFLAGS_ALL) FROM {in} TO {out} {libs}"
    LDFLAGS_MISC="STRIPDEBUG NOICONS MATH=STANDARD CPU=68\$(CPU)"
    LDLIBS="LIB LIB:scm.lib LIB:sc.lib LIB:amiga.lib"
    LDLIBS_GEN="LIB LIB:scm.lib LIB:sc.lib LIB:amiga.lib"
    NO_POSIX=1
    FOOTER_FILE="$OUTDIR"/amiga.footer
    test -z "$1" || "$*"
}

SETUPS="$SETUPS amiga_gcc"
setup_amiga_gcc() {
    setup_gcc
    RM=delete
    CFLAGS_MISC=-DUNIX
    test -z "$1" || "$*"
}

generate_for() {
    for target in $SETUPS; do
        test "$1" = "$target" && setup_"$target" gen > "$OUTDIR"/"$target".mk
    done
}

IFS=' '
if [ -z "$1" ]; then
    cat << EOF
Usage: $0 all
       $0 <target>
Use 'all' to generate an makefile for every target this script can
generate for.
Use '<target>' to only generate specific targets. Available targets:
EOF
    for target in $SETUPS; do
        echo "    $target"
    done
    exit 1
elif [ "$1" = "all" ]; then
    for target in $SETUPS; do
        generate_for "$target"
    done
else
    while ! [ -z "$1" ]; do
        generate_for "$1"
        shift
    done
fi

