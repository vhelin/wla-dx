CC = $(WLAVALGRIND) wla-z80
CFLAGS = -o

ERROR_SFILES = rept_trailing.s

all: check

check:
	@set -e; \
	command -v wla-z80 >/dev/null 2>&1; \
	for source in $(ERROR_SFILES); do \
		if $(CC) $(CFLAGS) failed.o $$source > $$source.log 2>&1; then \
			echo "$$source unexpectedly assembled"; \
			exit 1; \
		fi; \
	done; \
	grep -F "got an unexpected token" rept_trailing.s.log >/dev/null

clean:
	rm -f *.o *.log core *~
