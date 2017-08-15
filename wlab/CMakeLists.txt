
set(WLAB_SRCS
    main.c
    )

add_executable(wlab ${WLAB_SRCS})

if(USE_GIT_VERSION AND GIT_FOUND)
    set_property(SOURCE main.c APPEND
        PROPERTY COMPILE_DEFINITIONS "HAVE_GIT_VERSION_H")
endif(USE_GIT_VERSION AND GIT_FOUND)

install(TARGETS wlab DESTINATION bin)

