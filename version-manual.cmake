
file(READ "${VERSIONFILE}" VERSION_FILE_TEXT)
string(REPLACE "\n" "" VERSION_FILE_TEXT "${VERSION_FILE_TEXT}")

set(VERSION_FULL_STRING "v${VERSION_FILE_TEXT}")
string(REGEX MATCH "[0-9]+(\\.[0-9]+)+" VERSION "${VERSION_FILE_TEXT}")
string(REGEX MATCH "[0-9]+\\.[0-9]+" VERSION_AMIGA "${VERSION_FILE_TEXT}")
file(TIMESTAMP "${VERSIONFILE}" VERSION_DATE "%Y-%m-%d" UTC)

file(WRITE "BUILD_VERSION" "${VERSION_FULL_STRING}\n${VERSION_DATE}\nmanual")
