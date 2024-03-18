BITSTREAM ?= ./bitstream.bin
PARAM ?=./param.json
CGRAEMU_EXEC := $(BINARY) $(BITSTREAM) $(PARAM)
run-env: $(BINARY)
run: run-env
	$(CGRAEMU_EXEC)
gdb: run-env
	gdb -s $(BINARY) --args $(CGRAEMU_EXEC)

clean-tools = $(dir $(shell find ./tools -maxdepth 2 -mindepth 2 -name "Makefile"))
$(clean-tools):
	-@$(MAKE) -s -C $@ clean
clean-tools: $(clean-tools)
clean-all: clean distclean clean-tools
.PHONY: run run-env clean-all clean-tools $(clean-tools)
