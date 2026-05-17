EXAMPLE_DIR = ../../../examples/sh2/hello_world_32x
ROM = $(EXAMPLE_DIR)/hello32x.bin

all:
	$(MAKE) -C $(EXAMPLE_DIR) clean all
	byte_tester testsfile

clean:
	$(MAKE) -C $(EXAMPLE_DIR) clean