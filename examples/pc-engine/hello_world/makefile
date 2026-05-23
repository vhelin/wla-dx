CC = wla-huc6280
CFLAGS = -o
LD = wlalink
LDFLAGS = -v -s
MESEN ?= C:/Users/ville/Desktop/Mesen.exe
MESEN_FLAGS ?=

SFILES = main.s
OFILES = main.o
TARGET = hello.pce
RUN_ROM = $(shell if command -v cygpath >/dev/null 2>&1; then cygpath -w "$(TARGET)"; else printf '%s' "$(TARGET)"; fi)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OFILES) linkfile makefile
	$(LD) $(LDFLAGS) linkfile $(TARGET)

main.o: main.s makefile
	$(CC) $(CFLAGS) main.o main.s

run: $(TARGET)
	"$(MESEN)" "$(RUN_ROM)" $(MESEN_FLAGS)

clean:
	rm -f $(OFILES) core *~ $(TARGET) *.sym