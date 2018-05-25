cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/binaries/")

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Type of build")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release" "Debug"
    "MinSizeRel" "RelWithDebInfo")

project(WLA-DX C)

# --- Explicitly enable C
enable_language (C)

option(GDB_DEBUGGING
    "Enable debugging via gdb (Only when CMAKE_BUILD_TYPE is Debug)" ON)

# CMake will automatically define WIN32 for us for Microsoft compilers, how
# convenient! But we need MSDOS flag as well. Additionally, other compilers
# don't define it by default. WIN32 does NOT include Cygwin.
if (WIN32)
    add_definitions(-DWIN32)
    add_definitions(-DMSDOS)
endif (WIN32)

# This should work on ANY POSIX-compliant environment.
if (UNIX)
    add_definitions(-DUNIX)
    link_libraries(m) # Deprecated, but best solution. See
    # http://www.cmake.org/pipermail/cmake/2009-April/028439.html
endif (UNIX)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -pedantic-errors -Wall -Wextra")
    string(REPLACE "-O2" "-O3" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    if(GDB_DEBUGGING)
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb")
    endif(GDB_DEBUGGING)
endif ()

if (MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Za") # Disable extensions
    # Per http://www.cmake.org/pipermail/cmake/2011-October/046738.html,
    # replace flags directly instead of appending them.
    string(REPLACE "/O[0-4]" "/Ox" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    add_definitions(/D_CRT_SECURE_NO_WARNINGS) # STFU about "unsecure" functions
endif (MSVC)

# WATCOM C Compiler works just fine as well. Might be a decent alternative
# 386 MSDOS compiler as well (compare to DJGPP).
if (WATCOM)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -za") # Disable extensions
    string(REPLACE "-ot" "-ox" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    string(REGEX REPLACE "-w=[0-4]" "-w=4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
endif (WATCOM)

# Make the source tree available to includes
include_directories(${PROJECT_SOURCE_DIR})
# Generated tables are created at the top build-tree directory
include_directories(${CMAKE_BINARY_DIR})

# --- Dependencies ---
add_subdirectory (opcode_table_generator)
add_subdirectory (wlab)
add_subdirectory (wlalink)

# Generate documentation
add_subdirectory (doc)

# --- WLA executables ---
set(CFILES main.c crc32.c hashmap.c parse.c include_file.c pass_1.c pass_2.c pass_3.c pass_4.c stack.c listfile.c)

# GB
add_executable(wla-gb ${CFILES})
set_target_properties (wla-gb PROPERTIES COMPILE_DEFINITIONS "GB" )
add_dependencies(wla-gb GB_table)
add_custom_command(TARGET wla-gb POST_BUILD COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen_GB*")

# WDC65C02
add_executable(wla-65c02 ${CFILES})
set_target_properties (wla-65c02 PROPERTIES COMPILE_DEFINITIONS "WDC65C02" )
add_dependencies(wla-65c02 WDC65C02_table)
add_custom_command(TARGET wla-65c02 POST_BUILD COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen_WDC65C02*")

# MCS6502
add_executable(wla-6502 ${CFILES})
set_target_properties (wla-6502 PROPERTIES COMPILE_DEFINITIONS "MCS6502" )
add_dependencies(wla-6502 MCS6502_table)
add_custom_command(TARGET wla-6502 POST_BUILD COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen_MCS6502*")

# MCS6510
add_executable(wla-6510 ${CFILES})
set_target_properties (wla-6510 PROPERTIES COMPILE_DEFINITIONS "MCS6510" )
add_dependencies(wla-6510 MCS6510_table)
add_custom_command(TARGET wla-6510 POST_BUILD COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen_MCS6510*")

# W65816
add_executable(wla-65816 ${CFILES})
set_target_properties (wla-65816 PROPERTIES COMPILE_DEFINITIONS "W65816" )
add_dependencies(wla-65816 W65816_table)
add_custom_command(TARGET wla-65816 POST_BUILD COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen_W65816*")

# HUC6280
add_executable(wla-huc6280 ${CFILES})
set_target_properties (wla-huc6280 PROPERTIES COMPILE_DEFINITIONS "HUC6280" )
add_dependencies(wla-huc6280 HUC6280_table)
add_custom_command(TARGET wla-huc6280 POST_BUILD COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen_HUC6280*")

# SPC700
add_executable(wla-spc700 ${CFILES})
set_target_properties (wla-spc700 PROPERTIES COMPILE_DEFINITIONS "SPC700" )
add_dependencies(wla-spc700 SPC700_table)
add_custom_command(TARGET wla-spc700 POST_BUILD COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen_SPC700*")

# Z80
add_executable(wla-z80 ${CFILES})
set_target_properties (wla-z80 PROPERTIES COMPILE_DEFINITIONS "Z80" )
add_dependencies(wla-z80 Z80_table)
add_custom_command(TARGET wla-z80 POST_BUILD COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen_Z80*")

install(TARGETS wla-gb wla-65c02 wla-6510 wla-65816 wla-huc6280 wla-spc700 wla-z80
	DESTINATION bin)
