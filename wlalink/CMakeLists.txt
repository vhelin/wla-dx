
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
    ../crc32.c
    ../printf.c
    )

add_executable(wlalink ${WLALINK_SRCS})

install(TARGETS wlalink DESTINATION bin)

