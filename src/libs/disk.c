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

#include "disk.h"

/// @brief writes sectors to a location on a disk image
/// @param disk disk image to write to
/// @param lba number of sectors into the disk image to start writing from
/// @param count number of sectors to write from data
/// @param data data to be written to the disk image
/// @return 0 for success, -1 otherwise
int write_sectors(FILE* disk, size_t lba, size_t count, void* data) {

    if(fseek(disk, lba * BYTES_PER_SECTOR, SEEK_SET) < 0)
        return -1;

    if(!fwrite(data, BYTES_PER_SECTOR, count, disk))
        return -1;
    
    return 0;

}

/// @brief reads sectors from a location on a disk image
/// @param disk image to read from
/// @param lba number of sectors into the image to start reading from
/// @param count number of sectors to read from the image
/// @param data_out location in memory to store the sectors
/// @return 0 for success, -1 otherwise
int read_sectors(FILE *disk, size_t lba, size_t count, void *data_out) {

    if(fseek(disk, lba * BYTES_PER_SECTOR, SEEK_SET) < 0)
        return -1;

    if(!fread(data_out, BYTES_PER_SECTOR, count, disk))
        return -1;

    return 0;

}

