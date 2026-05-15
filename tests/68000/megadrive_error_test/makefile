CC = $(WLAVALGRIND) wla-68000
CFLAGS = -o

ERROR_SFILES = duplicate_mdvectors.s duplicate_mcdheader.s duplicate_mcdspheader.s malformed_mdvectors.s missing_mcd_ipstart.s missing_mcdsp_spentry.s mixed_mcd_headers.s org0_conflict.s
LIBRARY_ERROR_SFILES = library_mdvectors.s library_mcdheader.s library_mcdspheader.s

all: check

check:
	@set -e; \
	command -v wla-68000 >/dev/null 2>&1; \
	for source in $(ERROR_SFILES); do \
		if $(CC) $(CFLAGS) failed.o $$source > $$source.log 2>&1; then \
			echo "$$source unexpectedly assembled"; \
			exit 1; \
		fi; \
	done; \
	for source in $(LIBRARY_ERROR_SFILES); do \
		if $(CC) -l failed.lib $$source > $$source.log 2>&1; then \
			echo "$$source unexpectedly assembled as a library"; \
			exit 1; \
		fi; \
	done; \
	grep -F ".MDVECTORS can be defined only once." duplicate_mdvectors.s.log >/dev/null; \
	grep -F ".MCDHEADER can be defined only once." duplicate_mcdheader.s.log >/dev/null; \
	grep -F ".MCDSPHEADER can be defined only once." duplicate_mcdspheader.s.log >/dev/null; \
	grep -F "Error in .MDVECTORS data structure." malformed_mdvectors.s.log >/dev/null; \
	grep -F ".MCDHEADER requires IPSTART to be defined." missing_mcd_ipstart.s.log >/dev/null; \
	grep -F ".MCDSPHEADER requires SPENTRY to be defined." missing_mcdsp_spentry.s.log >/dev/null; \
	grep -F ".MCDHEADER and .MCDSPHEADER cannot be used in the same source file." mixed_mcd_headers.s.log >/dev/null; \
	grep -F "don't also place code or data with .ORGA 0." org0_conflict.s.log >/dev/null; \
	grep -F "Libraries don't take .MDVECTORS." library_mdvectors.s.log >/dev/null; \
	grep -F "Libraries don't take .MCDHEADER." library_mcdheader.s.log >/dev/null; \
	grep -F "Libraries don't take .MCDSPHEADER." library_mcdspheader.s.log >/dev/null

clean:
	rm -f *.o *.lib *.log core *~