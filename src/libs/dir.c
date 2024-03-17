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

#include "dir.h"
#include "disk.h"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "minmax.h"
#include <ctype.h>

/// @brief writes a directory to a disk image
/// @param disk disk image to write to
/// @param directory directory to write. Must be D100_DIRECTORY_SECTORS sectors in size
/// @return 0 on success, -1 otherwise
int d100_write_directory(FILE *disk, directory_entry_t *directory) {

    return write_sectors(disk, D100_DIRECTORY_LBA, D100_DIRECTORY_SECTORS,
                         directory);

}

/// @brief empties a directory
/// @param directory directory to clear
void d100_clear_directory(directory_entry_t *directory) {

    for(int i = 0; i < D100_MAX_DIRECTORY_ENTRIES; i++)
        directory[i].name[D100_DIRECTORY_DELETED_INDEX] = D100_DIRECTORY_DELETED;

}

/// @brief reads a directory from a disk image
/// @param disk image to read from
/// @return pointer to heap allocated directory on success, NULL otherwise
directory_entry_t *d100_read_directory(FILE *disk) {
    
    directory_entry_t *directory = malloc(D100_DIRECTORY_SECTORS * BYTES_PER_SECTOR);
    if(!directory) return NULL;

    if(read_sectors(disk, D100_DIRECTORY_LBA, D100_DIRECTORY_SECTORS,
                    directory) != 0) {
        free(directory);
        return NULL;
    }

    return directory;
 
}

/// @brief finds a free entry in the directory of a file system
/// @param directory directory to look through
/// @return pointer to the free entry in the directory on success, else NULL
directory_entry_t *d100_get_free_entry(directory_entry_t *directory) {

    for(size_t i = 0; i < D100_MAX_DIRECTORY_ENTRIES; i++)
        if(directory[i].name[D100_DIRECTORY_DELETED_INDEX] ==
           D100_DIRECTORY_DELETED)
            return &directory[i];

        return NULL;

}

/// @brief finds an entry by its name in a directory
/// @param directory directory to look through
/// @param name 8.3 name to look for
/// @return pointer to the entry in the directory on success, NULL otherwise
directory_entry_t *d100_find_entry(directory_entry_t *directory, char *name) {

    // NULL terminator isn't important because memcmp assumes the length
    char *dir_name = malloc(D100_NAME_EXT_LENGTH * sizeof(char));
    if(!dir_name) return NULL;
    d100_name_to_dir_name(name, dir_name);
    
    for(size_t i = 0; i < D100_MAX_DIRECTORY_ENTRIES; i++)
        if(!memcmp(dir_name, directory[i].name, D100_NAME_EXT_LENGTH)) {
            free(dir_name);
            return &directory[i];
        }

    free(dir_name);
    return NULL;

}

/// @brief converts an 8.3 file name into the format used on the disk
/// @param name 8.3 name to convert
/// @param dest destination to store the converted name. Must be
///        D100_NAME_EXT_LENGTH characters in size
void d100_name_to_dir_name(char *name, char *dest) {

    // Todo: check that the name is valid
    
    char *name_bak = malloc(strlen(name));
    memcpy(name_bak, name, strlen(name));

    memset(dest, ' ', D100_NAME_EXT_LENGTH);
    char *dot = strchr(name, '.');
    char *ext = NULL;

    if(dot) {
        *dot = '\0';    // splits name and ext into seperate strings
        ext = ++dot;
    }

    memcpy(dest, name, min(D100_NAME_LENGTH, strlen(name)));
    if(ext)
        memcpy(&dest[D100_NAME_LENGTH], ext,
               min(D100_EXT_LENGTH, strlen(ext)));

    for(size_t i = 0; i < D100_NAME_EXT_LENGTH; i++)
        dest[i] = toupper(dest[i]);

    memcpy(name, name_bak, strlen(name_bak));
    return;

}

