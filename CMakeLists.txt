cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/binaries/")

set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Type of build")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release" "Debug"
    "MinSizeRel" "RelWithDebInfo")

project(WLA-DX C)

set(ARCHS
    # wla-NAME:compile-def
    "gb:GB"
    "65c02:WDC65C02"
    "65ce02:CSG65CE02"
    "6502:MCS6502"
    "6510:MCS6510"
    "65816:W65816"
    "huc6280:HUC6280"
    "spc700:SPC700"
    "z80:Z80"
    "6800:MC6800"
    "8008:I8008"
    "8080:I8080"
    "6809:MC6809"
    "6801:MC6801"
    )

option(GDB_DEBUGGING
    "Enable debugging via gdb (Only when CMAKE_BUILD_TYPE is Debug)" ON)

# CMake will automatically define WIN32 for us for Microsoft compilers, how
# convenient! But we need MSDOS flag as well. Additionally, other compilers
# don't define it by default. WIN32 does NOT include Cygwin.
if(WIN32)
    add_definitions(-DWIN32)
    add_definitions(-DMSDOS)
endif()

# This should work on ANY POSIX-compliant environment.
if(UNIX)
    add_definitions(-DUNIX)
    link_libraries(m) # Deprecated, but best solution. See
    # http://www.cmake.org/pipermail/cmake/2009-April/028439.html
endif()

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_EXTENSIONS OFF)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_STANDARD) # Empty because we use -ansi
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic-errors -Wall -Wextra -ansi")
    string(REPLACE "-O2" "-O3" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    if(CMAKE_C_COMPILER_ID MATCHES "Clang" AND
            CMAKE_C_COMPILER_VERSION VERSION_LESS 3.8)
        set(GDB_DEBUGGING OFF)
        message(STATUS "You need at least clang 3.8 for GDB_DEBUGGING."
            " Disabling it for now.")
    endif()
    if(GDB_DEBUGGING)
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb")
    endif()
endif()

if(MSVC)
    # Disable extensions, enable W3 and force warnings as errors
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Za /W3 /WX") 
    # Enable W4 + WX at your own peril, just shy of 1000 warnings there.
    
    # Per http://www.cmake.org/pipermail/cmake/2011-October/046738.html,
    # replace flags directly instead of appending them.
    string(REPLACE "/O[0-4]" "/Ox" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    add_definitions(/D_CRT_SECURE_NO_WARNINGS) # STFU about "unsecure" functions
endif()

# WATCOM C Compiler works just fine as well. Might be a decent alternative
# 386 MSDOS compiler as well (compare to DJGPP).
if(WATCOM)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -za") # Disable extensions
    string(REPLACE "-ot" "-ox" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    string(REGEX REPLACE "-w=[0-4]" "-w=4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
endif()

include_directories("${PROJECT_SOURCE_DIR}")

add_subdirectory(wlab)
add_subdirectory(wlalink)
add_subdirectory(byte_tester)
add_subdirectory(doc)

add_custom_target(generators)
add_custom_target(gens DEPENDS generators)

if(CMAKE_CROSSCOMPILING)
    set(IMPORT_GENERATORS "IMPORT_GENERATORS-NOTFOUND"
        CACHE FILEPATH "ImportGenerators.cmake of a native build")
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.5)
        set(CMAKE_CROSSCOMPILING_EMULATOR "${CMAKE_CROSSCOMPILING_EMULATOR}"
            CACHE FILEPATH "Path/Command to an emulator that can execute foreign code")
    else()
        set(CMAKE_CROSSCOMPILING_EMULATOR OFF) # If not supported, use import
    endif()
    if(CMAKE_CROSSCOMPILING_EMULATOR)
        message(STATUS "Using crosscompiling emulator: ${CMAKE_CROSSCOMPILING_EMULATOR}")
    else()
        include("${IMPORT_GENERATORS}")
        set(GEN_PREFIX native-)
    endif()
endif()


# WLA-ARCH executables
set(WLA_SRCS
    main.c
    main.h
    crc32.c
    crc32.h
    hashmap.c
    hashmap.h
    parse.c
    parse.h
    include_file.c
    include_file.h
    pass_1.c
    pass_1.h
    pass_2.c
    pass_2.h
    pass_3.c
    pass_3.h
    pass_4.c
    pass_4.h
    stack.c
    stack.h
    listfile.c
    listfile.h
    printf.c
    printf.h
    )

foreach(ARCH IN LISTS ARCHS)
    string(REPLACE ":" ";" ARCH "${ARCH}")
    list(GET ARCH 0 WLA_NAME)
    list(GET ARCH 1 COMPILE_DEF)

    # Generate opcode table
    if((NOT CMAKE_CROSSCOMPILING) OR CMAKE_CROSSCOMPILING_EMULATOR)
        add_executable(gen-${WLA_NAME}
            "opcode_table_generator/main.c"
            "opcodes_${WLA_NAME}.c"
            )
        set_property(TARGET gen-${WLA_NAME} APPEND
            PROPERTY COMPILE_DEFINITIONS
                "${COMPILE_DEF}"
            )
        set_property(TARGET gen-${WLA_NAME} PROPERTY
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/op_tbl_gen/")
        list(APPEND GENERATOR_TARGETS gen-${WLA_NAME})
        add_dependencies(generators gen-${WLA_NAME})
    endif()
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/op_tbl_gen/")
    set(TABLE_GEN_OUTPUT
        "${CMAKE_CURRENT_BINARY_DIR}/op_tbl_gen/opcodes_${WLA_NAME}_tables.c")
    add_custom_command(
        OUTPUT "${TABLE_GEN_OUTPUT}"
        COMMAND ${GEN_PREFIX}gen-${WLA_NAME}
            "${TABLE_GEN_OUTPUT}"
        )

    # Generate actual wla binary
    add_executable(wla-${WLA_NAME}
        ${WLA_SRCS} "${TABLE_GEN_OUTPUT}"
        "opcodes_${WLA_NAME}.c"
        )
    set_property(TARGET wla-${WLA_NAME} APPEND PROPERTY
        COMPILE_DEFINITIONS "${COMPILE_DEF}")
    install(TARGETS wla-${WLA_NAME} DESTINATION bin)
endforeach(ARCH)

if(NOT CMAKE_CROSSCOMPILING)
    export(TARGETS ${GENERATOR_TARGETS}
        FILE "${CMAKE_CURRENT_BINARY_DIR}/ImportGenerators.cmake"
        NAMESPACE native-
        )
endif()


# Packaging via CPack
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "Yet Another GB-Z80/Z80/6502/65C02/65CE02/6510/65816/6800/6801/6809/8008/8080/HUC6280/SPC-700 Multi Platform Cross Assembler Package")
set(CPACK_PACKAGE_CHECKSUM MD5) # CPack 3.7+
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_GENERATOR ZIP TGZ 7Z)

set(CPACK_SOURCE_GENERATOR ZIP TGZ 7Z)
set(CPACK_SOURCE_IGNORE_FILES "/.git/" "̇\\\\.swp" "#" "~" "build")

include(CPack)

