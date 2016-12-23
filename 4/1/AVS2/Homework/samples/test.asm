;---------------------------------------------------------
;--
;-- Bootsector
;-- at 0x07C00
;--
;---------------------------------------------------------
org 0x7C00
Bits  16
jmp  0x0000:boot_begin

;---------------------------------------------------------
;--
;--	Variables
;--
;---------------------------------------------------------
msg_step_1:  db 'Step 1 - successful', 0
msg_step_2:  db 'Step 2 - successful', 0
msg_step_3:  db 'Step 3 - successful', 0
msg_step_4:  db 'Step 4 - successful', 0
msg_step_5:  db 'Step 5 - successful', 0
msg_step_6:  db 'Step 6 - successful', 0
msg_step_7:  db 'Step 7 - successful', 0
msg_step_8:  db 'Step 8 - successful', 0
msg_step_9:  db 'Step 9 - successful', 0
r_segment:  dw 0x0000
r_offset:  dw 0x7E00
r_count   db 0x03
r_track   db 0x00
r_start   db 0x02
msg_floppy_bad:  db 'Unable to read floppy', 0

nop

;---------------------------------------------------------
;--
;--	Procedures
;--
;---------------------------------------------------------
;-----------------------------------------
;--	Print_String
;-----------------------------------------
print_string:
mov ah , 0x0E
mov bx , 0x0006
mov cx , 0x50

print_string_repeat:
lodsb
cmp al , 0
je print_string_empty
int 0x10
dec cx
jmp print_string_repeat

print_string_empty:
cmp cx , 0x00
je print_string_done
mov al , 0x20
int 0x10
dec cx
jmp print_string_empty

print_string_done:
ret

;-----------------------------------------
;--	Read_Floppy
;-----------------------------------------
read_floppy:
mov cx , 0x50
read_floppy_init:
xor ax , ax
int 0x13
jc read_floppy_init_failed
mov cx , 0x50
jmp read_floppy_content

read_floppy_init_failed:
cmp cx , 0x00
je reset
dec cx
jmp read_floppy_init

read_floppy_content:
push cx
mov ax , WORD[r_segment]
mov es , ax
mov bx , WORD[r_offset]
mov ah , 0x02
mov al , BYTE[r_count]
mov ch , BYTE[r_track]
mov cl , BYTE[r_start]
mov dh , 0x00
mov dl , 0x00
int 0x13
pop cx
jc read_floppy_read_failed
ret

read_floppy_read_failed:
cmp cx , 0
je reset
dec cx
jmp read_floppy_content

reset:
mov si , msg_floppy_bad
call print_string
ret

;----------------------------------------
;-	Setup GDT
;----------------------------------------
gdt_start:
dd 0
dd 0

; Codesegment:
dw 0xFFFF
dw 0
db 0
db 10011010b
db 11001111b
db 0

; Datasegment:
dw 0xFFFF
dw 0
db 0
db 10010010b
db 11001111b
db 0
gdt_end:

gdt_point:
dw gdt_end - gdt_start - 1
dd gdt_start

;---------------------------------------------------------
;--
;--	Begin Bootloader
;--
;---------------------------------------------------------

boot_begin:
;----------------------------------------
;-	Setup Stack
;----------------------------------------
mov  ax,  0x0000
mov  ss,  ax
mov  ds,  ax
mov es, ax
mov  sp,  0xCC00

;----------------------------------------
;-	Create an empty screen
;----------------------------------------
mov  ah,  0x00
mov  al,  0x03
int  0x10

;----------------------------------------
;-	Step 1 - First Message
;----------------------------------------
mov si, msg_step_1
call print_string

;----------------------------------------
;-	Step 2 - Read Floppy
;----------------------------------------
call read_floppy
mov si, msg_step_2
call print_string
;----------------------------------------
;-	Jump to Second Bootloader
;----------------------------------------
jmp 0x0000:bootloader2

;-----------------------------------------
;--   Boot Sequence AA-55
;-----------------------------------------
times 512-($-$$) db 0
dw 0xAA55



;---------------------------------------------------------
;--
;--	Begin  2. Bootloader
;--
;---------------------------------------------------------

bootloader2:
;---------------------------------------------
;-	Step 3 - 2. Bootloader Message
;---------------------------------------------
mov si, msg_step_3
call print_string

;----------------------------------------
;-	Step 4 - Installing GDT
;----------------------------------------
cli
lgdt [gdt_point]
sti
mov si, msg_step_4
call print_string

;----------------------------------------
;-	Step 5 - Enable A20 Line
;----------------------------------------
pusha
mov  ax,  0x2401
int  0x15
popa

pusha
cli
in	al , 0x92
or	al , 2
out   0x92 , al
sti
popa

mov si, msg_step_5
call print_string

;----------------------------------------
;-	Step 6 - Remapp IRQ 0-15 to 32-48 -
;----------------------------------------
pusha
cli
mov   al , 0x11
out   0x20 , al
out   0xA0 , al
mov   al , 0x20
out   0x21 , al
mov   al , 0x28
out   0xA1 , al

mov   al , 0x04
out   0x21 , al
mov   al , 0x02
out   0xA1 , al
mov   al , 0x01
out   0x21 , al
out   0xA1 , al
sti
popa
mov si, msg_step_6
call print_string

;----------------------------------------
;-	Step 7 - Installing IDT   -
;----------------------------------------
pushad
cli
lidt	[idt_point]
sti
popad
mov si, msg_step_7
call print_string

;----------------------------------------
;-	Switching to Pmode  -
;----------------------------------------
cli
mov   eax , cr0
or	eax , 1
mov   cr0 , eax

;----------------------------------------
;-	Far Jump to 0x8:pmode  -
;----------------------------------------
jmp dword 0x8:pmode


;----------------------------------------
;-	Setup IDT	-
;----------------------------------------
idt_start:
irq0:
		dw isr0
		dw 0x0008
		db 0x00
		db 10101110b
		dw 0x0000
irq1:
		dw isr1
		dw 0x0008
		db 0x00
		db 10101110b
		dw 0x0000
irq2:
		dw isr2
		dw 0x0008
		db 0x00
		db 10101110b
		dw 0x0000

; ...
; ... (the code between here is not different)
; ...

irq32:
		dw isr32
		dw 0x0008
		db 0x00
		db 10101110b
		dw 0x0000
irq33:
		dw isr33
		dw 0x0008
		db 0x00
		db 10101110b
		dw 0x0000
idt_end:


idt_point:
		dw idt_end - idt_start - 1
		dw 0x0000
		dw idt_start


isr0:
		pusha
		; interrupt handler for isr0
		popa
		iret
isr1:
		pusha
		; interrupt handler for isr1
		popa
		iret
isr2:
		pusha
		; interrupt handler for isr2
		popa
		iret

; ...
; ... (the code between here is not different)
; ...

isr32:
		pusha
		; interrupt handler for isr32
		popa
		iret
isr33:
		pusha
		; interrupt handler for isr33
		popa
		iret

;---------------------------------------------------------
;--
;--	Step 8 - PMODE Section
;--
;---------------------------------------------------------

pmode:
Bits  32
mov ax , 0x10
mov ds , ax
mov es , ax
mov fs , ax
mov gs , ax
mov ss , ax
mov esp , 0x9000
jmp $

times 1024-($-$$) db 0
