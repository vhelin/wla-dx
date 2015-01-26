# Main CMakeLists file
cmake_minimum_required (VERSION 2.8.9)

# --- Project name ---
project (WLA-DX)

# --- Explicitly enable C
enable_language (C)

# --- GCC flags 
if (CMAKE_COMPILER_IS_GNUCC)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -ansi -pedantic-errors -Wall -Wextra -lm")
endif (CMAKE_COMPILER_IS_GNUCC)

# --- Tell MSVC to STFU about _CRT_SECURE_NO_WARNINGS
if (MSVC)
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
endif (MSVC)

#Make the source tree available to includes
include_directories(${PROJECT_SOURCE_DIR})
#Generated tables are created at the top build-tree directory
include_directories(${CMAKE_BINARY_DIR})

# --- Dependencies ---
add_subdirectory (opcode_table_generator)
add_subdirectory (wlab)
add_subdirectory (wlalink)

# --- WLA executables ---
set(CFILES main.c parse.c include_file.c pass_1.c pass_2.c pass_3.c pass_4.c stack.c listfile.c)

#GB
add_executable(wla-gb ${CFILES})
set_target_properties (wla-gb PROPERTIES COMPILE_DEFINITIONS "GB" )
add_dependencies(wla-gb GB_table)

#WDC65C02
add_executable(wla-65c02 ${CFILES})
set_target_properties (wla-65c02 PROPERTIES COMPILE_DEFINITIONS "WDC65C02" )
add_dependencies(wla-65c02 WDC65C02_table)

#MCS6502
add_executable(wla-6502 ${CFILES})
set_target_properties (wla-6502 PROPERTIES COMPILE_DEFINITIONS "MCS6502" )
add_dependencies(wla-6502 MCS6502_table)

#MCS6510
add_executable(wla-6510 ${CFILES})
set_target_properties (wla-6510 PROPERTIES COMPILE_DEFINITIONS "MCS6510" )
add_dependencies(wla-6510 MCS6510_table)

#W65816
add_executable(wla-65816 ${CFILES})
set_target_properties (wla-65816 PROPERTIES COMPILE_DEFINITIONS "W65816" )
add_dependencies(wla-65816 W65816_table)

#HUC6280
add_executable(wla-huc6280 ${CFILES})
set_target_properties (wla-huc6280 PROPERTIES COMPILE_DEFINITIONS "HUC6280" )
add_dependencies(wla-huc6280 HUC6280_table)

#SPC700
add_executable(wla-spc700 ${CFILES})
set_target_properties (wla-spc700 PROPERTIES COMPILE_DEFINITIONS "SPC700" )
add_dependencies(wla-spc700 SPC700_table)

#Z80
add_executable(wla-z80 ${CFILES})
set_target_properties (wla-z80 PROPERTIES COMPILE_DEFINITIONS "Z80" )
add_dependencies(wla-z80 Z80_table)


