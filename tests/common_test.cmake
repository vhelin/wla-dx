# Common functions and stuff that should be included in every .cmake-file
# that tests something

include(CMakeParseArguments)

set(FILES_TO_CLEAN_UP) # List of files to delete when finished

# Clean up, print message and exit with an error
macro(abort MESSAGE)
    end_it()
    message(FATAL_ERROR "${MESSAGE}")
endmacro(abort)

# Error when RESULT is not 0 wtih "Failed to VERB SRC: PROG returned RESULT"
macro(abort_result RESULT VERB PROG SRC)
    if(NOT RESULT EQUAL 0)
        abort("Failed to ${VERB} ${SRC}: ${PROG} returned ${RESULT}")
    endif(NOT RESULT EQUAL 0)
endmacro(abort_result)

# Cleanup, shouldn't be used manually
macro(end_it)
    foreach(FILE IN LISTS FILES_TO_CLEAN_UP)
        if(EXISTS "${FILE}")
            message(STATUS "[CLEAN] Removing '${FILE}'...")
            file(REMOVE "${FILE}")
        else(NOT EXISTS "${FILE}")
            message(STATUS "[CLEAN] Missing: '${FILE}'")
        endif()
    endforeach(FILE)
endmacro(end_it)

macro(convert_to_binary_path PATH OUTPUT_VAR)
    if(NOT IS_ABSOLUTE "${PATH}")
        set(${OUTPUT_VAR} "${BINARY_DIR}/${PATH}")
    else(IS_ABSOLUTE "${PATH}")
        string(REPLACE "${SOURCE_DIR}" "${BINARY_DIR}" ${OUTPUT_VAR} "${PATH}")
    endif()
endmacro(convert_to_binary_path)

function(get_full_path PATH PREFIX OUTPUT_VAR)
    if(NOT PREFIX)
        abort("Missing prefix for '${PATH}'!")
    endif(NOT PREFIX)
    if(NOT IS_ABSOLUTE "${PATH}")
        set(${OUTPUT_VAR} "${PREFIX}/${PATH}" PARENT_SCOPE)
    endif(NOT IS_ABSOLUTE "${PATH}")
endfunction()

function(file_equal A B OUT_VAR)
    if(NOT EXISTS "${A}")
        abort("Couldn't find file A '${A}'!")
    elseif(NOT EXISTS "${B}")
        abort("Found file A, but couldn't find file B '${B}'!")
    else()
        # Just check with hashes
        file(MD5 "${A}" A_HASH)
        file(MD5 "${B}" B_HASH)
        if(A_HASH STREQUAL B_HASH)
            set(${OUT_VAR} ON PARENT_SCOPE)
        else(NOT A_HASH STREQUAL B_HASH)
            set(${OUT_VAR} OFF PARENT_SCOPE)
        endif()
    endif()
endfunction(file_equal)

function(abort_if_file_unequal A B MESSAGE)
    file_equal("${A}" "${B}" FILE_EQUAL)
    if(NOT FILE_EQUAL)
        abort("${MESSAGE}")
    endif(NOT FILE_EQUAL)
endfunction(abort_if_file_unequal)

# (Internal function: Print the COMMANDs used)
function(execute_process_print)
    set(options OUTPUT_QUIET ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_STRIP_TRAILING_WHITESPACE)
    set(oneValueArgs WORKING_DIRECTORY RESULT_VARIABLE OUTPUT_VARIABLE
        ERROR_VARIABLE INPUT_FILE OUTPUT_FILE ERROR_FILE)
    set(multiValueArgs COMMAND)
    cmake_parse_arguments(TW "${options}" "${oneValueArgs}"
        "${multiValueArgs}" ${ARGN})
    
    set(CMD)
    foreach(PART IN LISTS TW_COMMAND)
        if(PART MATCHES "[ ]")
            list(APPEND CMD "\"${PART}\"")
        else()
            list(APPEND CMD "${PART}")
        endif()
    endforeach(PART)
    string(REPLACE ";" " " CMD "${CMD}")
    message(STATUS "[CMD] ${CMD}")
    
    if(TW_WORKING_DIRECTORY)
        message(STATUS "[CMD WD] (explicit) ${TW_WORKING_DIRECTORY}")
    else()
        # TODO: Cross platform
        message(STATUS "[CMD WD] (implicit) ${ENV:PWD}")
    endif()
endfunction(execute_process_print)
# Print the commands executed and execute them, see execute_process()
macro(execute_process_cmd)
    execute_process_print(${ARGN}) # Prints stuff based on it
    execute_process(${ARGN})       # Run original function
endmacro(execute_process_cmd)

