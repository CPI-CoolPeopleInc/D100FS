/*
MIT License

Copyright (c) 2024 CPI-CoolPeopleInc

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <stdint.h>
#include <stdio.h>
#include "disk.h"

#define D100_FAT_SECTORS 1

// relative sector maximum
#define RELSEC_MAX 0xfff

// entries before this are reserved by the file system
#define RELSEC_MIN 2

// refers to how much to shift left by when dealing with even relative sectors
#define RELSEC_HEADROOM 4

// and the value from the fat with this to clear an odd relative sector
#define RELSEC_CLEAR_ODD 0x000f
#define RELSEC_CLEAR_EVEN 0xf000

#define D100_FAT_RESERVED_BYTE1_INDEX 0
#define D100_FAT_RESERVED_BYTE2_INDEX 1

// these weren't documented anywhere but DOS 1.00 does use them
#define D100_FAT_RESERVED_BYTE1_VALUE 0xffe
#define D100_FAT_RESERVED_BYTE2_VALUE 0xfff

#define D100_FAT_LBA 1
#define D100_FAT_SECTORS 1
#define D100_FAT_COUNT 2

// number of sectors before relsec 0
#define RELSEC_OFFSET 5

// multiplying by 2 thirds is the same as diving by 1.5, which is the number
// of bytes per entry
#define RELSEC_LIMIT (BYTES_PER_SECTOR * D100_FAT_SECTORS * 2 / 3)

// this relative sector number marks the end of a chain, rather than mapping
// to a sector on the physical disk
#define RELSEC_END 0xfff

typedef uint16_t fat_entry_t;

/// @brief sets the value at an index of a file allocation table
/// @param fat file allocation table to modify
/// @param index entry to modify
/// @param value value to set. Only the low 12 bits are used
extern void d100_set_fat_entry(void *fat, size_t index, fat_entry_t value);

/// @brief sets the reserved entries at the beginning of a fat
/// @param fat file allocation table to update
extern void d100_set_fat_reserved_entries(void *fat);

/// @brief writes a file allocation table to the disk. Will also copy itself
///        into the backup location(s) on the disk as defined in this header
/// @param disk disk image to write to
/// @param fat file allocation table to write
/// @return 0 for success, -1 otherwise
extern int d100_write_fat(FILE *disk, void *fat);

/// @brief reads the first file allocation table from a disk image
/// @param disk image to read from
/// @return pointer to heap-allocated fat on success, NULL otherwise
extern void *d100_read_fat(FILE *disk);

/// @brief returns the first free sector number in the fat
/// @param fat file allocation table to look for entries in
/// @return the relative sector number on success, 0 otherwise
extern fat_entry_t d100_get_free_sector(void *fat);

/// @brief gets the value at an index of a file allocation table
/// @param fat file allocation table to look into
/// @param index entry to retrieve
/// @return the value at the index
extern fat_entry_t d100_get_fat_entry(void *fat, size_t index);

