# This Makefile has been generated using generate_mk.sh.

.POSIX:

WLA_SRCS = main.c hashmap.c parse.c include_file.c pass_1.c pass_2.c pass_3.c pass_4.c stack.c listfile.c
WLALINK_SRCS = wlalink/main.c wlalink/memory.c wlalink/parse.c wlalink/files.c wlalink/check.c wlalink/analyze.c wlalink/write.c wlalink/compute.c wlalink/discard.c wlalink/listfile.c hashmap.c
WLAB_SRCS = wlab/main.c
LDLIBS = -l c -l m
LDLIBS_GEN = -l c
LD = $(CC)
RM = rm -rf
DEBUGFLAGS = -DNDEBUG

.c.o:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -c $< -o $@

all: wlab wlalink wla-gb wla-65c02 wla-6502 wla-6510 wla-65816 wla-huc6280 wla-spc700 wla-z80  

clean:
	-$(RM) binaries/wlab binaries/wlalink  binaries/wla-gb binaries/wla-65c02 binaries/wla-6502 binaries/wla-6510 binaries/wla-65816 binaries/wla-huc6280 binaries/wla-spc700 binaries/wla-z80  
	-$(RM) $(WLAB_SRCS:.c=.o) $(WLALINK_SRCS:.c=.o)
	-$(RM)  $(WLA_SRCS:.c=.gb_o) $(WLA_gb_GEN:.c=.gb_o) $(WLA_SRCS:.c=.65c02_o) $(WLA_65c02_GEN:.c=.65c02_o) $(WLA_SRCS:.c=.6502_o) $(WLA_6502_GEN:.c=.6502_o) $(WLA_SRCS:.c=.6510_o) $(WLA_6510_GEN:.c=.6510_o) $(WLA_SRCS:.c=.65816_o) $(WLA_65816_GEN:.c=.65816_o) $(WLA_SRCS:.c=.huc6280_o) $(WLA_huc6280_GEN:.c=.huc6280_o) $(WLA_SRCS:.c=.spc700_o) $(WLA_spc700_GEN:.c=.spc700_o) $(WLA_SRCS:.c=.z80_o) $(WLA_z80_GEN:.c=.z80_o)  

wlab: binaries/wlab
binaries/wlab: $(WLAB_SRCS:.c=.o)
	$(LD) $(LDFLAGS) -o binaries/wlab $(WLAB_SRCS:.c=.o) $(LDLIBS)

wlalink: binaries/wlalink
binaries/wlalink: $(WLALINK_SRCS:.c=.o)
	$(LD) $(LDFLAGS) -o binaries/wlalink $(WLALINK_SRCS:.c=.o) $(LDLIBS)
#### gb

WLA_gb_GEN = opcode_table_generator.c opcodes_gb.c
WLA_gb_GENS = opcodes_gb_tables.c opcodes_gb.c
WLA_gb_FLAGS = -DGB=1
.SUFFIXES: .gb_o
.c.gb_o:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(WLA_gb_FLAGS) -c $< -o $@
wla-gb: binaries/wla-gb
binaries/wla-gb: $(WLA_SRCS:.c=.gb_o) $(WLA_gb_GENS:.c=.gb_o)
	$(LD) $(LDFLAGS) -o $@ $(WLA_SRCS:.c=.gb_o) $(WLA_gb_GENS:.c=.gb_o) $(LDLIBS)
gen-gb: $(WLA_gb_GEN:.c=.gb_o)
	$(LD) $(LDFLAGS) -o $@ $(WLA_gb_GEN:.c=.gb_o) $(LDLIBS_GEN)
opcodes_gb_tables.c: gen-gb
	./$< $@

#### 65c02

WLA_65c02_GEN = opcode_table_generator.c opcodes_65c02.c
WLA_65c02_GENS = opcodes_65c02_tables.c opcodes_65c02.c
WLA_65c02_FLAGS = -DWDC65C02=1
.SUFFIXES: .65c02_o
.c.65c02_o:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(WLA_65c02_FLAGS) -c $< -o $@
wla-65c02: binaries/wla-65c02
binaries/wla-65c02: $(WLA_SRCS:.c=.65c02_o) $(WLA_65c02_GENS:.c=.65c02_o)
	$(LD) $(LDFLAGS) -o $@ $(WLA_SRCS:.c=.65c02_o) $(WLA_65c02_GENS:.c=.65c02_o) $(LDLIBS)
gen-65c02: $(WLA_65c02_GEN:.c=.65c02_o)
	$(LD) $(LDFLAGS) -o $@ $(WLA_65c02_GEN:.c=.65c02_o) $(LDLIBS_GEN)
opcodes_65c02_tables.c: gen-65c02
	./$< $@

#### 6502

WLA_6502_GEN = opcode_table_generator.c opcodes_6502.c
WLA_6502_GENS = opcodes_6502_tables.c opcodes_6502.c
WLA_6502_FLAGS = -DMCS6502=1
.SUFFIXES: .6502_o
.c.6502_o:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(WLA_6502_FLAGS) -c $< -o $@
wla-6502: binaries/wla-6502
binaries/wla-6502: $(WLA_SRCS:.c=.6502_o) $(WLA_6502_GENS:.c=.6502_o)
	$(LD) $(LDFLAGS) -o $@ $(WLA_SRCS:.c=.6502_o) $(WLA_6502_GENS:.c=.6502_o) $(LDLIBS)
