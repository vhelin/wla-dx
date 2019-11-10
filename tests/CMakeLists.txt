
set(TEST_WLA_ARCHS_DEFS)
# Generate list of arguments that includes definitions for the tests
foreach(ARCH IN LISTS ARCHS)
    string(REPLACE ":" ";" ARCH "${ARCH}")
    list(GET ARCH 0 WLA_NAME)
    list(APPEND TEST_WLA_ARCHS_DEFS
        "-DWLA_${WLA_NAME}=$<TARGET_FILE:wla-${WLA_NAME}>")
endforeach(ARCH)
list(APPEND TEST_WLA_ARCHS_DEFS "-DWLALINK=$<TARGET_FILE:wlalink>")

# Adds an test that should be assembled and linked by wla-dx.
# add_wla_test(
#       name # The name of the test
#       arch # The arch to be assembled, use "" for auto
#       dir  # Where the files are located
# )
# When arch is a empty string (""), then the test runner uses the parent
# directory name of dir to determine the arch.
function(add_wla_test NAME ARCH DIR)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${DIR}")
    add_test(NAME ${NAME}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}"
        COMMAND "${CMAKE_COMMAND}" ${TEST_WLA_ARCHS_DEFS}
            "-DARCH=${ARCH}"
            "-DCOMMON_INCLUDE=${CMAKE_CURRENT_SOURCE_DIR}/common_test.cmake"
            "-DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/${DIR}"
            "-DBINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}/${DIR}"
            "-DDIFF_EXE=${DIFF_EXE}"
            "-P" "${CMAKE_CURRENT_SOURCE_DIR}/run_test.cmake"
        )
endfunction(add_wla_test)

find_program(DIFF_EXE diff)

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
        "65816/base_test_4"
        "65816/linker_test"
        "65816/operand_hint_test"
        
        "65c02/linker_test"
        
        "6800/linker_test"
        
        "gb/appendto_test"
        "gb/background_test"
        "gb/linker_test"
        "gb/namespace_test"
        "gb/sintest"
        "gb/union_test"
        
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
    string(REPLACE "/" "-" TEST_NAME "${DIR}")
    add_wla_test("${TEST_NAME}" "" "${DIR}")
endforeach(DIR)

# Test something with ALL wla-X things, mostly for definitions etc.
foreach(DIR
        #"all/unused-section-defs-export"
        "all/negative-redefinition"
        "all/long-filename"
        )
    string(REPLACE "/" "-" TEST_NAME "${DIR}")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${DIR}")
    foreach(ARCH IN LISTS ARCHS)
        string(REPLACE ":" ";" ARCH "${ARCH}")
        list(GET ARCH 0 WLA_NAME)
        add_wla_test("${WLA_NAME}-${TEST_NAME}" "${WLA_NAME}" "${DIR}")
    endforeach(ARCH)
endforeach(DIR)

# Copy simplemap.i to the binary dir since we build from there
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    foreach(ARCH IN LISTS ARCHS "all")
        string(REPLACE ":" ";" ARCH "${ARCH}")
        list(GET ARCH 0 WLA_NAME)
        set(FILES
            "simplemap.i"
            )
        foreach(FILE IN LISTS FILES)
            if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${WLA_NAME}/${FILE}")
                configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${WLA_NAME}/${FILE}"
                    "${CMAKE_CURRENT_BINARY_DIR}/${WLA_NAME}/${FILE}"
                    COPYONLY)
            endif()
        endforeach(FILE)
    endforeach(ARCH)
endif()
