
start:

Code_selector	=  8
Stack_selector	= 16
Data_selector	= 24
Screen_selector	= 32
R_Mode_Code		= 40	; Селектор дескриптора сегмента кода для возврата
				;  в режим реальных адресов.
R_Mode_Data		= 48	; Селектор дескриптора сегментов стека и данных.
;------------------------------------------------------------------------

; Сохраняем сегментные регистры, используемые в R-Mode:

	mov	R_Mode_SS,ss
	mov	R_Mode_DS,ds
	mov	R_Mode_ES,es
	mov	R_Mode_FS,fs
	mov	R_Mode_GS,gs

; Подготавливаем адрес возврата в R-Mode:

	mov	R_Mode_segment,cs
	lea	ax,R_Mode_entry
	mov	R_Mode_offset,ax

; Подготовка к переходу в защищённый режим:

	mov	bx,offset GDT + 8
	xor	eax,eax
	mov	edx,eax

	push	cs
	pop	ax

	shl	eax,4
	mov	dx,1024
	mov	cl,10011000b
	call	set_descriptor		; Code

	lea	dx,Stack_seg_start
	add	eax,edx
	mov	dx,1024
	mov	cl,10010110b
	call	set_descriptor		; Stack

	xor	eax,eax
	mov	ax,ds
	shl	eax,4
	mov	dx,0ffffh
	mov	cl,10010010b
	call	set_descriptor		; Data

	mov	eax,0b8000h
	mov	edx,4000
	mov	cl,10010010b
	call	set_descriptor		; Screen

; Готовим дополнительные дескрипторы для возврата в R-Mode:

	xor	eax,eax

	push	cs
	pop	ax

	shl	eax,4		; EAX = физический адрес сегмента кода
				;  (и всех остальных сегментов, т.к.
				;  это .com-программа)

	mov	edx,0ffffh
	mov	cl,10011010b		; P=1, DPL=00b, S=1, Тип=101b, A=0
	call	set_descriptor		; R_Mode_Code

	mov	cl,10010010b		; P=1, DPL=00b, S=1, Тип=001b, A=0
	call	set_descriptor		; R_Mode_Data


; Устанавливаем GDTR:

	xor	eax,eax
	mov	edx,eax

	mov	ax,ds
	shl	eax,4
	lea	dx,GDT
	add	eax,edx
	mov	GDT_adr,eax

	mov	dx,55		; Предел GDT = 8 * (1 + 6) - 1
	mov	GDT_lim,dx

	cli

	lgdt	GDTR

	mov	R_Mode_SP,sp		; Указатель на стек сохраняем в последний
					;  момент.

; Переходим в защищённый режим:

	mov	eax,cr0
	or	al,1
	mov	cr0,eax


; Процессор в защищённом режиме

	db	0eah		; Команда far jmp Code_selector:P_Mode_entry.
	dw	P_Mode_entry
	dw	Code_selector

;------------------------------------------------------------------------
P_Mode_entry:

	mov	ax,Screen_selector
	mov	es,ax

	mov	ax,Data_selector
	mov	ds,ax

	mov	ax,Stack_selector
	mov	ss,ax
	mov	sp,0

; Сообщаем о входе в P-Mode (выводим ZS-строку):

	lea	bx,Start_P_Mode_ZS
	mov	di,480
	call	putzs

; Работа программы в защищённом режиме (здесь - только вывод строки):

	lea	bx,P_Mode_ZS
	add	di,160
	call	putzs

; Возвращаемся в режим реальных адресов.
; 1. Запретить прерывания (CLI).
;    Прерывания уже запрещены при входе в P-Mode.

; 2. Передать управление в читаемый сегмент кода, имеющий предел в 64Кб.

	db	0eah		; Команда far jmp R_Mode_Code:Pre_R_Mode_entry.
	dw	Pre_R_Mode_entry
	dw	R_Mode_Code


Pre_R_Mode_entry:
; 3. Загрузить в SS, DS, ES, FS и GS селекторы дескрипторов, имеющих
;    следующие параметры:
;  1) Предел = 64 Кб (FFFFh)
;  2) Байтная гранулярность (G = 0)
;  3) Расширяется вверх (E = 0)
;  4) Записываемый (W = 1)
;  5) Присутствующий (P = 1)
;  6) Базовый адрес = любое значение

	mov	ax,R_Mode_Data	; Селектор R_Mode_Data - "один на всех".
	mov	ss,ax
	mov	ds,ax
	mov	es,ax
	mov	fs,ax
	mov	gs,ax

; 4. Сбросить флаг PE в CR0.

	mov	eax,cr0
	and	al,0feh		; FEh = 1111'1110b
	mov	cr0,eax

; 5. Выполнить команду far jump на программу режима реальных адресов.

		db	0eah
R_Mode_offset	dw	?	; Значения R_Mode_offset и R_Mode_segment
R_Mode_segment	dw	?	;  сюда прописались перед входом в
				;  защищённый режим (в начале программы).
;------------------------------------------------------------------------
R_Mode_entry:
; 6. Загрузить в регистры SS, DS, ES, FS и GS необходимые значения или 0
;    (восстанавливаем сохранённые значения):

	mov	ss,R_Mode_SS
	mov	ds,R_Mode_DS
	mov	es,R_Mode_ES
	mov	fs,R_Mode_FS
	mov	gs,R_Mode_GS

	mov	sp,R_Mode_SP		; Восстанавливаем указатель стека
					;  непосредственно перед разрешением
					;  прерываний.

; 7. Разрешить прерывания (STI).

	sti

; Выводим ZS-строку "Back to real address mode..."

	lea	bx,R_Mode_ZS
	mov	ax,0b800h
	mov	es,ax
	mov	di,800
	call	putzs		; Функция putzs универсальна и работает
				;  в обоих режимах.

	int	20h		; Конец программы (здесь - выход в MS-DOS).


;------------------------------------------------------------------------
; Вставка макросами кода функций, определённых в текстовом файле
;  "pmode.lib":

init_set_descriptor
init_putzs
;------------------------------------------------------------------------
; ZS-строка для вывода при входе в P-Mode:
Start_P_Mode_ZS:	db	"Entering to protected mode...",0

; ZS-строка для вывода при работе в P-Mode:
P_Mode_ZS:	db	"Working in P-mode...",0

; ZS-строка для вывода в R-Mode:
R_Mode_ZS:	db	"Back to real address mode...",0

