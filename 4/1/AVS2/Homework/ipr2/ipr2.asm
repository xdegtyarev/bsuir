[org 0x7C00] ; BIOS boot origin
[bits 16] ; 16-bit Real Mode
jmp start ;Jump to start() entry-point

%include "routines16.asm"

start:
  mov si, welcomeMsg
  call print

realMain:
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
welcomeMsg db "IPR2", 0x0
anykeyMsg db "Press any key", 0x0
;;;;;;;;;;;;;;

[bits 32]
%include "gdt32.asm"
%include "routines32.asm"

protectedMain:
  xor eax, eax
  mov eax, DATA_SEG_32
  mov ds, eax
  mov es, eax

  mov eax, 0x0000; zero line
  mov ebx, inprotectedmsg
  call print32
  add eax, 0x00A0; moving next line
  ;;maybe stack init
  ;calling task one
  mov ax, TASK0_SEG_32
  ltr ax

  jmp TASK0_SEG_32:0

task0:
  mov ebx, task0msg
  call print32
  add eax, 0x00A0; moving next line
  jmp TASK1_SEG_32:0
  jmp end
  jmp task0

task1:
  jmp TASK0_SEG_32:task1
  mov ebx, task1msg
  call print32
  add eax, 0x00A0; moving next line
  jmp task1

end:
  jmp end

;;;;;;;;;;;;;;
inprotectedmsg db "protected", 0x0
task0msg db "T0", 0x0
task1msg db "T1", 0x0
;;;;;;;;;;;;;;

times 510 - ($-$$) db 0 ;Fill the rest of the bootloader with zeros
dw 0xAA55 ;Boot signature


