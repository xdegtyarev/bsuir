gdt32:
	; Initialization null
	dq 0x0

CODE_SEG_32 equ $ - gdt32
gdt32_cs:
	dw 0xFFFF    ; Limit
	dw 0x0000    ; Base
	db 0x0000	 ; Base 23:16
	db 10011011b ; [p][dpl][][s][type][][][a]
	db 11011111b ; [g][x][0][avl][lim][][][]
	db 0x0000

DATA_SEG_32 equ $ - gdt32
gdt32_ds:
	dw 0xFFFF    ; Limit
	dw 0x0000    ; Base
	db 0x0000	 ; Base 23:16
	db 10010011b
	db 11011111b
	db 0x0000

STASK_SEG_32 equ $ - gdt32
gdt32_st:
	dw 0x0067    ; Limit
	dw 0x0000    ; Base
	db 0x0000	 ; Base 23:16
	db 10001001b ; [p][dpl][dpl][s][type][type][typeb][type]
	db 00000000b ; [g][x][0][avl][lim][lim][lim][lim]
	db 0x0000

UTASK_SEG_32 equ $ - gdt32
gdt32_ut:
	dw 0x0067    ; Limit
	dw 0x0000    ; Base
	db 0x0000	 ; Base 23:16
	db 10001001b ; [p][dpl][dpl][s][type][type][typeb][type]
	db 00000000b ; [g][x][0][avl][lim][lim][lim][lim]
	db 0x0000

gdt32_end:

gdt32_descriptor:
	dw gdt32_end - gdt32 - 1 ; GDT size
	dd gdt32

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	task state segments
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

stss:	dw 0, 0			; back link
	dd 0			; ESP0
	dw 0, 0			; SS0, reserved
	dd 0			; ESP1
	dw 0, 0			; SS1, reserved
	dd 0			; ESP2
	dw 0, 0			; SS2, reserved
	dd 0, 0, 0		; CR3, EIP, EFLAGS
	dd 0, 0, 0, 0		; EAX, ECX, EDX, EBX
	dd 0, 0, 0, 0		; ESP, EBP, ESI, EDI
	dw 0, 0			; ES, reserved
	dw 0, 0			; CS, reserved
	dw 0, 0			; SS, reserved
	dw 0, 0			; DS, reserved
	dw 0, 0			; FS, reserved
	dw 0, 0			; GS, reserved
	dw 0, 0			; LDT, reserved
	dw 0, 0			; debug, IO perm. bitmap

utss:	dw 0, 0			; back link
	dd 0			; ESP0
	dw 0, 0			; SS0, reserved
	dd 0			; ESP1
	dw 0, 0			; SS1, reserved
	dd 0			; ESP2
	dw 0, 0			; SS2, reserved
	dd 0			; CR3
utss_eip:
	dd 0, 0			; EIP, EFLAGS (EFLAGS=0x200 for ints)
	dd 0, 0, 0, 0		; EAX, ECX, EDX, EBX
utss_esp:
	dd 0, 0, 0, 0		; ESP, EBP, ESI, EDI
	dw DATA_SEG_32, 0	; ES, reserved
	dw CODE_SEG_32, 0	; CS, reserved
	dw DATA_SEG_32, 0	; SS, reserved
	dw DATA_SEG_32, 0	; DS, reserved
	dw DATA_SEG_32, 0	; FS, reserved
	dw DATA_SEG_32, 0	; GS, reserved
	dw 0, 0			; LDT, reserved
	dw 0, 0			; debug, IO perm. bitmap
end:
