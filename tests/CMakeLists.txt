
set(TEST_WLA_CPUS_DEFS)
# Generate list of arguments that includes definitions for the tests
foreach(CPU IN LISTS CPUS)
    string(REGEX REPLACE "^(wdc|w|mcs)(65)" "\\2" CPU_TARGET "${CPU}")
    list(APPEND TEST_WLA_CPUS_DEFS
        "-DWLA_${CPU_TARGET}=$<TARGET_FILE:wla-${CPU_TARGET}>")
endforeach(CPU)
list(APPEND TEST_WLA_CPUS_DEFS "-DWLALINK=$<TARGET_FILE:wlalink>") # Not a CPU

foreach(DIR
        "6502/compiler_test"
        "6502/linker_test"
        "6502/operand_hint_test"
        "6502/struct_test"
        
        "6510/zero_page"
        "6510/linker_test"
        "6510/operand_hint_test"
        
        "65816/base_test_1"
        "65816/base_test_2"
        "65816/base_test_3"
        "65816/linker_test"
        "65816/operand_hint_test"
        
        "65c02/linker_test"
        
        "gb/appendto_test"
        "gb/background_test"
        "gb/linker_test"
        "gb/namespace_test"
        "gb/sintest"
        
        "huc6280/linker_test"
        "huc6280/ram_sections"
        
        "spc700/linker_test"
        
        "z80/linker_header_test"
        "z80/linker_test_1"
        "z80/linker_test_2"
        "z80/rept_test"
        "z80/sdsc_test"
        "z80/sms_test"
        "z80/smsheader_test"
        "z80/ram_sections"
        )
    string(REPLACE "/" "-" TEST_TARGET "${DIR}")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${DIR}")
    add_test(NAME ${TEST_TARGET}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}"
        COMMAND "${CMAKE_COMMAND}" ${TEST_WLA_CPUS_DEFS}
            "-DCOMMON_INCLUDE=${CMAKE_CURRENT_SOURCE_DIR}/common-test.cmake"
            "-DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/${DIR}"
            "-DBINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}/${DIR}"
            "-P" "${CMAKE_CURRENT_SOURCE_DIR}/run_test.cmake"
        )
endforeach(DIR)

# Test something with ALL wla-X things, mostly for definitions etc.
foreach(DIR
        "all/unused-section-defs-export"
        )
    string(REPLACE "/" "-" TEST_TARGET "${DIR}")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${DIR}")
    foreach(CPU IN LISTS CPUS)
        string(REGEX REPLACE "${CPU_TARGET_RE}" "${CPU_TARGET_REPLACE}"
            CPU_TARGET "${CPU}")
        add_test(NAME ${TEST_TARGET}-${CPU_TARGET}
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}"
            COMMAND "${CMAKE_COMMAND}" ${TEST_WLA_CPUS_DEFS}
                "-DCPU=${CPU_TARGET}"
                "-DCOMMON_INCLUDE=${CMAKE_CURRENT_SOURCE_DIR}/common-test.cmake"
                "-DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/${DIR}"
                "-DBINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}/${DIR}"
                "-P" "${CMAKE_CURRENT_SOURCE_DIR}/run_test.cmake"
            )
    endforeach(CPU)
endforeach(DIR)

