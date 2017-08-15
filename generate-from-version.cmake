if(NOT EXISTS "BUILD_VERSION")
    message(FATAL_ERROR "Couldn't find version file BUILD_VERSION!")
endif()

if(NOT BUILD_VERSION_FILE)
    set(BUILD_VERSION_FILE "BUILD_VERSION")
endif()

file(READ "${BUILD_VERSION_FILE}" VERSION_FILE_STRING)
string(REPLACE "\n" ";" VERSION_FILE_LIST "${VERSION_FILE_STRING}")
list(GET VERSION_FILE_LIST 0 VERSION_FULL)
list(GET VERSION_FILE_LIST 1 VERSION_DATE)
list(GET VERSION_FILE_LIST 2 VERSION_METHOD)

string(REGEX MATCH "([0-9]+)\\.([0-9]+)(\\.([0-9]+))?"
    VERSION_TMP "${VERSION_FULL}")
set(VERSION_MAJOR "${CMAKE_MATCH_1}")
set(VERSION_MINOR "${CMAKE_MATCH_2}")
set(VERSION_PATCH "${CMAKE_MATCH_4}")
if(NOT VERSION_MAJOR)
    set(VERSION_MAJOR 0)
endif()
if(NOT VERSION_MINOR)
    set(VERSION_MINOR 0)
endif()
if(NOT VERSION_PATCH)
    set(VERSION_PATCH 0)
endif()

set(VERSION_2DOTTED "${VERSION_MAJOR}.${VERSION_MINOR}")
set(VERSION_3DOTTED "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
string(REGEX MATCH "([1-9]?[0-9])-([1-9]?[0-9]+)-([1-9][0-9]+)"
    VERSION_DATE "${VERSION_DATE}")
set(VERSION_DATE_AMIGA "${CMAKE_MATCH_3}.${CMAKE_MATCH_2}.${CMAKE_MATCH_1}")

if(DEFINED DEST)
    file(REMOVE "${DEST}.tmp")
    configure_file("${SRC}" "${DEST}.tmp" @ONLY)
    execute_process(
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${DEST}.tmp" "${DEST}"
        RESULT_VARIABLE RESULT
        )

    if(NOT RESULT EQUAL 0)
        message(FATAL_ERROR "Couldn't copy_if_different: ${RESULT}")
    endif()

    file(REMOVE "${DEST}.tmp")
endif()
