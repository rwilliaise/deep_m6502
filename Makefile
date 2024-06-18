CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lm
EXECUTABLES=hello_exec printer00 printer01 printer02 printer03
BINARIES=hello_bin.bin stack00.bin stack01.bin stack02.bin divisible.bin big.bin \
	printer00.bin printer01.bin printer02.bin printer03.bin
VASM=vasm/vasm6502_oldstyle

TESTS=hello_bin hello_exec stack00 stack01 stack02 divisible big printer00 \
	printer01 printer02 printer03

.PHONY: download_vasm test tests binaries clean clean_build

test: test_runner binaries tests
	./test_runner $(TESTS)

clean: clean_build
	rm -rf vasm vasm.tar.gz

clean_build:
	rm -f test_runner test_runner.o
	rm -f $(EXECUTABLES) $(foreach exe,$(EXECUTABLES),$(exe).o)
	rm -f $(BINARIES)

test_runner: test_runner.o

tests: $(EXECUTABLES)
binaries: $(VASM) $(foreach binary,$(BINARIES),$(binary))

$(OBJECTS): %: %.o
$(BINARIES): %.bin: %.inc

%.bin: %.inc
	./$(VASM) -quiet -dotdir -Fbin -o $@ $<

$(VASM): vasm/Makefile
	$(MAKE) -C vasm CPU=6502 SYNTAX=oldstyle

vasm/Makefile: vasm.tar.gz
	tar -xvf vasm.tar.gz

vasm.tar.gz:
	curl -LO http://sun.hasenbraten.de/vasm/release/vasm.tar.gz


define newline

endef
