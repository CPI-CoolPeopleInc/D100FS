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

#include "bootrec.h"

// see brdef.asm in the libs folder for clearer and easier to modify code
uint8_t default_boot_record[BOOT_RECORD_SECTORS * BYTES_PER_SECTOR] = {

    // print setup
    0x31, 0xdb,                 // xor bx, bx
    0x8e, 0xdb,                 // mov ds, bx
    0xbe, 0x18, 0x7c,           // mov si, str_error

    // print loop
    0xac,                       // lodsb
    0x84, 0xc0,                 // test al, al
    0x74, 0x06,                 // jz print_end

    0xb4, 0x0e,                 // mov ah, 0eh          ; 0eh tells BIOS to print character to screen
    0xcd, 0x10,                 // int 10h              ; int 10h calls BIOS video functions
    0xeb, 0xf5,                 // jmp print_loop

    // print end

    // wait for key and reboot
    0x30, 0xe4,                 // xor ah, ah           ; ah = 0 tell BIOS to read character from the user
    0xcd, 0x16,                 // int 16h              ; int 16h calls BIOS keyboard functions
    0xcd, 0x19,                 // int 19h              ; int 19h tells BIOS to retry booting from floppy

    // str_error: db "This is not a bootable disk. Insert one and press any key to restart", 0x0d, 0x0a, 0
    // this can be changed to whatever you want as long as the final array is the correct size. Remove
    // pad bytes where necessary. Line feeds must be CRLF and there must be a NULL terminator
    'T', 'h', 'i', 's', ' ', 'i', 's', ' ', 'n', 'o', 't', ' ', 'a', ' ', 'b',
    'o', 'o', 't', 'a', 'b', 'l', 'e', ' ', 'd', 'i', 's', 'k', '.', ' ', 'I',
    'n', 's', 'e', 'r', 't', ' ', 'o', 'n', 'e', ' ', 'a', 'n', 'd', ' ', 'p',
    'r', 'e', 's', 's', ' ', 'a', 'n', 'y', ' ', 'k', 'e', 'y', ' ', 't', 'o',
    ' ', 'r', 'e', 's', 't', 'a', 'r', 't', '\r', '\n', 0,

    // pad bytes
    0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

};

/// @brief writes the default boot record onto a disk image
/// @param disk image to write to
/// @return 0 for success, -1 otherwise
int d100_write_boot_record(FILE *disk) {
    
    return write_sectors(disk, BOOT_RECORD_LBA, BOOT_RECORD_SECTORS,
                         default_boot_record);

}

