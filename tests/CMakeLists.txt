
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
            "-P" "${CMAKE_CURRENT_SOURCE_DIR}/run_test.cmake"
        )
endfunction(add_wla_test)

foreach(DIR
        )
    string(REPLACE "/" "-" TEST_NAME "${DIR}")
    add_wla_test("${TEST_NAME}" "" "${DIR}")
endforeach(DIR)

# Test something with ALL wla-X things, mostly for definitions etc.
foreach(DIR
        #"all/unused-section-defs-export"
        "all/negative-redefinition"
        )
    string(REPLACE "/" "-" TEST_NAME "${DIR}")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${DIR}")
    foreach(ARCH IN LISTS ARCHS)
        string(REPLACE ":" ";" ARCH "${ARCH}")
        list(GET ARCH 0 WLA_NAME)
        add_wla_test("${WLA_NAME}-${TEST_NAME}" "${WLA_NAME}" "${DIR}")
    endforeach(ARCH)
endforeach(DIR)

