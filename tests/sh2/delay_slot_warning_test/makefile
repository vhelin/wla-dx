CC = $(WLAVALGRIND) wla-sh2

all: check

check:
	@output="$$( $(CC) -i -o main.o main.s 2>&1 )"; \
	printf "%s\n" "$$output"; \
	echo "$$output" | grep 'SH-2 delay slot after "BRA" contains "BSR"' >/dev/null

clean:
	rm -f main.o core *~ *.sym *.lst
