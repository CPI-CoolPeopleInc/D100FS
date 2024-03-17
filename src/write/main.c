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

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <d100.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    if(argc != 4) {
        fprintf(stderr, "Format: \"%s\" <DISK NAME> <SOURCE FILE> <DESTINATION FILE NAME>\n",
                argv[0]);
        return -1;
    }

    FILE *disk_image = fopen(argv[1], "rb+");
    if(!disk_image) {
        fprintf(stderr, "Error: Could not open disk image \"%s\": %s\n",
                argv[1], strerror(errno));
        return -1;
    }

    if(!d100_is_valid(disk_image)) {
        fprintf(stderr, "Disk image is not usable by PC/MS-DOS 1.00\n");
        fclose(disk_image);
        return -1;
    }

    FILE *source_file = fopen(argv[2], "r");
    if(!source_file) {
        fprintf(stderr, "Error: Could not open source file \"%s\": %s\n",
                        argv[2], strerror(errno));
        fclose(disk_image);
        return -1;
    }

    dos100fs_t *fs = d100_open(disk_image);
    if(!fs) {
        fprintf(stderr, "Error: Could not open file system\n");
        fclose(disk_image);
        fclose(source_file);
        return -1;
    }

    if(d100_create_file(fs, source_file, argv[3], D100_DEFAULT_ATTRS) != 0) {
        fprintf(stderr, "Error: Could not create the file on the disk image\n");
        d100_close(fs);
        fclose(disk_image);
        fclose(source_file);
        free(fs);
        return -1;
    }

    d100_close(fs);
    fclose(disk_image);
    fclose(source_file);
    free(fs);
    return 0;

}

