CC = wla-65c02
CFLAGS = -o
LD = wlalink
LDFLAGS = -v -s
HANDY_DIR ?= C:/Users/ville/Downloads/Handy-0.95
HANDY_EXE ?= handy.exe
HANDY_FLAGS ?=

SFILES = main.s
OFILES = main.o
TARGET = hello.lnx
RUN_ROM = $(shell if command -v cygpath >/dev/null 2>&1; then cygpath -w "$(abspath $(TARGET))"; else printf '%s' "$(abspath $(TARGET))"; fi)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OFILES) linkfile makefile
	$(LD) $(LDFLAGS) linkfile $(TARGET)

main.o: main.s makefile
	$(CC) $(CFLAGS) main.o main.s

run: $(TARGET)
	cd "$(HANDY_DIR)" && ./$(HANDY_EXE) "$(RUN_ROM)" $(HANDY_FLAGS)

clean:
	rm -f $(OFILES) core *~ $(TARGET) *.sym