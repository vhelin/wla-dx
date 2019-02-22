#!/bin/sh

# This script generates an Makefile and outputs it to stdout.
# Use generate.sh to generate preconfigured makefiles instread!
#
# Accepted parameters are passed through environment variables:
# RM - Remove command (default: rm -rf)
# RM_GLOB - Remove command glob syntax (default: *.%s)
# EXT - Extension (default: )
# CC - C compiler (default: $CC, otherwise make default)
# CC_TEMPLATE - Command (def: \$(CC) \$(DEBUGFLAGS) {flags} \$(CFLAGS_ALL) -c {in} -o {out})
# COMPILE_DEF - Format string for defining defs (default: -D%s)
# CFLAGS - C flags, overwritable by user (default: make default)
# CFLAGS_MISC - Misc C flags (default: )
# CFLAGS_OPT - C flags about optimization (default: -O3)
# LD - Linker (default: $LD, or \$(CC))
# LD_TEMPLATE - Command (default: \$(LD) \$(LDFLAGS_ALL) {in} {libs} -o {out})
# LDFLAGS - Linker flags, overwritable by user (default: make default)
# LDFLAGS_MISC - Misc linker flags (default: )
# LDFLAGS_SRCS - Format string for defining srcs (default: %s)
# LDLIBS - Libraries to link with (std c and math lib) (default: -l c -l m)
# LDLIBS_GEN - Libraries to link when making gen-binaries (default: -l c)
# NO_POSIX - Don't insert .POSIX when 1 (default: 0)
# BACKSLASH_SRC_DIR - 1 for \ than / for src dirs seperator (default: 0)
# BACKSLASH_BIN_DIR - 1 for \ than / for bin dirs seperator (default: 0)
# FOOTER_FILE - Additional footer to include (default: )
#
# Uses echo, printf, tr, sed, awk, cat and sh
#env >&2

# WLA name, DEFINE name
TARGETS="
gb,GB
65c02,WDC65C02
6502,MCS6502
6510,MCS6510
65816,W65816
huc6280,HUC6280
spc700,SPC700
z80,Z80
"
# Processed Targets, first column is the "object number"
PTARGETS="$(echo "$TARGETS" | awk -F , 'NF {print NR-1 "," $0}')"

WLA_SRCS="
main.c
hashmap.c
parse.c
include_file.c
pass_1.c
pass_2.c
pass_3.c
pass_4.c
stack.c
listfile.c
crc32.c
"
WLA_SRCS="$(echo "$WLA_SRCS" | tr '\n' ' ')"

WLALINK_SRCS="
wlalink/main.c
wlalink/memory.c
wlalink/parse.c
wlalink/files.c
wlalink/check.c
wlalink/analyze.c
wlalink/write.c
wlalink/compute.c
wlalink/discard.c
wlalink/listfile.c
hashmap.c
crc32.c
"
WLALINK_SRCS="$(echo "$WLALINK_SRCS" | tr '\n' ' ')"
test "${BACKSLASH_SRC_DIR:-0}" = 1 && WLALINK_SRCS="$(echo "$WLALINK_SRCS" | tr '/' '\\')"

WLAB_SRCS="
wlab/main.c
"
WLAB_SRCS="$(echo "$WLAB_SRCS" | tr '\n' ' ')"
test "${BACKSLASH_SRC_DIR:-0}" = 1 && WLAB_SRCS="$(echo "$WLAB_SRCS" | tr '/' '\\')"

# \1 is the first part, \2 is the second, etc.
c() {
    sed -e 's#\([[:alnum:]]\+\),\([[:alnum:]]\+\)\+,\([[:alnum:]]\+\)\+#'"$1"'#g' | tr '\n' ' '
}
cc() {
    echo "$PTARGETS" | c "$*"
}
pf() {
    printf -- "$1" "$2"
}
bindir() {
    test "${BACKSLASH_BIN_DIR:-0}" = 1 && \
        echo "binaries\\" || echo "binaries/"
}
templatenize() {
    STR="$1"
    shift
    while ! [ -z "$1" ]; do
        SAFE="$(printf "%s" "$2" | sed -e 's,\\,\\\\,g')"
        STR="$(printf "%s" "$STR" | sed -e "s^$1^$SAFE^")"
        shift
        shift
    done
    printf "%s" "$STR"
}
ld_template() {
    templatenize "$LD_TEMPLATE" "{in}" "$1" "{out}" "$2" "{libs}" "$3"
}
cc_template() {
    templatenize "$CC_TEMPLATE" "{in}" "$1" "{out}" "$2" "{flags}" "$3"
}
rm_glob() {
    test -z "$RM_GLOB" && echo "$1" || \
        templatenize "$RM_GLOB" "{pre}" "$2" "{suf}" "$3"
}

# Header
RM="${RM:-rm -rf}"
test -z "$RM_GLOB" && RM_GLOB="{pre}*.{suf}"
CC="${CC}"
test -z "$CC_TEMPLATE" && CC_TEMPLATE="\$(CC) \$(DEBUGFLAGS) {flags} \$(CFLAGS_ALL) -c {in} -o {out}"
COMPILE_DEF="${COMPILE_DEF:--D%s}"
CFLAGS_OPT="${CFLAGS_OPT:--O3}"
DBGFLAGS="$(pf "$COMPILE_DEF" "NDEBUG=1")"
LD="${LD:-\$(CC)}"
test -z "$LD_TEMPLATE" && LD_TEMPLATE="\$(LD) \$(LDFLAGS_ALL) {in} {libs} -o {out}"
LDFLAGS_SRCS="${LDFLAGS_SRCS:-%s}"
LDLIBS="${LDLIBS:--l c -l m}"
LDLIBS_GEN="${LDLIBS:--l c}"
SLASH="/"
test "${BACKSLASH_SRC_DIR:-0}" = 1 && SLASH="\\"

