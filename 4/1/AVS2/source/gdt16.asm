gdt16_start:

gdt16_null: 		; Initialization null
	dd 0x0
	dd 0x0

gdt16_cs:
	dw 0xFFFF    ; Limit
	dw 0x0000    ; Base
	db 0x0000	 ; Base 23:16
	db 10011010b
	db 11011111b
	db 0x0000   ; Base 24-31

gdt16_ds:
	dw 0xFFFF    ; Limit
	dw 0x0000    ; Base
	db 0x0000	 ; Base 23:16
	db 10010010b
	db 11011111b
	db 0x0000

gdt16_end:

gdt16_descriptor:
	dw gdt16_end - gdt16_start - 1 ; GDT size
	dd gdt16_start

; Constants to get address of gdt16
CODE_SEG_16 equ gdt16_cs - gdt16_start
DATA_SEG_16 equ gdt16_ds - gdt16_start

