section .data
titlemsg: db "a(x^2) + b x + c solver", 10, 0
reseq: db "Lets solve %f(x^2) + %f(x) + %f = 0",10, 0
resmsg: db "Roots are %f , %f",10,0
oneresmsg: db "Single root is %f",10,0
imagemsg: db "Desc is below 0, imaginary roots",10,0
statmsg: db "%x",10,0
amsg: db "A is:",10,0
bmsg: db "B is:",10,0
cmsg: db "C is:",10,0

four: dq 4.0
two: dq 2.0

flfmt: db "%lf",0

section .bss
a: resq 1
b: resq 1
c: resq 1
status: resd 1
root1: resq 1
root2: resq 1

section .text
	global main
	extern scanf
	extern printf

main:
push ebp; setting up stackframe
mov ebp,esp

;;;;welcome
push dword titlemsg
call printf
add esp, 4; cleaning stack

;;;;Areq
push dword amsg
call printf
add esp, 4; cleaning stack
push a
push flfmt; %lf (double)
call scanf
add esp, 8

;;;;Breq
push dword bmsg
call printf
add esp, 4; cleaning stack
push b
push flfmt; %lf (double)
call scanf
add esp, 8

;;;;Creq
push dword cmsg
call printf
add esp, 4; cleaning stack
push c
push flfmt; %lf (double)
call scanf
add esp, 8

;;;Prin EQ
push dword [c+4]
push dword [c]
push dword [b+4]
push dword [b]
push dword [a+4]
push dword [a]
push reseq
call printf
add esp, 24

;;;; solve
finit
	fld qword [a]
	fmul qword [four]
	fmul qword [c]
	fld qword [b]
	fmul st0
	fsub st0, st1; b^2 - 4ac
	
	ftst; d>0
	fstsw [status]
	fwait
	
	mov ah, byte [status+1]
	sahf; store compare result in flags

	jc imaginary
	
	fsqrt; st0 = sqrt (b^2-4ac)
	fld st0; duplicate sqr
	fchs; change sign
	fadd qword [b]
	fchs 

	fxch st1; st0 <-> st1
	fadd qword [b]
	fchs
	;;getting root 1
	fdiv qword [a]
	fdiv qword [two]
	fstp qword [root1]

	jz single;; check if 1 root

	;;getting root 2
	fdiv qword [a]
	fdiv qword [two]
	fst qword [root2]

	push dword [root1+4]
	push dword [root1]
	push dword [root2+4]
	push dword [root2]
	push resmsg
	call printf
	add esp, 20
	jmp end

single:
	push dword [root1+4]
	push dword [root1]
	push oneresmsg
	call printf
	add esp, 12
	jmp end


imaginary:
	push imagemsg
	call printf
	add esp, 4
	jmp end


end:

	mov esp, ebp; take down stack frame
	pop ebp

	mov eax,0; no error ret
	ret




