# Subproject: wlalink
project(wlalink)

set(CFILES main.c memory.c parse.c files.c check.c analyze.c write.c compute.c discard.c listfile.c ../hashmap.c)
add_executable(wlalink ${CFILES})

