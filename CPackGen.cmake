
include("${SRCDIR}/generate-from-version.cmake")

configure_file("CPackConfig.cmake.in" "CPackConfig.cmake" @ONLY)
configure_file("CPackSourceConfig.cmake.in" "CPackSourceConfig.cmake" @ONLY)
