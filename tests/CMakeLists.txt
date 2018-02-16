
set(TEST_WLA_ARCHS_DEFS)
# Generate list of arguments that includes definitions for the tests
foreach(ARCH IN LISTS ARCHS)
    string(REPLACE ":" ";" ARCH "${ARCH}")
    list(GET ARCH 0 WLA_NAME)
    list(APPEND TEST_WLA_ARCHS_DEFS
        "-DWLA_${WLA_NAME}=$<TARGET_FILE:wla-${WLA_NAME}>")
endforeach(ARCH)
list(APPEND TEST_WLA_ARCHS_DEFS "-DWLALINK=$<TARGET_FILE:wlalink>")

foreach(DIR
        )
    string(REPLACE "/" "-" TEST_TARGET "${DIR}")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${DIR}")
    add_test(NAME ${TEST_TARGET}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}"
        COMMAND "${CMAKE_COMMAND}" ${TEST_WLA_ARCHS_DEFS}
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
    foreach(ARCH IN LISTS ARCHS)
        string(REGEX REPLACE "${ARCH_TARGET_RE}" "${ARCH_TARGET_REPLACE}"
            ARCH_TARGET "${ARCH}")
        add_test(NAME ${TEST_TARGET}-${ARCH_TARGET}
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}"
            COMMAND "${CMAKE_COMMAND}" ${TEST_WLA_ARCHS_DEFS}
                "-DARCH=${ARCH_TARGET}"
                "-DCOMMON_INCLUDE=${CMAKE_CURRENT_SOURCE_DIR}/common-test.cmake"
                "-DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/${DIR}"
                "-DBINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}/${DIR}"
                "-P" "${CMAKE_CURRENT_SOURCE_DIR}/run_test.cmake"
            )
    endforeach(ARCH)
endforeach(DIR)

