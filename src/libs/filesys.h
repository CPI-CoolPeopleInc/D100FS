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

#ifndef _FILESYS_H_
#define _FILESYS_H_

#include "dir.h"
#include "fat.h"
#include <stdio.h>

typedef struct dos100fs {
    FILE *disk;
    directory_entry_t *directory;
    void *fat;
} dos100fs_t;

/// @brief returns resources used by the dos100fs_t structure to the system
///        does not close the disk image descriptor or free the structure
/// @param fs file system to close
extern void d100_close(dos100fs_t *fs);

/// @brief opens the dos 1.00 file system on a disk image
/// @param disk image to get the file system from
/// @return pointer to the new file system structure on success, NULL otherwise
extern dos100fs_t *d100_open(FILE *disk);

#endif

