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

#ifndef _DIR_H_
#define _DIR_H_

#include <stdio.h>
#include <stdint.h>
#include "disk.h"

#define D100_NAME_LENGTH 8
#define D100_EXT_LENGTH 3
#define D100_NAME_EXT_LENGTH (D100_NAME_LENGTH + D100_EXT_LENGTH)
#define D100_RESERVED_BYTES 12

#define D100_DIRECTORY_LBA 3
#define D100_DIRECTORY_SECTORS 4

typedef struct {

    char name[D100_NAME_LENGTH];
    char extension[D100_EXT_LENGTH];
    uint8_t attributes;
    uint8_t _reserved[D100_RESERVED_BYTES];
    uint16_t update_date;
    uint16_t first_relative_sector;
    uint32_t size_bytes;

} __attribute__((packed)) directory_entry_t;

#define D100_MAX_DIRECTORY_ENTRIES (D100_DIRECTORY_SECTORS * BYTES_PER_SECTOR / sizeof(directory_entry_t))

// index into the name of the entry that is also used to mark the file as
// deleted
#define D100_DIRECTORY_DELETED_INDEX 0

#define D100_DIRECTORY_DELETED ((char)(0xe5))

#define D100_ATTR_SYSTEM 0b00000100
#define D100_ATTR_HIDDEN 0b00000010
#define D100_DEFAULT_ATTRS 0

/// @brief writes a directory to a disk image
/// @param disk image to write to
/// @param directory directory to write. Must be D100_DIRECTORY_SECTORS sectors in size
/// @return 0 for success, -1 otherwise
extern int d100_write_directory(FILE *disk, directory_entry_t *directory);

/// @brief empties a directory
/// @param directory directory to clear
extern void d100_clear_directory(directory_entry_t *directory);

/// @brief reads a directory from a disk image
/// @param disk image to read from
/// @return pointer to heap allocated directory on success, NULL otherwise
extern directory_entry_t *d100_read_directory(FILE *disk);

/// @brief finds a free entry in the directory of a file system
/// @param directory directory to look through
/// @return pointer to the free entry in the directory on success, else NULL
extern directory_entry_t *d100_get_free_entry(directory_entry_t *directory);

/// @brief finds an entry by its name in a directory
/// @param directory directory to look through
/// @param name 8.3 name to look for
/// @return pointer to the entry in the directory on success, NULL otherwise
extern directory_entry_t *d100_find_entry(directory_entry_t *directory,
                                          char *name);

/// @brief converts an 8.3 file name into the format used on the disk
/// @param name 8.3 name to convert
/// @param dest destination to store the converted name. Must be
///        D100_NAME_EXT_LENGTH characters in size
extern void d100_name_to_dir_name(char *name, char *dest);

#endif

