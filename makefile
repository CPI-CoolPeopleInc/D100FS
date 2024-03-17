# MIT License
#
# Copyright (c) 2024 Gabriel Jickells
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

SRC=src
BIN=bin
TMP=$(BIN)/tmp
OUTPUT_DIRS=$(BIN) \
			$(TMP) \
			$(TMP)/format \
			$(TMP)/libs \
			$(TMP)/write
PREFIX=/usr/local/bin
LIB_PREFIX=/usr/lib

LIBS_OBJS=$(patsubst $(SRC)/libs/%.c,$(TMP)/libs/%.o,$(shell find $(SRC)/libs -name "*.c" -type f))
FORMAT_OBJS=$(patsubst $(SRC)/format/%.c,$(TMP)/format/%.o,$(wildcard $(SRC)/format/*.c))
WRITE_OBJS=$(patsubst $(SRC)/write/%.c,$(TMP)/write/%.o,$(wildcard $(SRC)/write/*.c))

FORMAT_CFLAGS=-I$(SRC)/libs
FORMAT_LDFLAGS=-L$(BIN) -ld100
LIBS_CFLAGS=-fPIC
LIBS_LDFLAGS=-shared
WRITE_CFLAGS=-I$(SRC)/libs/
WRITE_LDFLAGS=-L$(BIN) -ld100

CC=gcc
LINK=gcc

.PHONY: all install format clean dirs libs uninstall

all: dirs format write clean

dirs: $(OUTPUT_DIRS)

install:
	install $(BIN)/d100mkfs $(PREFIX)
	install $(BIN)/d100copy $(PREFIX)
	install $(BIN)/libd100.so $(LIB_PREFIX)

format: $(BIN)/d100mkfs

write: $(BIN)/d100copy

clean:
	rm -r $(TMP)

libs: $(BIN)/libd100.so

uninstall:
	rm $(PREFIX)/d100mkfs
	rm $(PREFIX)/d100copy
	rm $(LIB_PREFIX)/libd100.so

$(OUTPUT_DIRS):
	mkdir -p $(OUTPUT_DIRS)

$(BIN)/d100mkfs: $(FORMAT_OBJS) libs
	$(LINK) $(FORMAT_LDFLAGS) $(FORMAT_OBJS) -o $(BIN)/d100mkfs

$(BIN)/d100copy: $(WRITE_OBJS) libs
	$(LINK) $(WRITE_LDFLAGS) $(WRITE_OBJS) -o $(BIN)/d100copy

$(BIN)/libd100.so: $(LIBS_OBJS)
	$(LINK) $(LIBS_LDFLAGS) $(LIBS_OBJS) -o $(BIN)/libd100.so

$(FORMAT_OBJS): $(TMP)/format/%.o: $(SRC)/format/%.c
	$(CC) $(FORMAT_CFLAGS) -c $^ -o $@

$(LIBS_OBJS): $(TMP)/libs/%.o: $(SRC)/libs/%.c
	$(CC) $(LIBS_CFLAGS) -c $^ -o $@

$(WRITE_OBJS): $(TMP)/write/%.o: $(SRC)/write/%.c
	$(CC) $(WRITE_CFLAGS) -c $^ -o $@
