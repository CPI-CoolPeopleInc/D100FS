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

#include "chkdsk.h"

#define BYTES_1KB 1024
#define BYTES_160KB (BYTES_1KB*160)

/// @brief checks if a disk image is supported by PC/MS-DOS 1.00
/// @param disk disk image to check
/// @return 1 if it is supported and 0 if not
int d100_is_valid(FILE* disk) {
    
    if(fseek(disk, 0, SEEK_END) < 0)
        return 0;

    if(ftell(disk) != BYTES_160KB)
        return 0;

    return 1;

}
