[org 0x7C00] ; BIOS boot origin
[bits 16] ; 16-bit Real Mode
jmp start ;Jump to start() entry-point

start:
  xor ebx,ebx
  mov bx,cs   ; EBX=segment
  shl ebx,4   ; << 4
  lea eax,[ebx]   ; EAX=linear address of segment base
  ; fix up TSS entries, too
  lea eax,[ebx + stss]  ; EAX=linear address of stss
  mov [gdt32_st + 2],ax
  shr eax,16
  mov [gdt32_st + 4],al
  mov [gdt32_st + 7],ah
  lea eax,[ebx + utss]  ; EAX=linear address of utss
  mov [gdt32_ut + 2],ax
  shr eax,16
  mov [gdt32_ut + 4],al
  mov [gdt32_ut + 7],ah

  cli
  lgdt [gdt32_descriptor] ; Load GDT
  mov eax, cr0
  or eax, 0x1
  mov cr0, eax
  jmp dword CODE_SEG_32:pmain

[bits 32]
%include "gdt32.asm"
%include "routines32.asm"

pmain:
  mov ax, DATA_SEG_32
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  ;;loading task0 TSS
  mov ax, STASK_SEG_32;
  ltr ax

  xor eax, eax
  xor ecx, ecx

task0:
  mov eax, ecx
  mov ebx, t0msg
  call print32
  add ecx, 0x0002; moving next line

  lea eax,[task1]    ; task entry point
  mov [utss_eip],eax
  mov [utss_esp],esp
  jmp UTASK_SEG_32:0
  pusha
  mov        ecx,02000000h        ; delay
  loop       $
  popa
  cmp ecx, 0x0080
  jb task0
  jmp $

task1:
  mov eax, ecx
  add ecx, 0x0002;
  task1loop:
    add eax, 0x00A0
    mov ebx, t1msg
    call print32
    pusha
    mov        ecx,02000000h        ; delay
    loop       $
    popa
    cmp eax, 0x0780
    jb task1loop

  lea eax,[task1]    ; task entry point
  jmp STASK_SEG_32:0

  jmp $

;;;;;;;;;;;;;;
t0msg db "0", 0x0
t1msg db "1", 0x0
;;;;;;;;;;;;;;

times 510 - ($-$$) db 0 ;Fill the rest of the bootloader with zeros
dw 0xAA55 ;Boot signature


