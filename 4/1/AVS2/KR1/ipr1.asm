section .data
toprotectedmsg: db "switching to protected", 10, 0
inprotectedmsg: db "in protected", 10, 0
tonativemsg: db "switching to native", 10, 0
innativemsg: db "in native", 10, 0

section .text
	global main
	extern scanf
	extern printf

main:
	push ebp; setting up stackframe
	mov ebp,esp

	;;;;welcome
	push dword inprotectedmsg
	call printf
	add esp, 4; cleaning stack
	jmp end;


end:
	mov esp, ebp; take down stack frame
	pop ebp

	mov eax,0; no error ret
	ret
