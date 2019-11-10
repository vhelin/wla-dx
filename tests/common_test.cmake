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
    else()
        set(${OUTPUT_VAR} "${PATH}" PARENT_SCOPE)
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

# Like file_equal, but strips ;comments because they can contain file name
# which because of absoulte paths differ
function(sym_file_equal A B OUT_VAR)
    if(NOT EXISTS "${A}")
        abort("Couldn't find file A '${A}'!")
    elseif(NOT EXISTS "${B}")
        abort("Found file A, but couldn't find file B '${B}'!")
    else()
        # Read file but strip ;comments
        file(STRINGS "${A}" A_FILE REGEX "^[^;].*")
        file(STRINGS "${B}" B_FILE REGEX "^[^;].*")
        if(A_FILE STREQUAL B_FILE)
            set(${OUT_VAR} ON PARENT_SCOPE)
        else(NOT A_FILE STREQUAL B_FILE)
            set(${OUT_VAR} OFF PARENT_SCOPE)
        endif()
    endif()
endfunction(sym_file_equal)

function(abort_if_file_unequal A B MESSAGE)
    file_equal("${A}" "${B}" FILE_EQUAL)
    if(NOT FILE_EQUAL)
        abort("${MESSAGE}")
    endif(NOT FILE_EQUAL)
endfunction(abort_if_file_unequal)

function(abort_if_text_file_unequal A B MESSAGE)
    file_equal("${A}" "${B}" FILE_EQUAL)
    if(NOT FILE_EQUAL)
        if(DIFF_EXE)
            execute_process_cmd(COMMAND ${DIFF_EXE} ${A} ${B})
        endif()
        abort("${MESSAGE}")
    endif(NOT FILE_EQUAL)
endfunction(abort_if_text_file_unequal)

function(abort_if_sym_file_unequal A B MESSAGE)
    sym_file_equal("${A}" "${B}" FILE_EQUAL)
    if(NOT FILE_EQUAL)
        if(DIFF_EXE)
            execute_process_cmd(COMMAND ${DIFF_EXE} ${A} ${B})
        endif()
        abort("${MESSAGE}")
    endif(NOT FILE_EQUAL)
endfunction(abort_if_sym_file_unequal)

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

function(library_object_new VAR_OUTPUT)
    set(options)
    set(oneValueArgs BANK SLOT BASE FILE)
    set(multiValueArgs)
    cmake_parse_arguments(TW "${options}" "${oneValueArgs}"
        "${multiValueArgs}" ${ARGN})
    
    set(OBJ "${TW_BANK}" "${TW_SLOT}" "${TW_BASE}" "${FILE}")
    string(REPLACE ";" ":" OBJ OBJ)
    set("${VAR_OUTPUT}" "${OBJ}" PARENT_SCOPE)
endfunction(library_object_new)

function(library_object_get OBJ)
    set(options)
    set(oneValueArgs BANK SLOT BASE FILE)
    set(multiValueArgs)
    cmake_parse_arguments(TW "${options}" "${oneValueArgs}"
        "${multiValueArgs}" ${ARGN})
    
    string(replace ":" ";" OBJ OBJ)
    
    if(TW_BANK)
        list(GET OBJ 0 "${TW_BANK}")
        set("${TW_BANK}" "${TW_BANK}" PARENT_SCOPE)
    endif()
    
    if(TW_SLOT)
        list(GET OBJ 1 "${TW_SLOT}")
        set("${TW_SLOT}" "${TW_SLOT}" PARENT_SCOPE)
    endif()
    
    if(TW_BASE)
        list(GET OBJ 2 "${TW_BASE}")
        set("${TW_BASE}" "${TW_BASE}" PARENT_SCOPE)
    endif()
    
    if(TW_FILE)
        list(GET OBJ 3 "${TW_FILE}")
        set("${TW_FILE}" "${TW_FILE}" PARENT_SCOPE)
    endif()
endfunction(library_object_get)


