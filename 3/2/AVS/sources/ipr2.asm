section .data
titlemsg: db "Lab2 Var6", 10, 0
resmsg: db "For X:%f in [%f,%f] with h: %f yx=%f sx=%f and sx-yx<e on k=%f",10, 0

amsg: db "a is:",10,0
bmsg: db "b is:",10,0
hmsg: db "h is:",10,0
emsg: db "e is:",10,0

one: dq 1.0
two: dq 2.0
four: dq 4.0
k: dq 0.0

ststr: db "status %x",10,0
fmtstr: db "debug %f",10,0
flfmt: db "%lf",0	

section .bss
a: resq 1
b: resq 1
h: resq 1
e: resq 1
x: resq 1
yx: resq 1
sx: resq 1

status: resd 1

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

	;;;;hreq
	push dword hmsg
	call printf
	add esp, 4; cleaning stack
	push h
	push flfmt; %lf (double)
	call scanf
	add esp, 8

	;;;;ereq
	push dword emsg
	call printf
	add esp, 4; cleaning stack
	push e
	push flfmt; %lf (double)
	call scanf
	add esp, 8

	finit;
	fld qword [a]
	fstp qword [x]

loopx:
	fninit

	fld qword [x]; st0 x
	fld1
	fpatan; st0 - atanx
	fdiv qword [two]
	
	fld qword [x]; 
	fadd qword [one]; st0 x+1 st1 1/2atanx

	fld qword [x];
	fchs ;st0 -x
	fadd qword [one]; st0 1-x; st1 1+x; st2 1/2atanx
	fdivp st1; st0 -> 1-x/1+x st1 1.2atanx

	fldln2; st0 ln(2) st1 .. 
	fxch st1; st0 <-> st1 
	fyl2x; st0 gets ln(2)*log2(1+x/1-x)
	fdiv qword [four]; st0 1/4 ln (1+x/1-x); st1 1/2atanx

	faddp; st0 has Y(x)

	fstp qword [yx]

	
	fldz;
	fst qword [k]
	fst qword [sx]
	
	;loop s(x) while e not larger than sub
	loopsx:
		fninit
		;find s(x)
		fld qword [k]
		fmul qword [four]
		fadd qword [one]
		fld st0
		fld qword [x]; st0 x st1 4k+1
		fyl2x; (4k+1)log2(x) 
		
		fld st0; ylg2x ylg2x st2 4k+1
		frndint ;st0 int[ylg2x]
		fsub st1,st0; int[ylg2x]; fractport; 4k+1
		fxch st1; st0 <->st1
		f2xm1; 2^st0(aka fractport)-1; int[ylg2x]; 4k+1
		fadd qword [one]
		fscale; x^4k+1; 
		fstp st1; st0->st1 + pop
		fdivrp; st0 = 1sx

		;summ up with prev result
		fadd qword [sx]
		fst qword [sx]

		fld qword [yx]; st0 yx st1 sx

		fsubp st1; st0 has sub
		fabs
		fld qword [e]; epsilon

		fcomp; compare and pop
		fstsw [status]
		fwait
		mov ah, byte [status+1]
		sahf; store compare result in flags; 
		
		jnc endloopsx; !e<=[(yx-sx)];; 
		
		;;;;inc K
		fninit; clear stack
		fld1
		fld qword [k]
		fadd st1
		fst qword [k]
		jmp loopsx;

	endloopsx:
	;;;print routine
	push dword [k+4]
	push dword [k]
	push dword [sx+4]
	push dword [sx]
	push dword [yx+4]
	push dword [yx]
	push dword [h+4]
	push dword [h]
	push dword [b+4]
	push dword [b]
	push dword [a+4]
	push dword [a]
	push dword [x+4]
	push dword [x]
	push dword resmsg
	call printf
	add esp, 52; cleaning stack

	fninit
	;;;;inc and test X
	fld qword [h]
	fld qword [x]
	fadd
	fst qword [x]; x+h
	fld qword [b]; b; x+h
	fcomp
	fstsw [status]
	fwait
	mov ah, byte [status+1]
	sahf
	jc end; b<=x

jmp loopx


end:
	mov esp, ebp; take down stack frame
	pop ebp

	mov eax,0; no error ret
	ret

debug:
		fstp qword [sx]
		push dword [sx+4]
		push dword [sx]
		push dword fmtstr
		call printf
		add esp, 12
		fstp qword [sx]
		push dword [sx+4]
		push dword [sx]
		push dword fmtstr
		call printf
		add esp, 12
		jmp end
		ret



