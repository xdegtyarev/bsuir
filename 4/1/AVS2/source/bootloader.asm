bits 16 ; 16-bit Real Mode
org 0x7c00 ; BIOS boot origin

jmp rmain ;Jump to start main() entry-point

;;;;;;;;;;;;;;
; vars
welcomeMsg: db "ipr1-degtyarev", 0x0
bootloaderMsg db "16bit bootloader.", 0x0
anykeyMsg db "Press any key to restart...", 0x0
;;;;;;;;;;;;;;

;BIOS INT CODES:
;0x10 : 0x0E Write Character in TTY Mode
;0x16 : 0x00 Read Character

print:
    lodsb ;Load string
    or al, al
    jz complete
    mov ah, 0x0E
    int 0x10
jmp print ;Loop

complete:
    call printNewLine

printNewLine:
    mov al, 0	; null terminator '\0'
    stosb ; Store string

    mov ah, 0x0E
    mov al, 0x0D ; carriage return char
    int 0x10
    mov al, 0x0A ; new line
    int 0x10
	ret

restart:
    mov si, anykeyMsg
    call print
    call getKey

    ;Sends us to the end of the memory
    ;causing reboot
    db 0x0ea
    dw 0x0000
    dw 0xffff

getKey:
    mov ah, 0x00
    int 0x16
    ret

;entry-code
rmain:
   cli ;Clear interrupts
   ;Setup stack segments
   mov ax,cs
   mov ds,ax
   mov es,ax
   mov ss,ax
   sti ;Enable interrupts

   ;Print the first characters
   mov si, welcomeMsg
   call print

   mov si, bootloaderMsg
   call print

   call restart

   times 510 - ($-$$) db 0 ;Fill the rest of the bootloader with zeros
   dw 0xAA55 ;Boot signature