function(library_object_set OBJ OUT_VAR)
    set(options)
    set(oneValueArgs BANK SLOT BASE FILE)
    set(multiValueArgs)
    cmake_parse_arguments(TW "${options}" "${oneValueArgs}"
        "${multiValueArgs}" ${ARGN})
    
    string(replace ":" ";" OBJ OBJ)
    
    if(TW_BANK)
        list(REMOVE_AT OBJ 0)
        list(SET OBJ 0 "${TW_BANK}")
    endif()
    
    if(TW_SLOT)
        list(REMOVE_AT OBJ 0)
        list(SET OBJ 0 "${TW_SLOT}")
    endif()
    
    if(TW_BASE)
        list(REMOVE_AT OBJ 0)
        list(SET OBJ 0 "${TW_BASE}")
    endif()
    
    if(TW_FILE)
        list(REMOVE_AT OBJ 0)
        list(SET OBJ 0 "${TW_FILE}")
    endif()
    
    string(replace ";" ":" OBJ OBJ)
    set("${OUT_VAR}" "${OBJ}" PARENT_SCOPE)
endfunction(library_object_set)

# Assemble a file to an object/library file
# wla(
#     <OBJECT|LIBRARY> # Whenever to comile either an object or an library
#     ARCH arch        # Use wla-ARCH
#     SOURCE source    # Source file
#     OUTPUT outfile   # Output file
#     [DEFINES def=value [def=value]...] # Defines
#     [FLAGS flag [flag]...]  # Extra flags
#     [VERBOSE]               # Be verbose
#     )
function(wla)
    set(options OBJECT LIBRARY VERBOSE)
    set(oneValueArgs ARCH SOURCE OUTPUT)
    set(multiValueArgs DEFINES FLAGS)
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
    
    if(NOT TW_SOURCE)
        abort("Please specify SOURCE for wla!")
    endif()
    
    if(NOT TW_OUTPUT)
        abort("Please specify OUTPUT for wla!")
    endif()
    
    if(TW_VERBOSE)
        list(APPEND TW_FLAGS "-v")
    endif(TW_VERBOSE)
    
    foreach(DEFINE IN LISTS TW_DEFINES)
        list(APPEND TW_FLAGS "-D${DEFINE}")
    endforeach(DEFINE)
    
    if(TW_OBJECT)
        list(APPEND TW_FLAGS "-o")
    elseif(TW_LIBRARY)
        list(APPEND TW_FLAGS "-l")
    endif()
    
    get_full_path("${TW_SOURCE}" "${SOURCE_DIR}" SRC)
    get_full_path("${TW_OUTPUT}" "${BINARY_DIR}" OUT)
    execute_process_cmd(
        COMMAND "${WLA_${TW_ARCH}}" ${TW_FLAGS} "${OUT}" "${SRC}"
        WORKING_DIRECTORY "${SOURCE_DIR}"
        RESULT_VARIABLE RESULT
        )
    abort_result("${RESULT}" "assemble" "wla-${TW_ARCH}" "${SRC}")
    
    list(APPEND FILES_TO_CLEAN_UP ${TW_OUTPUT})
    set(FILES_TO_CLEAN_UP ${FILES_TO_CLEAN_UP} PARENT_SCOPE)
endfunction(wla)

# Assemble files to object files
# wlas(
#     <OBJECT|LIBRARY> # Whenever to comile either an object or an library
#     ARCH arch        # Use wla-ARCH
#     SOURCES source [source]... # Source files
#     [OUT_VAR var]    # Variable to put list of object files
#     [DEFINES def=value [def=value]...] # Defines
#     [FLAGS flag [flag]...]  # Extra flags
#     [VERBOSE]               # Be verbose
#     )
function(wlas)
    set(options OBJECT LIBRARY VERBOSE)
    set(oneValueArgs ARCH OUT_VAR)
    set(multiValueArgs SOURCES DEFINES FLAGS)
    cmake_parse_arguments(TW "${options}" "${oneValueArgs}"
        "${multiValueArgs}" ${ARGN})
    
    if(NOT (TW_OBJECT OR TW_LIBRARY))
        abort("Please put either OBJECT or LIBRARY for wlas!")
    endif(NOT (TW_OBJECT OR TW_LIBRARY))
    
    if(TW_OBJECT AND TW_LIBRARY)
        abort("You can't create an OBJECT and an LIBRARY at the same time!")
    endif(TW_OBJECT AND TW_LIBRARY)
    
    if(TW_OBJECT)
        set(OUTPUT_FORMAT OBJECT)
    else()
        set(OUTPUT_FORMAT LIBRARY)
    endif()
    
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
        convert_to_binary_path("${SRC}" OUT_SRC)
        string(REGEX REPLACE "\\.s$" ".o" OUT_SRC "${OUT_SRC}")
        get_full_path("${SRC}" "${SOURCE_DIR}" SRC)
        wla(${OUTPUT_FORMAT}
            ARCH ${TW_ARCH}
            SOURCE "${SRC}"
            OUTPUT "${OUT_SRC}"
            FLAGS ${TW_FLAGS}
            )
        list(APPEND OUT_SRCS "${OUT_SRC}")
    endforeach(SRC)
    
    if(TW_OUT_VAR)
        set(${TW_OUT_VAR} ${OUT_SRCS} PARENT_SCOPE)
    endif(TW_OUT_VAR)
    set(FILES_TO_CLEAN_UP ${FILES_TO_CLEAN_UP} PARENT_SCOPE)
