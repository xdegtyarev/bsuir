section .data
welcomemsg: db "ipr1-degtyarev",10, 0
torealdmsg: db "switching to real", 10, 0
inrealdmsg: db "in real", 10, 0
tonativemsg: db "switching to native", 10, 0
innativemsg: db "in native", 10, 0
statmsg: db "%x",10,0

section .bss
a: resd 1

section .text
	global main
	extern scanf
	extern printf

main:
	push ebp; setting up stackframe
	mov ebp,esp

	;;;;welcome
	push dword welcomemsg
	call printf
	add esp, 4; cleaning stack

	;;;;to native mode
	cli
	;lgdt [gdtr]	
	;mov eax, cr0
	;or eax, 1
	;mov cr0, eax
	;jmp 08h:nmain;

;	mov eax, cr0
;	mov [a], eax

;	push dword [a+4]
;	push statmsg
;	call printf
;	add esp, 8

	push dword inrealdmsg
	call printf
	add esp, 4; cleaning stack
	jmp end;

nmain:
	push dword innativemsg
	call printf
	add esp, 4; cleaning stack
	jmp end;

rmain:
	push dword inrealdmsg
	call printf
	add esp, 4; cleaning stack
	jmp end;	


end:
	mov esp, ebp; take down stack frame
	pop ebp

	mov eax,0; no error ret
	ret
