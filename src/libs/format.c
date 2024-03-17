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

#include "format.h"
#include "disk.h"
#include <stdint.h>
#include "dir.h"
#include "bootrec.h"
#include <stdlib.h>
#include "fat.h"
#include "filesys.h"
#include <stddef.h>

/// @brief formats a disk image to be usable by PC/MS-DOS 1.00
/// @param disk disk image to format
/// @return pointer to new file system on success, NULL otherwise
dos100fs_t *d100_format(FILE *disk) {

    dos100fs_t *newfs = malloc(sizeof(dos100fs_t));
    if(!newfs) return NULL;

    newfs->disk = disk;
    if(d100_write_boot_record(disk) != 0) {
        d100_close(newfs);
        free(newfs);
        return NULL;
    }

    newfs->directory = calloc(D100_DIRECTORY_SECTORS, BYTES_PER_SECTOR);
    if(!newfs->directory) {
        d100_close(newfs);
        free(newfs);
        return NULL;
    }

    d100_clear_directory(newfs->directory);
    if(d100_write_directory(disk, newfs->directory) != 0) {
        d100_close(newfs);
        free(newfs);
        return NULL;
    }

    newfs->fat = calloc(D100_FAT_SECTORS, BYTES_PER_SECTOR);
    if(!newfs->fat) {
        d100_close(newfs);
        free(newfs);
        return NULL;
    }

    d100_set_fat_reserved_entries(newfs->fat);
    if(d100_write_fat(disk, newfs->fat) != 0) {
        d100_close(newfs);
        free(newfs);
        return NULL;
    }

    return newfs;

}

