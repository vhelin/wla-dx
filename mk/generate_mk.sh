#!/bin/sh

# This shell script generates an Makefile so the makefile doesn't have to
# be changed so much in order to add just some things.

# WLA name, DEFINE name
CPUS="
gb,GB
65c02,WDC65C02
6502,MCS6502
6510,MCS6510
65816,W65816
huc6280,HUC6280
spc700,SPC700
z80,Z80
"

# \1 is the first part, \2 is the second, etc.
c() {
    tr '\n' ' ' | sed -e 's# \+\([[:alnum:]]\+\),\([[:alnum:]]\+\)\+#'"$1"'#g'
}
cc() {
    echo "$CPUS" | c "$*"
}

gen() {
    # Header
cat << EOF
# This Makefile has been generated using generate_mk.sh.

.POSIX:

WLA_SRCS = main.c hashmap.c parse.c include_file.c pass_1.c pass_2.c pass_3.c pass_4.c stack.c listfile.c
WLALINK_SRCS = wlalink/main.c wlalink/memory.c wlalink/parse.c wlalink/files.c wlalink/check.c wlalink/analyze.c wlalink/write.c wlalink/compute.c wlalink/discard.c wlalink/listfile.c hashmap.c
WLAB_SRCS = wlab/main.c
LDLIBS = -l c -l m
LDLIBS_GEN = -l c
LD = \$(CC)
RM = rm -rf
DEBUGFLAGS = -DNDEBUG

.c.o:
	\$(CC) \$(CFLAGS) \$(DEBUGFLAGS) -c \$< -o \$@

all: wlab wlalink$(cc ' wla-\1')

clean:
	-\$(RM) binaries/wlab binaries/wlalink $(cc ' binaries/wla-\1')
	-\$(RM) \$(WLAB_SRCS:.c=.o) \$(WLALINK_SRCS:.c=.o)
	-\$(RM) $(cc ' $(WLA_SRCS:.c=.\1_o) $(WLA_\1_GEN:.c=.\1_o)')

wlab: binaries/wlab
binaries/wlab: \$(WLAB_SRCS:.c=.o)
	\$(LD) \$(LDFLAGS) -o binaries/wlab \$(WLAB_SRCS:.c=.o) \$(LDLIBS)

wlalink: binaries/wlalink
binaries/wlalink: \$(WLALINK_SRCS:.c=.o)
	\$(LD) \$(LDFLAGS) -o binaries/wlalink \$(WLALINK_SRCS:.c=.o) \$(LDLIBS)
EOF

    # Now let's generate stuff for each cpu/arch supported
    for CPU in $CPUS; do
        W="$(printf " $CPU" | c '\1')"
        D="$(printf " $CPU" | c '\2')"
        cat << EOF
#### $W

WLA_${W}_GEN = opcode_table_generator.c opcodes_${W}.c
WLA_${W}_GENS = opcodes_${W}_tables.c opcodes_${W}.c
WLA_${W}_FLAGS = -D${D}=1
.SUFFIXES: .${W}_o
.c.${W}_o:
	\$(CC) \$(CFLAGS) \$(DEBUGFLAGS) \$(WLA_${W}_FLAGS) -c \$< -o \$@
wla-${W}: binaries/wla-${W}
binaries/wla-${W}: \$(WLA_SRCS:.c=.${W}_o) \$(WLA_${W}_GENS:.c=.${W}_o)
	\$(LD) \$(LDFLAGS) -o \$@ \$(WLA_SRCS:.c=.${W}_o) \$(WLA_${W}_GENS:.c=.${W}_o) \$(LDLIBS)
gen-${W}: \$(WLA_${W}_GEN:.c=.${W}_o)
	\$(LD) \$(LDFLAGS) -o \$@ \$(WLA_${W}_GEN:.c=.${W}_o) \$(LDLIBS_GEN)
opcodes_${W}_tables.c: gen-${W}
	./\$< \$@

EOF
    done

}

OUTPUT=mk/Makefile
test ! -d mk && OUTPUT=Makefile

gen > $OUTPUT

