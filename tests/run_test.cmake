include("${COMMON_INCLUDE}")

# Determine ARCH if not set
if(NOT ARCH)
    string(REGEX MATCH "([^/]+)/[^/]+/?$" TMPVAR "${SOURCE_DIR}")
    set(ARCH "${CMAKE_MATCH_1}")
endif()
set(SRCS)
set(LIBSRCS)
set(OBJECTS)
set(LIBRARIES)
set(DEFS)
set(LDEFS)
set(WLA_FLAGS -x)
set(LINK_FLAGS )
set(LINKFILE)


if(EXISTS "${SOURCE_DIR}/linkfile")
    set(LINKFILE "${SOURCE_DIR}/linkfile")
elseif(EXISTS "${SOURCE_DIR}/Linkfile")
    set(LINKFILE "${SOURCE_DIR}/Linkfile")
endif()

if(LINKFILE)
    read_linkfile(
        LINKFILE "${LINKFILE}"
        OBJECTS SRCS_PRE
        #LIBRARIES LIBSRCS_PRE
        DEFINITIONS LDEFS
        #HEADER HEADER
        #FOOTER FOOTER
        )
    # Convert from objects to source files
    foreach(OBJ IN LISTS SRCS_PRE)
        foreach(REGEXP "(.)$;\\1.s" "\\.o;.s")
            list(GET REGEXP 0 MATCH)
            list(GET REGEXP 1 REPLACEMENT)
            string(REGEX REPLACE "${MATCH}" "${REPLACEMENT}" FILENAME "${OBJ}")
            if(EXISTS "${SOURCE_DIR}/${FILENAME}")
                list(APPEND SRCS "${SOURCE_DIR}/${FILENAME}")
                break()
            endif(EXISTS "${SOURCE_DIR}/${FILENAME}")
        endforeach(REGEXP)
    endforeach(OBJ)
    # TODO: Libraries
else()
    file(GLOB SRCS "*.s")
endif()

set(OUTPUT "out.bin")
set(EXPECTED "expected.bin")

if(EXISTS "./test.cmake")
    set(ASSEMBLE_NORMALLY OFF)
    set(CHECK_FILE_EQUALS OFF)
    include("./test.cmake")
else()
    set(ASSEMBLE_NORMALLY ON)
    if(EXISTS "${EXPECTED}")
        set(CHECK_FILE_EQUALS ON)
    else()
        set(CHECK_FILE_EQUALS OFF)
    endif()
endif()

if(ASSEMBLE_NORMALLY)
    # Automatic compilation
    wla_all(
        OUTPUT "${OUTPUT}"
        ARCH "${ARCH}"
        SOURCES ${SRCS}
        LIBSOURCES ${LIBSRCS}
        OBJECTS ${OBJECTS}
        LIBRARIES ${LIBRARIES}
        DEFINES ${DEFS}
        LINK_DEFINES ${LDEFS}
        WLA_FLAGS ${WLA_FLAGS}
        LINK_FLAGS ${LINK_FLAGS}
        VERBOSE
        )
    if(EXISTS "./test.cmake" AND EXISTS "${EXPECTED}")
        set(CHECK_FILE_EQUALS ON)
    endif()
endif()

if(CHECK_FILE_EQUALS)
    if(EXPECTED)
        abort_if_file_unequal("${OUTPUT}" "${EXPECTED}"
            "Output file doesn't equals expected file!")
    endif(EXPECTED)
endif()

# Add temporary files
file(GLOB TEMP_FILES ".wla*a" ".wla*b" "wla_a.tmp" "wla_b.tmp")
list(APPEND FILES_TO_CLEAN_UP ${TEMP_FILES})

end_it()
