[org 0x7C00] ; BIOS boot origin
[bits 16] ; 16-bit Real Mode
jmp start ;Jump to start() entry-point

%include "routines16.asm"

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
  ; mov bp,7c00h
  ; mov sp,7c00h
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
welcomeMsg db "IPR2-degtyarev. Press any key to start", 0x0
inrealmsg db "Currently in real-mode", 0x0
anykeyMsg db "Press any key to switch cpu mode and start tasks", 0x0
;;;;;;;;;;;;;;

%include "gdt32.asm"
%include "routines32.asm"

[bits 32]

protectedMain:
  xor eax, eax
  mov eax, DATA_SEG_32
  mov ds, eax
  mov es, eax
  ;;maybe stack init
  ;load TSS0
  mov ax, TASK0_SEG_32
  ltr ax
  jmp CODE_SEG_32:task1

  mov eax, 0x0000; zero line
  mov ebx, inprotectedmsg
  call print32
  add eax, 0x00A0; moving next line

task0:
  jmp TASK1_SEG_32:task1
  mov ebx, inprotectedmsg
  call print32
  add eax, 0x00A0; moving next line
  jmp task0

task1:
  jmp TASK0_SEG_32:task1
  mov ebx, inprotectedmsg
  call print32
  add eax, 0x00A0; moving next line
  jmp task1

end:
  jmp end

;;;;;;;;;;;;;;
inprotectedmsg db "Currently in protected-mode", 0x0
task0msg db "Task 0", 0x0
task1msg db "Task 1", 0x0
;;;;;;;;;;;;;;

times 510 - ($-$$) db 0 ;Fill the rest of the bootloader with zeros
dw 0xAA55 ;Boot signature