gen-6502: $(WLA_6502_GEN:.c=.6502_o)
	$(LD) $(LDFLAGS) -o $@ $(WLA_6502_GEN:.c=.6502_o) $(LDLIBS_GEN)
opcodes_6502_tables.c: gen-6502
	./$< $@

#### 6510

WLA_6510_GEN = opcode_table_generator.c opcodes_6510.c
WLA_6510_GENS = opcodes_6510_tables.c opcodes_6510.c
WLA_6510_FLAGS = -DMCS6510=1
.SUFFIXES: .6510_o
.c.6510_o:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(WLA_6510_FLAGS) -c $< -o $@
wla-6510: binaries/wla-6510
binaries/wla-6510: $(WLA_SRCS:.c=.6510_o) $(WLA_6510_GENS:.c=.6510_o)
	$(LD) $(LDFLAGS) -o $@ $(WLA_SRCS:.c=.6510_o) $(WLA_6510_GENS:.c=.6510_o) $(LDLIBS)
gen-6510: $(WLA_6510_GEN:.c=.6510_o)
	$(LD) $(LDFLAGS) -o $@ $(WLA_6510_GEN:.c=.6510_o) $(LDLIBS_GEN)
opcodes_6510_tables.c: gen-6510
	./$< $@

#### 65816

WLA_65816_GEN = opcode_table_generator.c opcodes_65816.c
WLA_65816_GENS = opcodes_65816_tables.c opcodes_65816.c
WLA_65816_FLAGS = -DW65816=1
.SUFFIXES: .65816_o
.c.65816_o:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(WLA_65816_FLAGS) -c $< -o $@
wla-65816: binaries/wla-65816
binaries/wla-65816: $(WLA_SRCS:.c=.65816_o) $(WLA_65816_GENS:.c=.65816_o)
	$(LD) $(LDFLAGS) -o $@ $(WLA_SRCS:.c=.65816_o) $(WLA_65816_GENS:.c=.65816_o) $(LDLIBS)
gen-65816: $(WLA_65816_GEN:.c=.65816_o)
	$(LD) $(LDFLAGS) -o $@ $(WLA_65816_GEN:.c=.65816_o) $(LDLIBS_GEN)
opcodes_65816_tables.c: gen-65816
	./$< $@

#### huc6280

WLA_huc6280_GEN = opcode_table_generator.c opcodes_huc6280.c
WLA_huc6280_GENS = opcodes_huc6280_tables.c opcodes_huc6280.c
WLA_huc6280_FLAGS = -DHUC6280=1
.SUFFIXES: .huc6280_o
.c.huc6280_o:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(WLA_huc6280_FLAGS) -c $< -o $@
wla-huc6280: binaries/wla-huc6280
binaries/wla-huc6280: $(WLA_SRCS:.c=.huc6280_o) $(WLA_huc6280_GENS:.c=.huc6280_o)
	$(LD) $(LDFLAGS) -o $@ $(WLA_SRCS:.c=.huc6280_o) $(WLA_huc6280_GENS:.c=.huc6280_o) $(LDLIBS)
gen-huc6280: $(WLA_huc6280_GEN:.c=.huc6280_o)
	$(LD) $(LDFLAGS) -o $@ $(WLA_huc6280_GEN:.c=.huc6280_o) $(LDLIBS_GEN)
opcodes_huc6280_tables.c: gen-huc6280
	./$< $@

#### spc700

WLA_spc700_GEN = opcode_table_generator.c opcodes_spc700.c
WLA_spc700_GENS = opcodes_spc700_tables.c opcodes_spc700.c
WLA_spc700_FLAGS = -DSPC700=1
.SUFFIXES: .spc700_o
.c.spc700_o:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(WLA_spc700_FLAGS) -c $< -o $@
wla-spc700: binaries/wla-spc700
binaries/wla-spc700: $(WLA_SRCS:.c=.spc700_o) $(WLA_spc700_GENS:.c=.spc700_o)
	$(LD) $(LDFLAGS) -o $@ $(WLA_SRCS:.c=.spc700_o) $(WLA_spc700_GENS:.c=.spc700_o) $(LDLIBS)
gen-spc700: $(WLA_spc700_GEN:.c=.spc700_o)
	$(LD) $(LDFLAGS) -o $@ $(WLA_spc700_GEN:.c=.spc700_o) $(LDLIBS_GEN)
opcodes_spc700_tables.c: gen-spc700
	./$< $@

#### z80

WLA_z80_GEN = opcode_table_generator.c opcodes_z80.c
WLA_z80_GENS = opcodes_z80_tables.c opcodes_z80.c
WLA_z80_FLAGS = -DZ80=1
.SUFFIXES: .z80_o
.c.z80_o:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(WLA_z80_FLAGS) -c $< -o $@
wla-z80: binaries/wla-z80
binaries/wla-z80: $(WLA_SRCS:.c=.z80_o) $(WLA_z80_GENS:.c=.z80_o)
	$(LD) $(LDFLAGS) -o $@ $(WLA_SRCS:.c=.z80_o) $(WLA_z80_GENS:.c=.z80_o) $(LDLIBS)
gen-z80: $(WLA_z80_GEN:.c=.z80_o)
	$(LD) $(LDFLAGS) -o $@ $(WLA_z80_GEN:.c=.z80_o) $(LDLIBS_GEN)
opcodes_z80_tables.c: gen-z80
	./$< $@

