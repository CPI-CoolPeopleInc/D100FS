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

int main(int argc, char** argv) {

    if(argc != 2) {
        fprintf(stderr, "Format: \"%s\" <DISK IMAGE>\n", argv[0]);
        return -1;
    }

    FILE* disk_image = fopen(argv[1], "rb+");
    if(!disk_image) {
        fprintf(stderr, "Could not open disk image \"%s\": %s\n",
                argv[1], strerror(errno));
        return -1;
    }

    if(!d100_is_valid(disk_image)) {
        fprintf(stderr, "Disk image is not usable by PC/MS-DOS 1.00\n");
        printf("DOS only supports 160KB floppy disk images\n");
        fclose(disk_image);
        return -1;
    }

    dos100fs_t *newfs = d100_format(disk_image);
    if(!newfs) {
        fprintf(stderr, "Could not format disk image\n");
        fclose(disk_image);
        return -1;
    }

    d100_close(newfs);
    fclose(disk_image);
    free(newfs);
    
    return 0;

}

