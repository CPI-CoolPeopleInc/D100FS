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

LIBS_OBJS=$(patsubst $(SRC)/libs/%.c,$(TMP)/libs/%.o,$(shell find $(SRC)/libs -name "*.c" -type f))
FORMAT_OBJS=$(patsubst $(SRC)/format/%.c,$(TMP)/format/%.o,$(wildcard $(SRC)/format/*.c))
WRITE_OBJS=$(patsubst $(SRC)/write/%.c,$(TMP)/write/%.o,$(wildcard $(SRC)/write/*.c))

FORMAT_CFLAGS=-Isrc/libs/
FORMAT_LDFLAGS=
LIBS_CFLAGS=
WRITE_CFLAGS=-Isrc/libs/
WRITE_LDFLAGS=

CC=gcc
LINK=gcc

.PHONY: all install format clean dirs

all: dirs format write clean

dirs: $(OUTPUT_DIRS)

install:
	cp $(BIN)/d100mkfs $(PREFIX)/d100mkfs
	cp $(BIN)/d100copy $(PREFIX)/d100copy

format: $(BIN)/d100mkfs

write: $(BIN)/d100copy

clean:
	rm -r $(TMP)

$(OUTPUT_DIRS):
	mkdir -p $(OUTPUT_DIRS)

$(BIN)/d100mkfs: $(FORMAT_OBJS) $(LIBS_OBJS)
	$(LINK) $(FORMAT_LDFLAGS) $(FORMAT_OBJS) $(LIBS_OBJS) -o $(BIN)/d100mkfs

$(BIN)/d100copy: $(WRITE_OBJS) $(LIBS_OBJS)
	$(LINK) $(WRITE_LDFLAGS) $(WRITE_OBJS) $(LIBS_OBJS) -o $(BIN)/d100copy

$(FORMAT_OBJS): $(TMP)/format/%.o: $(SRC)/format/%.c
	$(CC) $(FORMAT_CFLAGS) -c $^ -o $@

$(LIBS_OBJS): $(TMP)/libs/%.o: $(SRC)/libs/%.c
	$(CC) $(LIBS_CFLAGS) -c $^ -o $@

$(WRITE_OBJS): $(TMP)/write/%.o: $(SRC)/write/%.c
	$(CC) $(WRITE_CFLAGS) -c $^ -o $@
