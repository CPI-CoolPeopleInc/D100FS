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

#include "filesys.h"
#include <stdlib.h>
#include <stddef.h>
#include "dir.h"
#include "fat.h"

/// @brief returns resources used by the dos100fs_t structure to the system
///        does not close the disk image descriptor or free the structure
/// @param fs file system to close
void d100_close(dos100fs_t *fs) {

    free(fs->directory);
    free(fs->fat);

    return;

}

/// @brief opens the dos 1.00 file system on a disk image
/// @param disk image to get the file system from
/// @return pointer to the new file system structure on success, NULL otherwise
dos100fs_t *d100_open(FILE *disk) {

    dos100fs_t *newfs = malloc(sizeof(dos100fs_t));
    if(!newfs) return NULL;

    newfs->disk = disk;

    newfs->directory = d100_read_directory(newfs->disk);
    if(!newfs->directory) {
    	d100_close(newfs);
        free(newfs);
        return NULL;
    }

    newfs->fat = d100_read_fat(newfs->disk);
    if(!newfs->fat) {
        d100_close(newfs);
        free(newfs);
        return NULL;
    }

    // todo: check the file system for correctness

    return newfs;

}

