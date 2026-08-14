CC = wla-cp1610
CFLAGS = -o
LD = wlalink
LDFLAGS = -v -s
JZINTV ?= C:/Users/ville/Downloads/jzintv-20200712-win32-sdl2/bin/jzintv.exe
JZINTV_FLAGS ?=

SFILES = main.s
OFILES = main.o
TARGET = hello.bin
RUN_ROM = $(shell if command -v cygpath >/dev/null 2>&1; then cygpath -w "$(abspath $(TARGET))"; else printf '%s' "$(abspath $(TARGET))"; fi)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OFILES) linkfile makefile hello.cfg
	$(LD) $(LDFLAGS) linkfile $(TARGET)

main.o: main.s makefile
	$(CC) $(CFLAGS) main.o main.s

run: $(TARGET)
	"$(JZINTV)" $(JZINTV_FLAGS) "$(RUN_ROM)"

clean:
	rm -f $(OFILES) core *~ $(TARGET) *.sym
