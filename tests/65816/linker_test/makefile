
CC = wla-65816
CFLAGS = -v -I include_dir_1 -I include_dir_2 -D NAME="kullervo" -o
LD = wlalink
LDFLAGS = -v -S

SFILES = main.s can_you_tell_this_name_is_intended_to_test_the_capability_of_wla_to_handle_long_filenames_and_stufffffffff.s
IFILES = defines.i
OFILES = main.o can_you_tell_this_name_is_intended_to_test_the_capability_of_wla_to_handle_long_filenames_and_stufffffffff.o

all: $(OFILES) test.lib makefile
	$(LD) $(LDFLAGS) linkfile result.smc

main.o: main.s defines.i
	$(CC) $(CFLAGS) main.o main.s

can_you_tell_this_name_is_intended_to_test_the_capability_of_wla_to_handle_long_filenames_and_stufffffffff.o: can_you_tell_this_name_is_intended_to_test_the_capability_of_wla_to_handle_long_filenames_and_stufffffffff.s defines.i
	$(CC) $(CFLAGS) can_you_tell_this_name_is_intended_to_test_the_capability_of_wla_to_handle_long_filenames_and_stufffffffff.o can_you_tell_this_name_is_intended_to_test_the_capability_of_wla_to_handle_long_filenames_and_stufffffffff.s

test.lib: test.s
	$(CC) -l test.lib test.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.smc *.lib *.sym
