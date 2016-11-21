[org 0x7C00] ; BIOS boot origin
[bits 16] ; 16-bit Real Mode

jmp start ;Jump to start() entry-point
%include "routines16.asm"
[bits 16]

start:
  mov si, welcomeMsg
  call print
  call getKey
  call clear

realMain:
  cli
  mov ax, cs
  mov ds, ax
  mov es, ax
  ; mov ax, STACK16
  ; mov ss, ax
  sti

  mov si, inrealmsg
  call print

  mov si, anykeyMsg
  call print
  call getKey

  call toProtected

toProtected:
  mov si, toprotectedmsg
  call print
  call clear;
  ;move
  cli
  lgdt [gdt32_descriptor] ; Load GDT
  mov eax, cr0
  or eax, 0x1
  mov cr0, eax
  jmp dword CODE_SEG_32:protectedMain

;;;;;;;;;;;;;;
welcomeMsg db "IPR1-degtyarev. Press any key to start", 0x0
inrealmsg db "in real", 0x0
toprotectedmsg db "switching to protected", 0x0
anykeyMsg db "Press any key to switch cpu mode...", 0x0
;;;;;;;;;;;;;;

%include "gdt32.asm"
%include "routines32.asm"

[bits 32]

protectedMain:
  mov eax, DATA_SEG_32
  mov ds, eax
  mov es, eax

  mov eax, 0x0000; zero line
  mov ebx, inprotectedmsg
  call print32
  add eax, 0x00A0; moving next line
  call toReal

toReal:
  mov edx, eax
  mov ebx, torealmsg
  call print32
  ;move
  cli

  mov eax, cr0
  dec al
  mov cr0, eax

  jmp 0x7C00:realMain

end:
  jmp end

;;;;;;;;;;;;;;
inprotectedmsg db "in protected", 0x0
torealmsg db "switching to real", 0x0
;;;;;;;;;;;;;;

times 510 - ($-$$) db 0 ;Fill the rest of the bootloader with zeros
dw 0xAA55 ;Boot signature


