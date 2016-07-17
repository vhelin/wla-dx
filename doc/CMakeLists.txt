# Documentation CMakeLists file

project(DOCS)

option(GEN_DOC "Generate Sphinx documentation" ON)
option(GEN_DOC_MAN "Generate Manpages" ON)
option(GEN_DOC_SINGLEHTML "Generate single HTML documentation" ON)
option(GEN_DOC_TEXT "Generate text documentation" OFF)

set(DOC_OUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/doc")
set(DOC_SRC "${CMAKE_CURRENT_SOURCE_DIR}/source")

# Find sphix-build
# eb2.co/blog/2012/03/sphinx-and-cmake-beautiful-documentation-for-c-projects/
find_program(SPHINX_EXECUTABLE
    NAMES sphinx-build
    HINTS $ENV{SPHINX_DIR}
    PATH_SUFFIXES bin
    DOC "Sphinx documentation generator"
)

if (SPHINX_EXECUTABLE STREQUAL "SPHINX_EXECUTABLE-NOTFOUND")
    message(WARNING "Sphinx not found")
    return()
endif (SPHINX_EXECUTABLE STREQUAL "SPHINX_EXECUTABLE-NOTFOUND")

if (!GEN_DOC)
    return()
endif (!GEN_DOC)

if (GEN_DOC_MAN)
    # TODO: Manpages generation.
    # What i thought:
    #   wla-dx.?: Main documentation
    #   wlalink.1: wlalink documentation
    #   wla-CPU.1: wla-CPU combined documentation (all on one)
    #       (see http://www.schweikhardt.net/man_page_howto.html#q4)
endif (GEN_DOC_MAN)

if (GEN_DOC_SINGLEHTML)
    add_custom_command(
        OUTPUT "${DOC_OUT}/wla-dx-doc.html"
        COMMAND ${SPHINX_EXECUTABLE} -b singlehtml "${DOC_SRC}" singlehtml
        COMMAND ${CMAKE_COMMAND} -E copy singlehtml/index.html
            "${DOC_OUT}/wla-dx-doc.html"
        COMMAND ${CMAKE_COMMAND} -E copy_directory singlehtml/_static/
            "${DOC_OUT}/_static/"
        COMMENT "Generate single HTML page documentation"
    )
    add_custom_target(doc_singlehtml ALL DEPENDS "${DOC_OUT}/wla-dx-doc.html")
endif (GEN_DOC_SINGLEHTML)

