
set(WLALINK_SRCS
    main.c
    memory.c
    parse.c
    files.c
    check.c
    analyze.c
    write.c
    compute.c
    discard.c
    listfile.c
    ../hashmap.c
    )

add_executable(wlalink ${WLALINK_SRCS})

if(USE_GIT_VERSION AND GIT_FOUND)
    set_property(SOURCE main.c APPEND
        PROPERTY COMPILE_DEFINITIONS "HAVE_GIT_VERSION_H")
endif(USE_GIT_VERSION AND GIT_FOUND)

install(TARGETS wlalink DESTINATION bin)

