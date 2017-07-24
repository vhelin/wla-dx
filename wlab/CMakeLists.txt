
set(WLAB_SRCS
    main.c
    )

add_executable(wlab ${WLAB_SRCS})

install(TARGETS wlab DESTINATION bin)

