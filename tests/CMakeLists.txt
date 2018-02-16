
set(TEST_WLA_CPUS_DEFS)
# Generate list of arguments that includes definitions for the tests
foreach(CPU IN LISTS CPUS)
    string(REGEX REPLACE "^(wdc|w|mcs)(65)" "\\2" CPU_TARGET "${CPU}")
    list(APPEND TEST_WLA_CPUS_DEFS
        "-DWLA_${CPU_TARGET}=$<TARGET_FILE:wla-${CPU_TARGET}>")
endforeach(CPU)
list(APPEND TEST_WLA_CPUS_DEFS "-DWLALINK=$<TARGET_FILE:wlalink>") # Not a CPU

foreach(DIR
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

