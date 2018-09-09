#!/bin/sh

OUTDIR=mk
test ! -d mk && OUTDIR=.

# Calls the actual generator
gen(){
    (
    export RM
    export CC COMPILE_DEF CFLAGS CFLAGS_MISC CFLAGS_OPT
    export LD LDFLAGS LDFLAGS_MISC LDLIBS LDLIBS_GEN
    export NO_POSIX
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
    CC=
    COMPILE_DEF=
    CFLAGS=
    CFLAGS_MISC=
    CFLAGS_OPT=
    LD=
    LDFLAGS=
    LDFLAGS_MISC=
    LDLIBS=
    LDLIBS_GEN=
    NO_POSIX=
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

