# Subproject: opcode_table_generator
project(opcode_table_generator)

add_definitions( -DWLA_QUIET )
set(CFILES main.c)

# --- WLA_TARGET=GB
add_executable (gen_GB ${CFILES})
set_target_properties (gen_GB PROPERTIES COMPILE_DEFINITIONS "GB")
# Generate tables
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/opcodes_gb_tables.c
	COMMAND gen_GB
	DEPENDS gen_GB
	)
ADD_CUSTOM_TARGET(GB_table ALL DEPENDS ${CMAKE_BINARY_DIR}/opcodes_gb_tables.c)

# --- WLA_TARGET=Z80
add_executable (gen_Z80 ${CFILES})
set_target_properties (gen_Z80 PROPERTIES COMPILE_DEFINITIONS "Z80")
# Generate tables
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/opcodes_z80_tables.c
	COMMAND gen_Z80
	DEPENDS gen_Z80
	)
ADD_CUSTOM_TARGET(Z80_table ALL DEPENDS ${CMAKE_BINARY_DIR}/opcodes_z80_tables.c)

# --- WLA_TARGET=MCS6502
add_executable (gen_MCS6502 ${CFILES})
set_target_properties (gen_MCS6502 PROPERTIES COMPILE_DEFINITIONS "MCS6502")
# Generate tables
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/opcodes_6502_tables.c
	COMMAND gen_MCS6502
	DEPENDS gen_MCS6502
	)
ADD_CUSTOM_TARGET(MCS6502_table ALL DEPENDS ${CMAKE_BINARY_DIR}/opcodes_6502_tables.c)

# --- WLA_TARGET=WDC65C02
add_executable (gen_WDC65C02 ${CFILES})
set_target_properties (gen_WDC65C02 PROPERTIES COMPILE_DEFINITIONS "WDC65C02")
# Generate tables
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/opcodes_65c02_tables.c
	COMMAND gen_WDC65C02
	DEPENDS gen_WDC65C02
	)
ADD_CUSTOM_TARGET(WDC65C02_table ALL DEPENDS ${CMAKE_BINARY_DIR}/opcodes_65c02_tables.c)

# --- WLA_TARGET=MCS6510
add_executable (gen_MCS6510 ${CFILES})
set_target_properties (gen_MCS6510 PROPERTIES COMPILE_DEFINITIONS "MCS6510")
# Generate tables
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/opcodes_6510_tables.c
	COMMAND gen_MCS6510
	DEPENDS gen_MCS6510
	)
ADD_CUSTOM_TARGET(MCS6510_table ALL DEPENDS ${CMAKE_BINARY_DIR}/opcodes_6510_tables.c)

# --- WLA_TARGET=W65816
add_executable (gen_W65816 ${CFILES})
set_target_properties (gen_W65816 PROPERTIES COMPILE_DEFINITIONS "W65816")
# Generate tables
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/opcodes_65816_tables.c
	COMMAND gen_W65816
	DEPENDS gen_W65816
	)
ADD_CUSTOM_TARGET(W65816_table ALL DEPENDS ${CMAKE_BINARY_DIR}/opcodes_65816_tables.c)

# --- WLA_TARGET=SPC700
add_executable (gen_SPC700 ${CFILES})
set_target_properties (gen_SPC700 PROPERTIES COMPILE_DEFINITIONS "SPC700")
# Generate tables
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/opcodes_spc700_tables.c
	COMMAND gen_SPC700
	DEPENDS gen_SPC700
	)
ADD_CUSTOM_TARGET(SPC700_table ALL DEPENDS ${CMAKE_BINARY_DIR}/opcodes_spc700_tables.c)

# --- WLA_TARGET=HUC6280
add_executable (gen_HUC6280 ${CFILES})
set_target_properties (gen_HUC6280 PROPERTIES COMPILE_DEFINITIONS "HUC6280")
# Generate tables
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/opcodes_huc6280_tables.c
	COMMAND gen_HUC6280
	DEPENDS gen_HUC6280
	)
ADD_CUSTOM_TARGET(HUC6280_table ALL DEPENDS ${CMAKE_BINARY_DIR}/opcodes_huc6280_tables.c)

