; MIT License
; 
; Copyright (c) 2024 CPI-CoolPeopleInc
; 
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
; 
; The above copyright notice and this permission notice shall be included in all
; copies or substantial portions of the Software.
; 
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
; SOFTWARE.

; +======================================+
; |                                      |
; | default boot record for D100FS tools |
; |                                      |
; | Written: 19th Feb 2024               |
; | Last Updated: 17th Mar 2024          |
; |                                      |
; | Written by:                          |
; | - Gabriel Jickells                   |
; |                                      |
; +======================================+

%define BYTES_PER_SECTOR 512
%define BOOT_RECORD_SECTORS 1

%define START_ADDRESS 7C00h

%define BIOS_VIDEO_WRITE_CHAR 14
%define BIOS_VIDEO_INT 10h
%define BIOS_KBD_INT 16h
%define BIOS_RESTART_INT 19h

%define SYS_ENDL 0x0d, 0x0a             ; CRLF line feed
%define SYS_NUL 0

%define TARGET_ARCHITECTURE 8086
%define TARGET_WORD_SIZE 16

org START_ADDRESS
cpu TARGET_ARCHITECTURE
bits TARGET_WORD_SIZE

print_setup:
    xor bx, bx
    mov ds, bx
    ; if you want to be on the safe side then uncomment the next line
    ; cld
    mov si, str_error

print_loop:
    lodsb
    test al, al
    jz print_end
    
    mov ah, BIOS_VIDEO_WRITE_CHAR
    int BIOS_VIDEO_INT

    jmp print_loop

print_end:

wait_for_key:
    xor ah, ah                          ; quicker way of setting ah to 0 which
                                        ; is the bios read key command
    int BIOS_KBD_INT
    int BIOS_RESTART_INT

str_error: db "This is not a bootable disk. Insert one and press any key to restart", SYS_ENDL, SYS_NUL

times (BYTES_PER_SECTOR * BOOT_RECORD_SECTORS)-($-$$) db 0