cat << EOF
#!/usr/bin/env make -f
# This makefile has been auto-generated.
$(test "${NO_POSIX:-0}" = "0" && echo ".POSIX:")

WLA_SRCS=${WLA_SRCS}
WLALINK_SRCS=${WLALINK_SRCS}
WLAB_SRCS=${WLAB_SRCS}
RM=${RM}
EXT=${EXT}
$(test -z "$CC" || echo "CC=${CC}")
CFLAGS_ALL=${CFLAGS_MISC} ${CFLAGS_OPT} \$(CFLAGS)
DEBUGFLAGS=${DBGFLAGS}
LD=${LD}
LDFLAGS_ALL=${LDFLAGS_MISC} \$(LDFLAGS)
LDLIBS=${LDLIBS}
LDLIBS_GEN=${LDLIBS_GEN}

.c.o:
	$(cc_template '$*.c' '$@' "")

all: wlab wlalink $(cc 'wla-\2')
generators: $(cc 'gen-\2')

clean: clean-wlab clean-wlalink clean-wla clean-gen
clean-objects:
	-\$(RM) $(cc "$(rm_glob '$(WLA_SRCS:.c=.o\1) $(WLA_\2_GENO) $(WLA_\2_GENSO)' \
		"" 'o\1')")
	-\$(RM) $(rm_glob "\$(WLAB_SRCS:.c=.o)" "wlab${SLASH}" "o")
	-\$(RM) $(rm_glob "\$(WLALINK_SRCS:.c=.o)" "wlalink${SLASH}" "o") $(rm_glob "" "" "o")
clean-wlab:
	-\$(RM) $(bindir)wlab\$(EXT)
	-\$(RM) \$(WLAB_SRCS:.c=.o)
	-\$(RM) $(rm_glob "\$(WLAB_SRCS:.c=.o)" "wlab${SLASH}" "o")
clean-wlalink:
	-\$(RM) $(bindir)wlalink\$(EXT)
	-\$(RM) $(rm_glob "\$(WLALINK_SRCS:.c=.o)" "wlalink${SLASH}" "o") $(rm_glob "" "" "o")
clean-wla: $(cc 'clean-wla-\2')
clean-gen: $(cc 'clean-gen-\2')

wlab: $(bindir)wlab\$(EXT)
$(bindir)wlab\$(EXT): \$(WLAB_SRCS:.c=.o)
	$(ld_template "$(pf "$LDFLAGS_SRCS" "\$(WLAB_SRCS:.c=.o)")" \
        "\$@" "\$(LDLIBS)")

wlalink: $(bindir)wlalink\$(EXT)
$(bindir)wlalink\$(EXT): \$(WLALINK_SRCS:.c=.o)
	$(ld_template "$(pf "$LDFLAGS_SRCS" "\$(WLALINK_SRCS:.c=.o)")" \
        "\$@" "\$(LDLIBS)")
EOF

# Now let's generate stuff for each cpu/arch supported
WO=0 # Object number, so the object file extension is <=3 chars
for TARGET in $PTARGETS; do
    W="$(printf "$TARGET" | c '\2')"
    WO=$((WO+1))
    OW="o$WO" # Object file extension
    D="$(printf "$TARGET" | c '\3')"
    cat << EOF

WLA_${W}_O=\$(WLA_SRCS:.c=.${OW})
WLA_${W}_GEN=opcode_table_generator.c opcodes_${W}.c
WLA_${W}_GENO=\$(WLA_${W}_GEN:.c=.${OW})
WLA_${W}_GENS=opcodes_${W}_tables.c opcodes_${W}.c
WLA_${W}_GENSO=\$(WLA_${W}_GENS:.c=.${OW})
WLA_${W}_FLAGS=$(printf -- "$COMPILE_DEF" "${D}=1")
.SUFFIXES: .${OW}
.c.${OW}:
	$(cc_template '$*.c' '$@' "\$(WLA_${W}_FLAGS)")
wla-${W}: $(bindir)wla-${W}\$(EXT)
$(bindir)wla-${W}\$(EXT): \$(WLA_${W}_O) \$(WLA_${W}_GENSO)
	$(ld_template "$(pf "$LDFLAGS_SRCS" "\$(WLA_${W}_O) \$(WLA_${W}_GENSO)")" \
        "\$@" "\$(LDLIBS)")
gen-${W}\$(EXT): \$(WLA_${W}_GENO)
	$(ld_template "$(pf "$LDFLAGS_SRCS" "\$(WLA_${W}_GENO)")" \
        "\$@" "\$(LDLIBS_GEN)")
opcodes_${W}_tables.c: gen-${W}\$(EXT)
	./\$< \$@
clean-wla-${W}:
	-\$(RM) $(bindir)wla-${W}\$(EXT) \$(WLA_${W}_O)
clean-gen-${W}:
	-\$(RM) gen-${W}\$(EXT) \$(WLA_${W}_GENO) \$(WLA_${W}_GENSO)
EOF
    done

test -e "$FOOTER_FILE" && cat "$FOOTER_FILE"

