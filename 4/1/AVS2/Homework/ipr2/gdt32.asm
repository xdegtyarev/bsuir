gdt32_start:

gdt32_null: 		; Initialization null
	dq 0x0

gdt32_cs:
	dw 0xFFFF    ; Limit
	dw 0x0000    ; Base
	db 0x0000	 ; Base 23:16
	db 10011011b ; [p][dpl][][s][type][][][a]
	db 11011111b ; [g][x][0][avl][lim][][][]
	db 0x0000

gdt32_ds:
	dw 0xFFFF    ; Limit
	dw 0x0000    ; Base
	db 0x0000	 ; Base 23:16
	db 10010011b
	db 11011111b
	db 0x0000

gtd32_task0:
	dw 0x0067    ; Limit
	dw 0x0000    ; Base
	db 0x0000	 ; Base 23:16
	db 10001001b ; [p][dpl][dpl][s][type][type][typeb][type]
	db 01000000b ; [g][x][0][avl][lim][lim][lim][lim]
	db 0x0000

gtd32_task1:
	dw 0x0067    ; Limit
	dw 0x0000    ; Base
	db 0x0000	 ; Base 23:16
	db 10001001b ; [p][dpl][dpl][s][type][type][typeb][type]
	db 01000000b ; [g][x][0][avl][lim][lim][lim][lim]
	db 0x0000

gdt32_end:

gdt32_descriptor:
	dw gdt32_end - gdt32_start - 1 ; GDT size
	dd gdt32_start

; Constants to get address of gdt32
CODE_SEG_32 equ gdt32_cs - gdt32_start
DATA_SEG_32 equ gdt32_ds - gdt32_start
TASK0_SEG_32 equ gtd32_task0 - gdt32_start
TASK1_SEG_32 equ gtd32_task1 - gdt32_start

TSS_0: TIMES 104 db 0
; TSS_1: TIMES 104 db 0
