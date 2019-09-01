
add_executable(byte_tester main.c)
set_property(TARGET byte_tester
    PROPERTY RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
# Don't install() because it's rather an internal tool