endfunction(wlas)

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
    if(TL_LIBRARIES)
        foreach(LIB IN LISTS TL_LIBRARIES)
            library_object_get(LIB BANK BANK SLOT SLOT BASE BASE FILE FNAME)
            file(APPEND "${LINKFILE}" "bank ${BANK}")
            file(APPEND "${LINKFILE}" "slot ${SLOT}")
            if(BASE)
                file(APPEND "${LINKFILE}" "base ${BASE}")
            endif()
            file(APPEND "${LINKFILE}" "${FNAME}\n")
        endforeach(LIB)
    endif()
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
        WORKING_DIRECTORY "${BINARY_DIR}"
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
#     [LINKFILE linkfile] # Linkfile to use rather to generate it.
#          # If set, LINK_DEFINES, LIBRARIES and OBJECTS are not used
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
    set(oneValueArgs OUTPUT ARCH LINKFILE)
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
    
    wlas(
        OBJECT ARCH "${TW_ARCH}"
        SOURCES ${TW_SOURCES}
        OUT_VAR OBJECTS
        DEFINES ${TW_DEFINES}
        FLAGS ${TW_WLA_FLAGS}
        ${WLA_VERBOSE}
        )
    
    wlas(
        LIBRARY ARCH "${TW_ARCH}"
        SOURCES ${TW_LIBSOURCES}
        OUT_VAR LIBRARIES
        DEFINES ${TW_DEFINES}
        FLAGS ${TW_WLA_FLAGS}
        ${WLA_VERBOSE}
        )
    
    list(APPEND OBJECTS ${TW_OBJECTS})
    list(APPEND LIBRARIES ${TW_LIBRARIES})
    
    if(TW_LINKFILE)
        get_full_path("${TW_LINKFILE}" "${BINARY_DIR}" LINKFILE)
        message(STATUS "[LINKFILE] Using custom linkfile: ${LINKFILE}")
    else()
        get_full_path("${TW_OUTPUT}.link" "${BINARY_DIR}" LINKFILE)
        create_linkfile(
            OUTPUT "${LINKFILE}"
            OBJECTS ${OBJECTS}
            LIBRARIES ${LIBRARIES}
            DEFINES ${TW_LINK_DEFINES}
            )
    endif()
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
            string(REGEX REPLACE "bank +([^ ]+) *" "" LINE "${LINE}")
            set(BANK "${CMAKE_MATCH_1}")
            string(REGEX REPLACE "slot +([^ ]+) *" "" LINE "${LINE}")
            set(SLOT "${CMAKE_MATCH_1}")
            string(REGEX REPLACE "base +([^ ]+) *" "" LINE "${LINE}")
            set(BASE "${CMAKE_MATCH_1}")
            
            string(REGEX REPLACE " " ";" LINE "${LINE}")
            list(GET LINE -1 FNAME)
            list(REMOVE_AT LINE -1)
            
            library_object_new(LIBOBJ
                BANK "${BANK}"
                SLOT "${SLOT}"
                BASE "${BASE}"
                FILE "${FNAME}")
            list(APPEND LIBRARIES "${LIBOBJ}")
            
            if(LINE)
                message("[LINKILE PARSER] Leftover library: ${LINE}")
            endif()
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