# Assemble a file to an object/library file
# wla(
#     <OBJECT|LIBRARY> # Whenever to comile either an object or an library
#     ARCH arch         # Use wla-ARCH
#     SOURCES source [source]... # Source files
#     [OUT_VAR var]    # Variable to put list of object files
#     [DEFINES def=value [def=value]...] # Defines
#     [FLAGS flag [flag]...]  # Extra flags
#     [VERBOSE]               # Be verbose
#     )
function(wla)
    set(options OBJECT LIBRARY VERBOSE)
    set(oneValueArgs ARCH OUT_VAR)
    set(multiValueArgs SOURCES DEFINES FLAGS)
    cmake_parse_arguments(TW "${options}" "${oneValueArgs}"
        "${multiValueArgs}" ${ARGN})
    
    if(NOT (TW_OBJECT OR TW_LIBRARY))
        abort("Please put either OBJECT or LIBRARY for wla!")
    endif(NOT (TW_OBJECT OR TW_LIBRARY))
    
    if(TW_OBJECT AND TW_LIBRARY)
        abort("You can't create an OBJECT and an LIBRARY at the same time!")
    endif(TW_OBJECT AND TW_LIBRARY)
    
    if(NOT WLA_${TW_ARCH})
        abort("Couldn't find ARCH '${TW_ARCH}'")
    endif(NOT WLA_${TW_ARCH})
    
    if(TW_VERBOSE)
        list(APPEND TW_FLAGS "-v")
    endif(TW_VERBOSE)
    
    foreach(DEFINE IN LISTS TW_DEFINES)
        list(APPEND TW_FLAGS "-D${DEFINE}")
    endforeach(DEFINE)
    
    set(OUT_SRCS)
    foreach(SRC IN LISTS TW_SOURCES)
        convert_to_binary_path("${SRC}.o" OUT_SRC)
        get_full_path("${SRC}" "${SOURCE_DIR}" SRC)
        execute_process_cmd(
            COMMAND "${WLA_${TW_ARCH}}" ${TW_FLAGS} -o "${OUT_SRC}" "${SRC}"
            WORKING_DIRECTORY "${SOURCE_DIR}"
            RESULT_VARIABLE RESULT
            )
        list(APPEND OUT_SRCS "${OUT_SRC}")
        abort_result("${RESULT}" "assemble" "wla-${TW_ARCH}" "${SRC}")
    endforeach(SRC)
    list(APPEND FILES_TO_CLEAN_UP ${OUT_SRCS})
    
    if(TW_OUT_VAR)
        set(${TW_OUT_VAR} ${OUT_SRCS} PARENT_SCOPE)
    endif(TW_OUT_VAR)
    set(FILES_TO_CLEAN_UP ${FILES_TO_CLEAN_UP} PARENT_SCOPE)
endfunction(wla)

# Create a linkfile
# create_linkfile(
#     OUTPUT output     # Output linkfile
#     [OBJECTS object [object]...]     # Object files
#     [LIBRARIES library [library]...] # Library files
#     [DEFINES key=value [key=value]...] # Defines
#     [PREPEND_STR str] # Prepend "str" to file
#     [APPEND_STR str]  # Append "str" to file
#     )
function(create_linkfile)
    set(options)
    set(oneValueArgs OUTPUT PREPEND_STR APPEND_STR)
    set(multiValueArgs OBJECTS LIBRARIES DEFINES)
    cmake_parse_arguments(TL "${options}" "${oneValueArgs}"
        "${multiValueArgs}" ${ARGN})
    
    if(NOT TL_OUTPUT)
        abort("Please specify a output name for the linkfile!")
    endif(NOT TL_OUTPUT)
    
    get_full_path("${TL_OUTPUT}" "${BINARY_DIR}" OUTPUT)
    list(APPEND FILES_TO_CLEAN_UP "${LINKFILE}")
    file(WRITE "${LINKFILE}" "${TL_PREPEND_STR}\n")
    file(APPEND "${LINKFILE}" "[objects]\n")
    foreach(OBJ IN LISTS TL_OBJECTS)
        file(APPEND "${LINKFILE}" "${OBJ}\n")
    endforeach(OBJ)
    # TODO: Implement libraries in linkfile correctly
    if(TL_DEFINES)
        file(APPEND "${LINKFILE}" "[definitions]\n")
        foreach(DEF IN LISTS TL_DEFINES)
            string(REPLACE "=" ";" DEF "${DEF}")
            list(GET DEF 0 KEY)
            list(GET DEF 1 VALUE)
            file(APPEND "${LINKFILE}" "${KEY} ${VALUE}\n")
        endforeach(DEF)
    endif(TL_DEFINES)
    file(APPEND "${LINKFILE}" "${TL_APPEND_STR}\n")
    set(FILES_TO_CLEAN_UP ${FILES_TO_CLEAN_UP} PARENT_SCOPE)
endfunction(create_linkfile)

