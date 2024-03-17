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

#include "fat.h"
#include "disk.h"
#include <stdlib.h>

// there are 1.5 bytes per entry
#define D100_FAT_BYTE_INDEX(x) (x * 3 / 2)

/// @brief sets the value at an index of a file allocation table
/// @param fat file allocation table to modify
/// @param index entry to modify
/// @param value value to set. Only the low 12 bits are used
void d100_set_fat_entry(void *fat, size_t index, fat_entry_t value) {
    
    size_t byte_index = D100_FAT_BYTE_INDEX(index);
    fat_entry_t array_value;

    // voodoo black magic to compensate for lack of uint12_t type 
    fat_entry_t current_value = *(fat_entry_t *)(fat + byte_index);
    // because values are or-ed later, the fat entries need to be cleared first
    if(index & 1) {
        array_value = value << RELSEC_HEADROOM;
        *(fat_entry_t *)(fat + byte_index) &= RELSEC_CLEAR_ODD;
    } else {
        array_value = value & RELSEC_MAX;
        *(fat_entry_t *)(fat + byte_index) &= RELSEC_CLEAR_EVEN;
    }

    *(fat_entry_t *)(fat + byte_index) |= array_value;

    return;

}

/// @brief sets the reserved entries at the beginning of a fat
/// @param fat file allocation table to update
void d100_set_fat_reserved_entries(void *fat) {

    d100_set_fat_entry(fat, D100_FAT_RESERVED_BYTE1_INDEX,
                       D100_FAT_RESERVED_BYTE1_VALUE);
    d100_set_fat_entry(fat, D100_FAT_RESERVED_BYTE2_INDEX,
                       D100_FAT_RESERVED_BYTE2_VALUE);

    return;

}

/// @brief writes a file allocation to the disk. Will also copy itself
///        into the backup location(s) on the disk as defined in fat.h
/// @param disk disk image to write to
/// @param fat file allocation table to write
/// @return 0 for success, -1 otherwise
int d100_write_fat(FILE *disk, void *fat) {

    size_t current_lba = D100_FAT_LBA;

    for(size_t current_fat = 0; current_fat < D100_FAT_COUNT; current_fat++) {

        if(write_sectors(disk, current_lba, D100_FAT_SECTORS, fat) != 0)
            return -1;
        
        current_lba += D100_FAT_SECTORS;

    }

    return 0;

}

/// @brief reads the first file allocation table from a disk image
/// @param disk image to read from
/// @return pointer to heap-allocated fat on success, NULL otherwise
void *d100_read_fat(FILE *disk) {

    void *fat = malloc(D100_FAT_SECTORS * BYTES_PER_SECTOR);
    if(!fat) return NULL;

    if(read_sectors(disk, D100_FAT_LBA, D100_FAT_SECTORS, fat) != 0) {
        free(fat);
        return NULL;
    }

    return fat;

}

/// @brief returns the first free sector number in the fat
/// @param fat file allocation table to look for entries in
/// @return the relative sector number on success, 0 otherwise
fat_entry_t d100_get_free_sector(void *fat) {

    for(size_t i = RELSEC_MIN; i < RELSEC_LIMIT; i++) {

        if(d100_get_fat_entry(fat, i) == 0)
            return i;

    }

    return 0;

}

/// @brief gets the value at an index of a file allocation table
/// @param fat file allocation table to look into
/// @param index entry to retrieve
/// @return the value at the index
fat_entry_t d100_get_fat_entry(void *fat, size_t index) {

    size_t byte_index = D100_FAT_BYTE_INDEX(index);
    fat_entry_t raw_value = *(fat_entry_t *)(fat +
                            D100_FAT_BYTE_INDEX(index));

    if(index & 1)
        raw_value >>= RELSEC_HEADROOM;
    raw_value &= RELSEC_MAX;

    return raw_value;

}

