%include 'system.inc'

ipr1
%define BUFSIZE 2048

section .data
    four dd 4.0
    two dd 2.0
    titlemsg:    db      "a(x*x) + b(x) + c solver", 10, "reading a", 10
    .len:   equ     $ - titlemsg


section .bss
    a: resd 1
    b: resd 1
    c: resd 1
    status: resd 1
    root1: resd 1
    root2: resd 1
    ibuffer resb BUFSIZE
    obuffer resb BUFSIZE

section .text
    global start

start:
    push    dword titlemsg.len
    push    dword titlemsg
    push    dword stdout
    sys.write

    ;read coeff A

    ;call solve;

    ;print results
    push dword 0
    sys.exit

solve:
    finit
    sys.exit


;fild b; st0 = b
; FMUL ST0 ;перемножить числа ST(0):=B^2
; FILD A ;загрузить число в регистр стека, ST(0):=A
; FMUL FOUR ;умножить число в ST(0) на 4.0, ST(0):=4*A
; FIMUL C ;умножить число в ST(0) на C, ST(0):=4*A*C
; FSUBRP ST1,ST0 ;вычитаем обратные целые числа
; FTST ;проверяем дискриминант на отрицательность
; FSTSW STATUS;записываем в память управляющий регистр FPU
; FWAIT; и переводим FPU в режим ожидания
; MOV AH ,BYTE [STATUS+1];Читаем в AH
; SAHF; помещаем содержимое регистра AH в регистр флагов
; JB IMAGINARY;если корни мнимые переходим
; FSQRT ;извлекаем квадратный корень в ST(0):=SQRT(B^2-4*A*C)
; FLD ST0 ;загрузить число из ST(0) в ST(1),   ST(1):=ST(0)
; FCHS ;изменяем знак, ST(0):=-SQRT(D)
; FIADD B;складываем целые числа ST(0):=B-SQRT(D)
; FCHS ;изменяем знак ST(0):= -B+SQRT(D)
; FXCH ST1;обмениваемся данными между ST(0) и ST(1)
; FIADD B;складываем целые числа ST(0):=B+SQRT(D)
; FCHS ;изменяем знак, ST(0):= -B-SQRT(D)
; FIDIV A;делим на целое число ST(0):= -(B+SQRT(D))/A
; FDIV TWO;еще раз делим на 2, ST(0):=-(B+SQRT(D))/2A
; FST ROOT1;сохраняем в памяти 1-ый результат из ST(0)
; FIDIV A;делим на целое число ST(0):=(-B+SQRT(D))/A
; FDIV TWO ;делим на 2 ST(0):=(-B+SQRT(D))/2A
; FST ROOT2 ;сохраняем в памяти 2-ой результат
; JMP END

imaginary:

    sys.exit