# Link using a linkfile
# wlalink(
#     OUTPUT output     # Output file
#     LINKFILE linkfile # Linkfile to use
#     [FLAGS flag [flag]...]  # Extra flags
#     [VERBOSE]               # Be verbose
#     [SYMBOL_WLA]      # Output WLA symbols
#     [SYMBOL_NOGMB]    # Output NO$GMB symbols
#     )
function(wlalink)
    set(options VERBOSE)
    set(oneValueArgs OUTPUT LINKFILE)
    set(multiValueArgs FLAGS SYMBOL_WLA SYMBOL_NOGMB)
    cmake_parse_arguments(TW "${options}" "${oneValueArgs}"
        "${multiValueArgs}" ${ARGN})
    
    if(NOT TW_OUTPUT)
        abort("Please specify a output name for the binary!")
    endif(NOT TW_OUTPUT)
    
    if(NOT TW_LINKFILE)
        abort("Please specify a linkfile!")
    endif(NOT TW_LINKFILE)
    
    if(TW_SYMBOL_WLA)
        list(APPEND TW_FLAGS -S)
    endif(TW_SYMBOL_WLA)
    
    if(TW_SYMBOL_NOGMB)
        list(APPEND TW_FLAGS -s)
    endif(TW_SYMBOL_NOGMB)
    
    if(TW_VERBOSE)
        list(APPEND TW_FLAGS -v)
    endif(TW_VERBOSE)
    
    execute_process_cmd(
        COMMAND "${WLALINK}" ${TW_FLAGS} "${LINKFILE}" "${OUTPUT}"
        WORKING_DIRECTORY "${SOURCE_DIR}"
        RESULT_VARIABLE RESULT
        )
    list(APPEND FILES_TO_CLEAN_UP "${OUTPUT}")
    abort_result("${RESULT}" "link" "wlalink" "${OUTPUT}")
    set(FILES_TO_CLEAN_UP ${FILES_TO_CLEAN_UP} PARENT_SCOPE)
endfunction(wlalink)

# All-in-one function
# wla(
#     OUTPUT output  # Output file, shouldn't matter
#     ARCH arch       # ARCH, wla-ARCH
#     SOURCES source1 [source2]...       # Soruce files
#     [LIBSOURCES source1 [source2]...]  # Library source files
#     [OBJECTS object [object]...]       # Additional object files
#     [LIBRARIES library [library]...]   # Additional library files
#     [DEFINES def=value [dev=value]...] # Definitions
#     [LINK_DEFINES def=value [dev=value]...] # Linktime definitions
#     [WLA_FLAGS flag1 [flag2]...]       # Extra flags passed to wla-ARCH
#     [LINK_FLAGS flag1 [flag2]...]      # Extra flags passed to wlalink
#     [WLA_VERBOSE]  # wla-ARCH is verbose
#     [LINK_VERBOSE] # wlalink is verbose
#     [VERBOSE]      # WLA_VERBOSE + LINK_VERBOSE
#     [SYMBOL_WLA]   # Output WLA symbols
#     [SYMBOL_NOGMB] # Output NO$GMB symbols
#   )
function(wla_all)
    set(options WLA_VERBOSE LINK_VERBOSE VERBOSE
        SYMBOL_WLA SYMBOL_NOGMB)
    set(oneValueArgs OUTPUT ARCH)
    set(multiValueArgs SOURCES LIBSOURCES OBJECTS LIBRARIES
        DEFINES LINK_DEFINES WLA_FLAGS LINK_FLAGS)
    cmake_parse_arguments(TW "${options}" "${oneValueArgs}"
        "${multiValueArgs}" ${ARGN})
    
    if(TW_WLA_VERBOSE OR TW_VERBOSE)
        set(WLA_VERBOSE VERBOSE)
    endif(TW_WLA_VERBOSE OR TW_VERBOSE)
    
    if(TW_LINK_VERBOSE OR TW_VERBOSE)
        set(LINK_VERBOSE VERBOSE)
    endif(TW_LINK_VERBOSE OR TW_VERBOSE)
    
    wla(
        OBJECT ARCH "${TW_ARCH}"
        SOURCES ${TW_SOURCES}
        OUT_VAR OBJECTS
        DEFINES ${TW_DEFINES}
        FLAGS ${TW_WLA_FLAGS}
        ${WLA_VERBOSE}
        )
    
    wla(
        LIBRARY ARCH "${TW_ARCH}"
        SOURCES ${TW_LIBSOURCES}
        OUT_VAR LIBRARIES
        DEFINES ${TW_DEFINES}
        FLAGS ${TW_WLA_FLAGS}
        ${WLA_VERBOSE}
        )
    
    list(APPEND OBJECTS ${TW_OBJECTS})
    list(APPEND LIBRARIES ${TW_LIBRARIES})
    
    get_full_path("${TW_OUTPUT}.link" "${BINARY_DIR}" LINKFILE)
    create_linkfile(
        OUTPUT "${LINKFILE}"
        OBJECTS ${OBJECTS}
        LIBRARIES ${LIBRARIES}
        DEFINES ${TW_LINK_DEFINES}
        )
    file(READ "${LINKFILE}" LINKFILE_CONTENT) # For easier debugging
    message(STATUS "[LINKFILE CONTENTS] ${LINKFILE}\n${LINKFILE_CONTENT}")
    
    set(SYMBOLS)
    if(TW_SYMBOL_WLA)
        list(APPEND SYMBOLS SYMBOL_WLA)
    endif(TW_SYMBOL_WLA)
    if(TW_SYMBOL_NOGMB)
        list(APPEND SYMBOLS SYMBOL_NOGMB)
    endif(TW_SYMBOL_NOGMB)
    
    get_full_path("${TW_OUTPUT}" "${BINARY_DIR}" OUTPUT)
    wlalink(
        OUTPUT "${OUTPUT}"
        LINKFILE "${LINKFILE}"
        FLAGS ${TW_LINK_FLAGS}
        ${LINK_VERBOSE} ${SYMBOLS}
        )
    
    set(FILES_TO_CLEAN_UP ${FILES_TO_CLEAN_UP} PARENT_SCOPE)
