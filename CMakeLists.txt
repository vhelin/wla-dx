cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/binaries/")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release"
        CACHE STRING "Type of build")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release" "Debug"
        "MinSizeRel" "RelWithDebInfo")
endif()

project(WLA-DX C)

set(CPUS gb wdc65c02 mcs6502 mcs6510 w65816 huc6280 spc700 z80)
set(CPU_TARGET_RE "^(wdc|w|mcs)(65)")
set(CPU_TARGET_REPLACE "\\2")

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
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -pedantic-errors -Wall -Wextra")
    string(REPLACE "-O2" "-O3" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    if(CMAKE_C_COMPILER_ID MATCHES "Clang" AND
            CMAKE_C_COMPILER_VERSION VERSION_LESS 3.8)
        set(GDB_DEBUGGING OFF)
        message(STATUS "You need at least clang 3.8 for GDB_DEBUGGING."
            " Disabling it for now.")
    endif()
    if(GDB_DEBUGGING)
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb")
    endif(GDB_DEBUGGING)
endif ()

if (MSVC)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Za") # Disable extensions
    # Per http://www.cmake.org/pipermail/cmake/2011-October/046738.html,
    # replace flags directly instead of appending them.
    string(REPLACE "/O[0-4]" "/Ox" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    add_definitions(/D_CRT_SECURE_NO_WARNINGS) # STFU about "unsecure" functions
endif (MSVC)

# WATCOM C Compiler works just fine as well. Might be a decent alternative
# 386 MSDOS compiler as well (compare to DJGPP).
if (WATCOM)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -za") # Disable extensions
    string(REPLACE "-ot" "-ox" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    string(REGEX REPLACE "-w=[0-4]" "-w=4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
endif (WATCOM)

# Determine if we could use git
set(GIT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.git"
    CACHE PATH ".git directory in the repo")
mark_as_advanced(GIT_ROOT_DIR)
if(EXISTS "${GIT_ROOT_DIR}")
    find_package(Git)
    if(GIT_FOUND)
        message("Found git: ${GIT_EXECUTABLE}")
        option(USE_GIT_VERSION "Use version from git" ON)
    endif()
else()
    option(USE_GIT_VERSION "Use version from git" OFF)
endif()

set(VERSION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/VERSION"
    CACHE FILEPATH "VERSION file, containing the current version")
set(BUILD_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/BUILD_VERSION")
mark_as_advanced(VERSION_FILE)
set(VERSION_INFO_H_IN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/version-info.h.in")
set(VERSION_INFO_H_FILE "${CMAKE_CURRENT_BINARY_DIR}/version-info.h")

if(USE_GIT_VERSION)
    if(NOT GIT_FOUND) # Implicity EXISTS .git
        message(FATAL_ERROR "Couldn't find git!")
    endif()
    if(NOT EXISTS "${GIT_ROOT_DIR}")
        message(FATAL_ERROR "Coudldn't find .git dir! Please Set GIT_ROOT_DIR.")
    endif()
    add_custom_command(OUTPUT "${BUILD_VERSION_FILE}"
        COMMAND "${CMAKE_COMMAND}"
            "-DCMAKE_COMMAND=${CMAKE_COMMAND}"
            "-DGIT=${GIT_EXECUTABLE}"
            "-DGITROOT=${GIT_ROOT_DIR}"
            "-DVERSIONFILE=${VERSION_FILE}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/version-git.cmake"
        DEPENDS "${GIT_ROOT_DIR}/logs/HEAD" "${GIT_ROOT_DIR}/HEAD"
            "${CMAKE_CURRENT_SOURCE_DIR}/version-git.cmake"
            "${VERSION_INFO_H_IN_FILE}"
        COMMENT "Generating build version file (git)" VERBATIM
        )
elseif(EXISTS "${VERSION_FILE}")
    add_custom_command(OUTPUT "${BUILD_VERSION_FILE}"
        COMMAND "${CMAKE_COMMAND}"
            "-DCMAKE_COMMAND=${CMAKE_COMMAND}"
            "-DVERSIONFILE=${VERSION_FILE}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/version-manual.cmake"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION"
            "${CMAKE_CURRENT_SOURCE_DIR}/version-manual.cmake"
            "${VERSION_INFO_H_IN_FILE}"
        COMMENT "Generating version file (VERSION file)" VERBATIM
        )
else()
    message(WARNING "Couldn't find a VERSION file (set VERSION_FILE) "
        "or a valid git setup (USE_GIT_VERSION and GIT_ROOT_DIR)! "
        "Using '1.0-unkown' as version.")
    file(WRITE "${BUILD_VERSION_FILE}" "1.0\n01-01-1970\nnone")
endif()

add_custom_command(OUTPUT "${VERSION_INFO_H_FILE}"
    COMMAND "${CMAKE_COMMAND}"
        "-DCMAKE_COMMAND=${CMAKE_COMMAND}"
        "-DSRC=${VERSION_INFO_H_IN_FILE}"
        "-DDEST=${VERSION_INFO_H_FILE}"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/generate-from-version.cmake"
    DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/generate-from-version.cmake"
        "${BUILD_VERSION_FILE}"
        "${VERSION_INFO_H_IN_FILE}"
    COMMENT "Generating version header file" VERBATIM
    )
add_definitions(-DHAVE_VERSION_INFO_H)
add_custom_target(version ALL # Generate version files
    DEPENDS "${BUILD_VERSION_FILE}" "${VERSION_INFO_H_FILE}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_subdirectory(wlab)
add_subdirectory(wlalink)
add_subdirectory(doc)

add_custom_target(generators)

if(CMAKE_CROSSCOMPILING)
    set(IMPORT_GENERATORS "IMPORT_GENERATORS-NOTFOUND"
        CACHE FILEPATH "ImportGenerators.cmake of a native build")
    set(CROSSCOMPILING_EMULATOR "${CMAKE_CROSSCOMPILING_EMULATOR}"
        CACHE FILEPATH "Path/Command to an emulator that can execute foreign code")
    if(CROSSCOMPILING_EMULATOR)
        message(STATUS "Using crosscompiling emulator: ${CROSSCOMPILING_EMULATOR}")
        set(EMU "${CROSSCOMPILING_EMULATOR}") # Easier to write
    else()
        include("${IMPORT_GENERATORS}")
        set(GEN_PREFIX native-)
    endif()
endif()


# WLA-CPU executables
set(WLA_SRCS
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
    )

set(WLA_CPU_TARGETS)
foreach(CPU IN LISTS CPUS)
    # CPU_TARGET has the prefix WDC and MCS removed because they usually
    # named like that and for backwards compatibility
    string(REGEX REPLACE "${CPU_TARGET_RE}" "${CPU_TARGET_REPLACE}"
        CPU_TARGET "${CPU}")
    string(TOUPPER "${CPU}" CPU_UPPER) # For COMPILE_DEFINITION
    list(APPEND WLA_CPU_TARGETS "wla-${CPU_TARGET}")

    # Generate opcode table
    if(NOT CMAKE_CROSSCOMPILING OR CROSSCOMPILING_EMULATOR)
        add_executable(gen-${CPU_TARGET}
            "opcode_table_generator.c"
            "opcodes_${CPU_TARGET}.c"
            )
        set_property(TARGET gen-${CPU_TARGET} APPEND
            PROPERTY COMPILE_DEFINITIONS
                "${CPU_UPPER}"
            )
        set_property(TARGET gen-${CPU_TARGET} PROPERTY
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/op_tbl_gen/")
        list(APPEND GENERATOR_TARGETS gen-${CPU_TARGET})
        add_dependencies(generators gen-${CPU_TARGET})
    endif()
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/op_tbl_gen/")
    set(TABLE_GEN_OUTPUT
        "${CMAKE_BINARY_DIR}/op_tbl_gen/opcodes_${CPU_TARGET}_tables.c")
    add_custom_command(
        OUTPUT "${TABLE_GEN_OUTPUT}"
        COMMAND ${EMU} $<TARGET_FILE:${GEN_PREFIX}gen-${CPU_TARGET}> "${TABLE_GEN_OUTPUT}"
        )

    # Generate actual wla binary
    add_executable(wla-${CPU_TARGET}
        ${WLA_SRCS} "${TABLE_GEN_OUTPUT}"
        "opcodes_${CPU_TARGET}.c"
        )
    set_property(TARGET wla-${CPU_TARGET} APPEND
        PROPERTY COMPILE_DEFINITIONS
            "${CPU_UPPER}"
        )
    install(TARGETS wla-${CPU_TARGET} DESTINATION bin)
endforeach(CPU)

foreach(t wlalink wlab ${WLA_CPU_TARGETS})
    add_dependencies(${t} version)
endforeach(t)

if(NOT CMAKE_CROSSCOMPILING)
    export(TARGETS ${GENERATOR_TARGETS}
        FILE "${CMAKE_BINARY_DIR}/ImportGenerators.cmake"
        NAMESPACE native-
        )
endif()

# Testing
enable_testing()
add_custom_target(check
    COMMAND "${CMAKE_CTEST_COMMAND}" --verbose
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
add_subdirectory(tests)

# Packaging via CPack
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "Yet Another GB-Z80/Z80/6502/65C02/6510/65816/HUC6280/SPC-700 Multi Platform Cross Assembler Package")
set(CPACK_PACKAGE_CHECKSUM MD5) # CPack 3.7+
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_GENERATOR ZIP TGZ 7Z)

set(CPACK_SOURCE_GENERATOR ZIP TGZ 7Z)
set(CPACK_SOURCE_IGNORE_FILES "/.git/" "̇\\\\.swp" "#" "~" "build")

SET(CPACK_PACKAGE_VERSION "@VERSION_FULL@")
SET(CPACK_PACKAGE_VERSION_MAJOR "@VERSION_MAJOR@")
SET(CPACK_PACKAGE_VERSION_MINOR "@VERSION_MINOR@")
SET(CPACK_PACKAGE_VERSION_PATCH "@VERSION_PATCH@")

set(CPACK_OUTPUT_CONFIG_FILE
    "${CMAKE_CURRENT_BINARY_DIR}/CPackConfig.cmake.in")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE
    "${CMAKE_CURRENT_BINARY_DIR}/CPackSourceConfig.cmake.in")
include(CPack)

add_custom_command(OUTPUT CPackConfig.cmake CPackSourceConfig.cmake
    COMMAND "${CMAKE_COMMAND}"
        "-DSRCDIR=${CMAKE_CURRENT_SOURCE_DIR}"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/CPackGen.cmake"
    DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/CPackGen.cmake"
        "${BUILD_VERSION_FILE}"
        "${CPACK_OUTPUT_CONFIG_FILE}" "${CPACK_SOURCE_OUTPUT_CONFIG_FILE}"
    )
add_custom_target(gen-cpack ALL
    DEPENDS CPackConfig.cmake CPackSourceConfig.cmake)

