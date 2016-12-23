clear:
    pusha
    mov ax, 0x0700  ; function 07, AL=0 means scroll whole window
    mov bh, 0x07    ; character attribute = white on black
    mov cx, 0x0000  ; row = 0, col = 0
    mov dx, 0x184f  ; row = 24 (0x18), col = 79 (0x4f)
    int 0x10        ; call BIOS video interrupt
    popa
    ret

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
    mov al, 0x00; null terminator '\0'
    stosb; Store string

    mov ah, 0x0E
    mov al, 0x0D; carriage return char
    int 0x10
    mov al, 0x0A; new line
    int 0x10
	ret

restart:
    mov si, anykeyMsg
    call print
    call getKey
    db 0x0ea; Sends to the end of memory causing reboot
    dw 0x0000
    dw 0xffff

getKey:
    pusha
    mov ah, 0x00
    int 0x16
    popa
    ret