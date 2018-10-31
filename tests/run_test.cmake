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
# USE_CUSTOM_LINKFILE needs to be unset for now

if(EXISTS "${SOURCE_DIR}/linkfile")
    set(LINKFILE "${SOURCE_DIR}/linkfile")
elseif(EXISTS "${SOURCE_DIR}/Linkfile")
    set(LINKFILE "${SOURCE_DIR}/Linkfile")
endif()

set(OUTPUT "out.bin")
set(EXPECTED)
if(EXISTS "expected.bin")
    set(EXPECTED "expected.bin")
endif()

set(ASSEMBLE_NORMALLY ON)
set(CHECK_FILE_EQUALS OFF)
if(EXISTS "${EXPECTED}")
    set(CHECK_FILE_EQUALS ON)
endif()

set(RAN_SETUP NO)
macro(setup)
    set(RAN_SETUP YES)
    if(ASSEMBLE_NORMALLY AND EXISTS "${EXPECTED}")
        set(CHECK_FILE_EQUALS ON)
    endif()
    if(EXISTS "${LINKFILE}")
        if(NOT DEFINED USE_CUSTOM_LINKFILE)
            set(USE_CUSTOM_LINKFILE ON)
        endif()
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
    elseif(NOT SRCS)
        file(GLOB SRCS "*.s")
    endif()
endmacro(setup)

if(EXISTS "./test.cmake")
    include("./test.cmake")
endif()

if(NOT RAN_SETUP)
    setup()
endif()


if(ASSEMBLE_NORMALLY)
    # Automatic compilation
    if(USE_CUSTOM_LINKFILE)
        set(CUSTOM_LINKFILE_SETTING LINKFILE "${LINKFILE}")
    endif()
    wla_all(
        OUTPUT "${OUTPUT}"
        ARCH "${ARCH}"
        ${CUSTOM_LINKFILE_SETTING}
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

# DEBUG: Set environment variable WLA_DONT_CLEAN_OUTPUT=1 to not to clean
# the output, so you can use it as a expected output.
if(ENV:WLA_DONT_CLEAN_OUTPUT)
    message("WLA_DONT_CLEAN_OUTPUT defined! Not gonna clean output!")
    list(REMOVE_ITEM FILES_TO_CLEAN_UP "${OUTPUT}")
endif()

# Add temporary files
file(GLOB TEMP_FILES ".wla*a" ".wla*b" "wla_a.tmp" "wla_b.tmp")
list(APPEND FILES_TO_CLEAN_UP ${TEMP_FILES})

end_it()
