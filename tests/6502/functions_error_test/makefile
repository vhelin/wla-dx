CC = $(WLAVALGRIND) wla-6502
CFLAGS = -o

ERROR_SFILES = mid_overflow.s

all: check

check:
	@set -e; \
	command -v wla-6502 >/dev/null 2>&1; \
	for source in $(ERROR_SFILES); do \
		if $(CC) $(CFLAGS) failed.o $$source > $$source.log 2>&1; then \
			echo "$$source unexpectedly assembled"; \
			exit 1; \
		fi; \
	done; \
	grep -F "mid() length 2147483647 runs outside string" mid_overflow.s.log >/dev/null

clean:
	rm -f *.o *.log core *~
