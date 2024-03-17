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

#include "chkdsk.h"
#include "format.h"
#include "filesys.h"
#include <stdio.h>
#include <stdint.h>

/// @brief creates a new file on a disk image with data from a file on
///        the host system
/// @param fs file system to write to
/// @param source_file file on the host system to get data from
/// @param name 8.3 file name for the new file on the disk image
/// @param attributes file attributes. See dir.h for available attributes
/// return 0 for success, -1 otherwise
extern int d100_create_file(dos100fs_t *fs, FILE *source_file, char *name,
                            uint8_t attributes);

/// @brief creates entry data for a new file and stores it in the directory
///        leaves the relative sector information blank
/// @param directory directory to modify
/// @param source_file file from the host system to base the meta data off
/// @param name 8.3 name for the new file
/// @param attributes file attributes
/// @return pointer to the file entry in the directory
extern directory_entry_t *d100_create_entry(dos100fs_t *fs, FILE *source_file,
                                            char *name, uint8_t attributes);

/// @brief gets the number of bytes free on the file system of a disk image
/// @param fs file system to get the free bytes of
/// @return number of bytes free on success, -1 otherwise
extern long d100_get_free_bytes(dos100fs_t *fs);

/// @brief allocates sectors in the file allocation table of a file system
///        for a file entry
/// @param fs file system to modify
/// @param entry file entry to allocate sectors for
///        this will be modified to set the first relative sector field
/// @return 0 for success; -1 otherwise
extern int d100_allocate_sectors(dos100fs_t *fs, directory_entry_t *entry);

/// @brief copies the data from a host file to a disk image
///        requires the sector list to have already been allocated
///        and the values in the entry data to have been set
/// @param fs file system to copy into
/// @param dest entry data for the destination file
/// @param src file handler for the source file
/// @return 0 for success, -1 otherwise
extern int d100_copy_data(dos100fs_t *fs, directory_entry_t *dest, FILE *src);

