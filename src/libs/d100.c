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

#include "d100.h"
#include "dir.h"
#include <string.h>
#include <stdlib.h>
#include "date.h"
#include "disk.h"

/// @brief creates a new file on a disk image with data from a file on
///        the host system
/// @param fs file system to write to
/// @param source_file file on the host system to get data from
/// @param name 8.3 file name for the new file on the disk image
/// @param attributes file attributes. See dir.h for available attributes
/// @return 0 for success, -1 otherwise
extern int d100_create_file(dos100fs_t *fs, FILE *source_file, char *name,
                            uint8_t attributes) {
    
    directory_entry_t *new_file = d100_create_entry(fs, source_file, name,
                                                    attributes);
    if(!new_file)
        return -1;

    if(d100_allocate_sectors(fs, new_file) < 0)
        return -1;

    
    if(d100_copy_data(fs, new_file, source_file) < 0)
        return -1;

    // the directory is written last so faulty files are inaccessible
    if(d100_write_fat(fs->disk, fs->fat) < 0)
        return -1;
    if(d100_write_directory(fs->disk, fs->directory) < 0)
        return -1;

    return 0;

}

/// @brief creates entry data for a new file and stores it in the directory
///        leaves the relative sector information blank
/// @param directory directory to modify
/// @param source_file file on the host system to base the meta data off
/// @param name 8.3 name for the new file
/// @param attributes file attributes
/// @return pointer to the file entry in the directory
directory_entry_t *d100_create_entry(dos100fs_t *fs, FILE *source_file,
                                     char *name, uint8_t attributes) {

    if(d100_find_entry(fs->directory, name))
        return NULL;

    directory_entry_t *free_entry = d100_get_free_entry(fs->directory);
    if(!free_entry)
        return NULL;

    if(fseek(source_file, 0, SEEK_END) < 0)
        return NULL;
    long source_file_size = ftell(source_file);
    if(source_file_size < 0)
        return NULL;

    char *dir_name = malloc(D100_NAME_EXT_LENGTH * sizeof(char));
    if(!dir_name) return NULL;
    d100_name_to_dir_name(name, dir_name);
    memcpy(free_entry->name, dir_name, D100_NAME_EXT_LENGTH);
    free(dir_name);

    free_entry->attributes = attributes;
    memset(free_entry->_reserved, 0, D100_RESERVED_BYTES);
    free_entry->update_date = d100_get_current_date();

    // will prevent silly behaviour if another file is created
    // before this one is written
    free_entry->first_relative_sector = 0;

    free_entry->size_bytes = source_file_size;

    return free_entry;

}

/// @brief allocates sectors in the file allocation table of a file system
///        for a file entry
/// @param fs file system to modify
/// @param entry file entry to allocate sectors for
///        this will be modified to set the first relative sector field
/// @return 0 for success; -1 otherwise
int d100_allocate_sectors(dos100fs_t *fs, directory_entry_t *entry) {

    // to make sure there are enough sectors for each track, this will make
    // sure that the division rounds up
    size_t sectors_needed = (entry->size_bytes + BYTES_PER_SECTOR - 1) /
                            BYTES_PER_SECTOR;
    if(!sectors_needed) {
        entry->first_relative_sector = RELSEC_END;
        return 0;
    }

    // to prevent errors, there should be a check to make sure there is enough
    // space in the fat and on the disk to store the file
    // it is unsafe to just throw an error half way through allocation

    fat_entry_t current_sector = d100_get_free_sector(fs->fat);
    if(!current_sector)
        return -1;
    fat_entry_t prev_sector;

    entry->first_relative_sector = current_sector;

    while(sectors_needed--) {

        prev_sector = current_sector;
        current_sector = d100_get_free_sector(fs->fat);
        if(!current_sector)
            return -1;
        d100_set_fat_entry(fs->fat, prev_sector, current_sector);
        d100_set_fat_entry(fs->fat, current_sector, RELSEC_END);

    }

    return 0;

}

/// @brief copies the data from a host file to a disk image
///        requires the sector list to have already been allcoated
///        and the values in the entry data to have been set
/// @param fs file system to copy into
/// @param dest entry data for the destination file
/// @param src file handler for the source file
/// @return 0 for success, -1 otherwise
int d100_copy_data(dos100fs_t *fs, directory_entry_t *dest, FILE *src) {

    void *src_buffer = malloc(BYTES_PER_SECTOR);
    if(!src_buffer)
        return -1;
    fat_entry_t current_sector = dest->first_relative_sector;

    for(int i = 0; current_sector != RELSEC_END; i++) {

        if(read_sectors(src, i, 1, src_buffer) < 0 && !feof(src))
            return -1;

        if(write_sectors(fs->disk, RELSEC_OFFSET + current_sector,
                         1, src_buffer) < 0)
            return -1;

        current_sector = d100_get_fat_entry(fs->fat, current_sector);

    }

    return 0;

}

