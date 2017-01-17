[org 0x7C00] ; BIOS boot origin
[bits 16] ; 16-bit Real Mode
jmp start ;Jump to start() entry-point
%include "routines16.asm"

gdt16_descriptor: ;16 gdt
dw 0x0047
dd 0x000fe898

start:
  mov si, welcomeMsg
  call print

realMain:
  cli
  mov eax, cs
  mov ds, eax
  mov ss, eax
  sti

  mov si, inrealmsg
  call print

  mov si, anykeyMsg
  call print
  call getKey

  call toProtected

toProtected:
  cli
  lgdt [gdt32_descriptor] ; Load GDT
  mov eax, cr0
  or eax, 0x1
  mov cr0, eax
  jmp dword CODE_SEG_32:protectedMain

;;;;;;;;;;;;;;
welcomeMsg db "IPR1-degtyarev", 0x0
inrealmsg db "in real", 0x0
anykeyMsg db "Press any key to switch cpu mode...", 0x0
;;;;;;;;;;;;;;

%include "gdt32.asm"
%include "routines32.asm"

[bits 32]
protectedMain:
  mov eax, DATA_SEG_32
  mov ds, eax
  mov ss, eax

  mov eax, 0x0000; zero line
  mov ebx, inprotectedmsg
  call print32
  add eax, 0x00A0; moving next line

  call toReal

toReal:
  cli
  lgdt [gdt16_descriptor]
  mov eax, cr0
  dec al
  mov cr0, eax
  jmp word 0:realMain

end:
  jmp end

;;;;;;;;;;;;;;
inprotectedmsg db "in protected", 0x0
;;;;;;;;;;;;;;

times 510 - ($-$$) db 0 ;Fill the rest of the bootloader with zeros
dw 0xAA55 ;Boot signature