endfunction(wla_all)

# Reads an linkfile and extract properties from it
# read_linkfile(
#     LINKFILE linkfile         # Linkfile to look at
#     [OBJECTS object_var]      # Variable to store the [objects]
#     [LIBRARIES libraries_var] # Variable to store the [libraries]
#     [DEFINITIONS defs_var]    # Variable to store the [definitions]
#     [HEADER header_var]       # Variable to store the [header]
#     [FOOTER footer_var]       # Variable to store the [footer]
#     )
function(read_linkfile)
    set(options)
    set(oneValueArgs LINKFILE OBJECTS LIBRARIES DEFINITIONS HEADER FOOTER)
    set(multiValueArgs)
    cmake_parse_arguments(RL "${options}" "${oneValueArgs}"
        "${multiValueArgs}" ${ARGN})
    
    if(NOT LINKFILE)
        abort("Please specify LINKFILE!")
    endif(NOT LINKFILE)
    
    file(READ "${RL_LINKFILE}" CONTENT)
    string(REGEX REPLACE ";" "\\\\;" CONTENT "${CONTENT}")
    string(REGEX REPLACE "\n" ";" CONTENT "${CONTENT}")
    set(SECTION "")
    set(OBJECTS)
    set(LIBRARIES)
    set(DEFINITONS)
    
    foreach(LINE IN LISTS CONTENT)
        string(REGEX MATCH "^[ \t]+$" IS_EMPTY "${LINE}")
        string(REGEX MATCH "\\[([a-zA-Z0-9_-]+)\\]" IS_SECTION "${LINE}")
        if(IS_EMPTY OR LINE STREQUAL "") # Ignore empty lines
        elseif(IS_SECTION)
            set(SECTION "${CMAKE_MATCH_1}")
        elseif(SECTION STREQUAL "objects")
            list(APPEND OBJECTS "${LINE}")
        elseif(SECTION STREQUAL "libraries")
            list(APPEND LIBRARIES "${LINE}")
        elseif(SECTION STREQUAL "definitions")
            string(REGEX MATCH "([^ ]+) +(.*)" IS_DEFS "${LINE}")
            if(NOT IS_DEFS)
                abort("Failed to parse linkfile '${RL_LINKFILE}': Not a definition: '${LINE}'")
            endif(NOT IS_DEFS)
            list(APPEND DEFINITIONS "${CMAKE_MATCH_1}=${CMAKE_MATCH_2}")
        elseif(SECTION STREQUAL "header")
            if(RL_HEADER)
                set("${RL_HEADER}" "${LINE}" PARENT_SCOPE)
            endif(RL_HEADER)
        elseif(SECTION STREQUAL "footer")
            if(RL_FOOTER)
                set("${RL_FOOTER}" "${LINE}" PARENT_SCOPE)
            endif(RL_FOOTER)
        else()
            message("[LINKFILE READER] Unknown! Section: '${SECTION}', line: '${LINE}'")
        endif()
        
        if(RL_OBJECTS)
            set("${RL_OBJECTS}" ${OBJECTS} PARENT_SCOPE)
        endif(RL_OBJECTS)
        if(RL_LIBRARIES)
            set("${RL_LIBRARIES}" ${LIBRARIES} PARENT_SCOPE)
        endif(RL_LIBRARIES)
        if(RL_DEFINITIONS)
            set("${RL_DEFINITIONS}" ${DEFINITIONS} PARENT_SCOPE)
        endif(RL_DEFINITIONS)
    endforeach(LINE)
    
endfunction(read_linkfile)
