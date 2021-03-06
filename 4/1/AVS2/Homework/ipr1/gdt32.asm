; Descriptor CONFIG
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

gdt32_end:

gdt32_descriptor:
	dw gdt32_end - gdt32_start - 1 ; GDT size
	dd gdt32_start

; Constants to get address of gdt32
CODE_SEG_32 equ gdt32_cs - gdt32_start
DATA_SEG_32 equ gdt32_ds - gdt32_start

