gdt16_start:

gdt16_null: 		; Initialization null
	dq 0x0

gdt16_cs:
	dw 0xFFFF    ; Limit
	dw 0x0000    ; Base
	db 0x0000	 ; Base 23:16
	db 10011011b ; [p][dpl][][s][type][C/D][R/W][a]
	db 00011111b ; [g][x][0][avl][lim][][][]
	db 0x0000   ; Base 24-31

gdt16_ds:
	dw 0xFFFF    ; Limit
	dw 0x0000    ; Base
	db 0x0000	 ; Base 23:16
	db 10010010b
	db 00011111b
	db 0x0000

gdt16_end:

gdt16_descriptor:
	dw gdt16_end - gdt16_start - 1 ; GDT size
	dd gdt16_start

; Constants to get address of gdt16
CODE_SEG_16 equ gdt16_cs - gdt16_start
DATA_SEG_16 equ gdt16_ds - gdt16_start

