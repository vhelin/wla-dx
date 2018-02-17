
execute_process(COMMAND "${GIT}" describe --dirty --always
    WORKING_DIRECTORY "${GITROOT}/.."
    RESULT_VARIABLE RESULT
    OUTPUT_VARIABLE GIT_DESCRIBE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

if(NOT RESULT EQUAL 0)
    message(FATAL_ERROR "Couldn't git describe: ${RESULT}")
endif()

execute_process(
    COMMAND "${GIT}" show -s "--format=%cd"
        "--date=format:%Y-%m-%d" ${GIT_FULL_HASH}
    WORKING_DIRECTORY "${GITROOT}"
    RESULT_VARIABLE RESULT
    OUTPUT_VARIABLE VERSION_DATE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

if(NOT RESULT EQUAL 0)
    message(FATAL_ERROR "Couldn't get commiter date in DD.MM.YYYY: ${RESULT}")
endif()

set(GIT_BRANCH "$ENV{GIT_BRANCH}")
if(GIT_BRANCH)
    set(GIT_BRANCH "unknown")
endif()
# GitLab CI, CI_COMMIT_REF_NAME is a branch when CI_COMMIT_TAG is unset
if("$ENV{CI_COMMIT_REF_NAME}" AND NOT "$ENV{CI_COMMIT_TAG}")
    set(GIT_BRANCH "$ENV{CI_COMMIT_REF_NAME}")
else()
    execute_process(
        COMMAND "${GIT}" rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY "${GITROOT}"
        RESULT_VARIABLE RESULT
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )

    if(NOT RESULT EQUAL 0)
        message(FATAL_ERROR "Couldn't branch for git: ${RESULT}")
    endif()
endif()

set(VERSION_FULL_STRING "g-${GIT_BRANCH}-${GIT_DESCRIBE}")
string(REGEX MATCH "[0-9]+(\\.[0-9]+)+" VERSION "${VERSION_FULL_STRING}")
if(VERSION STREQUAL "")
    file(READ "${VERSIONFILE}" VERSION_FILE_TEXT)
    string(REPLACE "\n" "" VERSION_FILE_TEXT "${VERSION_FILE_TEXT}")
    set(VERSION_FULL_STRING "v${VERSION_FILE_TEXT}-${VERSION_FULL_STRING}")
endif()

file(WRITE "BUILD_VERSION" "${VERSION_FULL_STRING}\n${VERSION_DATE}\ngit")

