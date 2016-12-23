    
; Using Flat Assembler
; Copyright (C) Chourdakis Michael. All Rights Reserved.

FORMAT MZ 
ENTRY CODE16:Start16
STACK STACK16:sseg16


;
; -------------------------------------------------------------------
;
; Structure definitions
; 
; -------------------------------------------------------------------
;
struc GDT_STR s0_15,b0_15,b16_23,flags,access,b24_31
	{
		.s0_15   dw s0_15
		.b0_15   dw b0_15
		.b16_23  db b16_23
		.flags   db flags
		.access  db access
		.b24_31  db b24_31
	}

	
struc IDT_STR o0_15,se0_15,zb,flags,o16_31
	{
		.o0_15   dw o0_15
		.se0_15  dw se0_15
		.zb      db zb
		.flags   db flags
		.o16_31  dw o16_31
	}
	

PAGEORGBASE EQU 40000h
	
;
; -------------------------------------------------------------------
;
; 64-bit segments
;

; -------------------------------------------------------------------
;
; 64-bit page  segment
SEGMENT PAGE64 USE64

ORG PAGEORGBASE

; 4096 bytes for PML4
PML4_64 db 4096 dup (?)

; 4096 bytes of PDP_64
PDP_64 db 4096 dup (?)

; 4096 bytes of PD_64
PD_64 db 4096 dup (?)

; 
PAGE_64 dq 4096 dup (?)
; 32768 Bytes = 4096 Pages = 4096*4096 = 16MB (Each page takes 8 bytes)



; -------------------------------------------------------------------
;
; 64-bit data segment
 SEGMENT DATA64 USE64

; 64-bit data segment
 SEGMENT ABSD64 USE64

;
;
; -------------------------------------------------------------------
;
; 64-bit stack  segment
;SEGMENT STACK64 USE64
;
; -------------------------------------------------------------------
;
; 64-bit code segment
SEGMENT CODE64 USE64

stack64         db      1000 dup (?)    ; 64-bit stack
stack64_end:


 Print64:
  ; int 9

  push rax
  push rdi
  ;push ds
  ;push es

  mov ax,absd64_idx
  mov ds,ax
  mov es,ax

  mov rax,8787878787878787h
  mov rdi,0b8000h
  mov eax,8787h
  ;mov edi,0b8030h
  rep stosq
  ;mov [edi],eax
  ;mov [edi+4],eax

  ;mov ax,data16_idx
  ;mov ds,ax
  ;xor edi,edi
  ;mov di,DATA16
  ;shl edi,4
  ;add di,MessageFrom64
  ;mov dword [edi],0x111

  ;pop es
  ;pop ds
  pop rdi
  pop rax
  ret


Start64:

 xor r8d,r8d
 mov rsp,stack64_end  
 ; int 3
 push rax
 mov rax,0
 pop rax

 mov rcx,125
 TestLabel:
 call Print64

 ; Message
 xor rdi,rdi
 mov di,DATA16
 shl edi,4
 add di,MessageFrom64
 mov rcx,0x2222222211111111
 mov [rdi],rcx


 ;mov ax,data16_idx
 ;mov ds,ax
 ;xor rdi,rdi
 ;mov di,MessageFrom64
 ;mov dword [rdi]

; Back to Compatibility Mode

; sth bad
; mov rdi,0x1234567896563
; mov byte [rdi],0
 

; xor rax,rax
; int 9

xor rdx,rdx
mov rdx,0xeeeeeeee00000000
shr rdx,32
mov rax,rdx

push code32_idx
xor rcx,rcx
mov ecx,Back32
push rcx
retf

db 066h
db 0eah 
dd Back32
dw code32_idx
;dw 0



	
	
;
; -------------------------------------------------------------------
;
; 32-bit segments
;

; -------------------------------------------------------------------
;
; 32-bit page  segment
SEGMENT PAGE32 USE32

ORG PAGEORGBASE 



PageDir32 dd 1024 DUP (?)
  
; We'll map the first 16 MB of physical memory into the first 16 MB of physical
; memory

PageTables32 dd 4096 DUP (?)
; 16384 Bytes = 4096 Pages = 4096*4096 = 16MB (Each page takes 4 bytes)



; -------------------------------------------------------------------
;
; 32-bit data segment
SEGMENT DATA32 USE32

ldt_start:
ldt_1_descriptor  GDT_STR 0ffffh,0,0,92h,0cfh,0 ; 4GB 32-bit data
ldt_size=$-(ldt_1_descriptor)
; unlike the GDT , LDT does not have a 6-byte header to indicate its absolute address and limit ; these are specified into its GDT entry.
	
tssdata db 2048 dup (0) ; for some empty TSS
	
d32 dd 0
		
;
; -------------------------------------------------------------------
;
; 32-bit code segment
SEGMENT CODE32 USE32

ORG 0


Start32:

	mov     ax,stack32_idx          
	mov     ss,ax                   
	mov     esp,stack32_end  
	mov     ax,data32_idx           
	mov     ds,ax
	mov     es,ax
	mov     ax,data16_idx
	mov     gs,ax
	mov     fs,ax

  ; = END NO DEBUG HERE ; woooa!

   ; just load TR with a 32-bit segment
   ; We won't use it, but long mode will complain if TR still points to a 286 TSS
	mov ax,tssd32_idx
	ltr ax
  
  cmp [fs:IsSwat],1
  jne NoBrk;
  
  int 3;
  NoBrk:
  
  nop
  nop
  nop


  
  ; Test Data stuff
  mov eax,1
  mov [ds:d32],eax
  mov ebx,[ds:d32]
  
  ; Test LLDT stuff
  mov ax,ldt_idx
  lldt ax
  mov ax,data32_ldt_idx
  mov gs,ax

  mov ecx,1
  call Print32
  
 ; Initialize the Page Table
  call InitPageTable32

  ; Load Page Directory Offset
  mov edx,PageDir32
  mov CR3,edx;;;

  ; Enable Paging
  mov eax, cr0
  or eax,80000000h
  mov cr0, eax

  mov ecx,2
  call Print32

  nop


  ; Disable Paging
  mov eax, cr0 ; Read CR0.
  and eax,7FFFFFFFh; Set PE=0
  mov cr0, eax ; Write CR0.

  
  ; Test LONG mode x64
  cmp [fs:LongMode],1
  jnz NoLong

  ; Initialize 64-bit Pages
  ; It is already initialized from real mode
  ; call InitPageTable64
  call InitPageTable642

  mov ecx,3
  call Print32

  ; Enable PAE
    mov eax, cr4
    bts eax, 5
    mov cr4, eax
    
  mov ecx,4
  call Print32

  ; Load new page table
   mov edx,PAGEORGBASE
   mov cr3,edx
    
  mov ecx,5
  call Print32

  ; Enable Long Mode
    mov ecx, 0c0000080h ; EFER MSR number. 
    rdmsr ; Read EFER.
    bts eax, 8 ; Set LME=1.
    wrmsr ; Write EFER.
  
  mov ecx,6
  call Print32

  ; Enable Paging to activate Long Mode
    mov eax, cr0 ; Read CR0.
    or eax,80000000h ; Set PE=1.
    mov cr0, eax ; Write CR0.
	
  mov ecx,7
  call Print32

  ;
   ; We are now in Long Mode / Compatibility mode
  
    ; Jump to an 64-bit segment to enable 64-bit mode
	
	;db 066h ; because we are in a 32bit segment
	;db 0eah
	;dd Start64
	;dw code64_idx

	;db 066h
	db 0eah
	PutLinearStart64 dd 0
	dw code64_idx

	
	
  
  
Back32:
BackFromLong:  
  
	cmp eax,0xeeeeeeee
	jnz Not64
	; int 3 it works!
	Not64:
  
	; We are now in Compatibility mode again
	mov     ax,stack32_idx          
	mov     ss,ax                   
	mov     esp,stack32_end  
	mov     ax,data32_idx           
	mov     ds,ax
	mov     es,ax
	mov     ax,data16_idx
	mov     gs,ax
	mov     fs,ax

	
  
  mov ecx,8
  call Print32
   
   ;   Disable Paging to get out of Long Mode
	mov eax, cr0 ; Read CR0.
	and eax,7fffffffh ; Set PE=0.
	mov cr0, eax ; Write CR0.
  
  mov ecx,9
  call Print32
  
  ; Deactivate Long Mode
  mov ecx, 0c0000080h ; EFER MSR number. 
  rdmsr ; Read EFER.
  btc eax, 8 ; Set LME=0.
  wrmsr ; Write EFER.
 
    
  NoLong:
  
  mov ecx,10
  call Print32
  
  ; = Give FS the abs32 segment
  ; To test unreal mode 
  mov ax,absd32_idx
  mov fs,ax
  
  ; = Jump back to a CODE16 segment
  db 066h ; because we are in a 32bit segment
  db 0eah
  dw exit16
  dw code16_idx


intr00:
  ; do nothing but return
	IRETD

Print32:
 pushad
 push ds
 push es
 mov ax,absd32_idx
 mov ds,ax
 mov es,ax

 mov eax,06660721h
 mov edi,0b8000h
 ;mov ecx,80*2
 rep stosd

 pop es
 pop ds
 popad
 ret



InitPageTable32:
 pushad
 push ds
 push es
 mov ax,page32_idx
 mov ds,ax
 mov es,ax

; Fill "See Through" Page Tables for the first 16MB
 mov edi, PageTables32
 mov eax, 16*1024*1024 + 7-4096 ; 3 bits set + offset
 mov ecx, 1024*4; 4096 entries = 16MB
fill1MB32:
 mov [edi+ecx*4-4],eax
 sub eax,4096
 loop fill1MB32

; Create the Page Directory
; Clear everytihng
 mov edi, PageDir32
 mov ecx, 1024
 mov eax, 0fffffffeh
 rep stosd

mov eax, PageTables32
or al,7 ; Add in the page table mode bits
mov edi, PageDir32
mov [edi],eax
add eax,4096
mov [edi+4],eax
add eax,4096
mov [edi+8],eax
add eax,4096
mov [edi+12],eax

pop es
pop ds
popad
ret



	

InitPageTable64:
 pushad
 push ds
 push es
 mov ax,page64_idx
 mov ds,ax
 mov es,ax

 xor bx,bx
 mov edi,PML4_64
 mov ax,0b00fh
 stosw

 xor ax,ax
 mov cx,07ffh
 rep stosw

 mov ax,0c00fh
 stosw

 xor ax,ax
 mov cx,07ffh
 rep stosw

 mov ax,018fh
 stosw

 xor ax,ax
 mov cx,07ffh
 rep stosw
 
 pop es
 pop ds
 popad
 ret


InitPageTable642:
	pushad
	push ds
	push es
	mov ax,page64_idx
	mov ds,ax
	mov es,ax
	xor     eax, eax
	mov     edi,PAGEORGBASE
	mov     ecx,03000h
	rep     stosb

	;top level page table
	mov     eax, PAGEORGBASE + 0x1000
	or              eax,3
	mov     [PAGEORGBASE],eax
	mov     eax, PAGEORGBASE + 0x2000
	or              eax,3
	mov     [PAGEORGBASE + 0x1000],eax

	;2MB pages to identity map the first 16MB ram
	mov     eax,1
	shl             eax,7
	or              eax,3
	mov     [PAGEORGBASE + 0x2000],eax
	add     eax,0x200000
	mov     [PAGEORGBASE + 0x2008],eax
	add     eax,0x200000
	mov     [PAGEORGBASE + 0x2010],eax
	add     eax,0x200000
	mov     [PAGEORGBASE + 0x2018],eax
	add     eax,0x200000
	mov     [PAGEORGBASE + 0x2020],eax
	add     eax,0x200000
	mov     [PAGEORGBASE + 0x2028],eax
	add     eax,0x200000
	mov     [PAGEORGBASE + 0x2030],eax
	add     eax,0x200000
	mov     [PAGEORGBASE + 0x2038],eax

	pop es
	pop ds
	popad 
	ret





;
; -------------------------------------------------------------------
;
; 32-bit stack  segment
SEGMENT STACK32 USE32

stack32         db      100 dup (?)    ; 32-bit stack
stack32_end:




	
;
; -------------------------------------------------------------------
;
; 16-bit segments
;
; -------------------------------------------------------------------- --; 16-bit data segment
SEGMENT DATA16 USE16


; GDT definitions
gdt_start dw gdt_size
gdt_ptr dd 0
dummy_descriptor GDT_STR 0,0,0,0,0,0
code32_descriptor  GDT_STR 0ffffh,0,0,9ah,0cfh,0 ; 4GB 32-bit code , 9ah = 10011010b = Present, DPL 00,No System, Code Exec/Read. 0cfh access = 11001111b = Big,32bit,<resvd 0>,1111 more size
data32_descriptor  GDT_STR 0ffffh,0,0,92h,0cfh,0 ; 4GB 32-bit data,   92h = 10010010b = Presetn , DPL 00, No System, Data Read/Write
stack32_descriptor GDT_STR 0ffffh,0,0,92h,0cfh,0 ; 4GB 32-bit stack
code16_descriptor  GDT_STR 0ffffh,0,0,9ah,0,0    ; 64k 16-bit code
data16_descriptor  GDT_STR 0ffffh,0,0,92h,0,0    ; 64k 16-bit data
stack16_descriptor GDT_STR 0ffffh,0,0,92h,0,0    ; 64k 16-bit data
ldt_descriptor     GDT_STR ldt_size,0,0,82h,0,0  ; pointer to LDT,  82h = 10000010b = Present, DPL 00, System , Type "0010b" = LDT entry
code64_descriptor  GDT_STR 0ffffh,0,0,9ah,0afh,0 ; 16TB 64-bit code, 08cfh access = 01001111b = Big,64bit (0), 1111 more size
page32_descriptor  GDT_STR 0ffffh,0,0,92h,0cfh,0 ; 4GB 32-bit data,   92h = 10010010b = Presetn , DPL 00, No System, Data Read/Write
page64_descriptor  GDT_STR 0ffffh,0,0,92h,0cfh,0 ; 4GB 32-bit data,   92h = 10010010b = Presetn , DPL 00, No System, Data Read/Write
absd32_descriptor  GDT_STR 0ffffh,0,0,92h,0cfh,0 ; 4GB 32-bit data,   92h = 10010010b = Presetn , DPL 00, No System, Data Read/Write
data64_descriptor  GDT_STR 0ffffh,0,0,92h,0afh,0 ; 16TB 64-bit data, 08cfh access = 10001111b = Big,64bit (0), 1111 more size
absd64_descriptor  GDT_STR 0ffffh,0,0,92h,0afh,0 ; 16TB 64-bit data, 08cfh access = 10001111b = Big,64bit (0), 1111 more size
tssd32_descriptor  GDT_STR 0h,0,0,89h,040h,0 ; TSS segment in GDT
;swat_descriptor GDT_STR 0,0,0,0,0,0;50 dup(0)                ; Reserved entries for SWAT
swat_descriptor GDT_STR 0,0,0,0,0,0 
	rb 50 * 8
gdt_size = $-(dummy_descriptor)


dummy_idx       = 0h    ; dummy selector
code32_idx      =       08h             ; offset of 32-bit code  segment in GDT
data32_idx      =       10h             ; offset of 32-bit data  segment in GDT
stack32_idx     =       18h             ; offset of 32-bit stack segment in GDT
code16_idx      =       20h             ; offset of 16-bit code segment in GDT
data16_idx      =       28h             ; offset of 16-bit data segment in GDT
stack16_idx     =       30h             ; offset of 16-bit stack segment in GDT
ldt_idx         =       38h             ; offset of LDT in GDT
code64_idx      =       40h             ; offset of 64-bit code segment in GDT
page32_idx      =       48h             ; offset of 32-bit data segment in GDT
page64_idx      =       50h             ; offset of 64-bit data segment in GDT
absd32_idx      =       58h             ; offset of 32-bit data  segment in GDT
data64_idx      =       60h             ; offset of 64-bit data segment in GDT
absd64_idx      =       68h             ; offset of 64-bit data segment in GDT
tssd32_idx      =       70h             ; TSS descriptor
swat_idx        =       78h             ; SWAT debugger selectors

data32_ldt_idx  =       04h             ; offset of 32-bit data  segment in LDT


idt_RM_start      dw 0
idt_RM_ptr dd 0

; ---- IDT definition
; We just specifiy an INT 00h handler

idt_PM_start      dw             idt_size
idt_PM_ptr dd 0
interruptsall rb 256*8
;interruptsall IDT_STR 0,0,0,0,0
; rb 256 * 8
idt_size=$-(interruptsall)


; Information Messages
SMsg db "3x shut!$"
AskSwatMsg db "Initialize SWAT?",0dh,0ah,"$"
AskLongMsg db "Enter Long Mode?",0dh,0ah,"$"
OkSwatMsg db "Initialize SWAT OK.",0dh,0ah,"$"
LongModeSupport db "Long Mode x64 Supported.",0dh,0ah,"$"
NoLongModeSupport db "Long Mode x64 not supported.",0dh,0ah,"$"
EndTest db 0dh,0ah,"All Tests Finished!",0dh,0ah,"$"
MessageReceived db 0dh,0ah,"64-bit Message Received!",0dh,0ah,"$"
UnrealWorks db 0dh,0ah,"Unreal mode works!",0dh,0ah,"$"

IsSwat db 0
LongMode db 0
PhysicalPagingOffset32 dd 0
PhysicalPagingOffset64 dd 0

MessageFrom64 dd 0
MessageFrom64_2 dd 0


;
; -------------------------------------------------------------------
;
; 16-bit Stack Segment
SEGMENT STACK16 USE16
sseg16 dw 1024 dup (?)

stack16_end:


;
; -------------------------------------------------------------------
;
; 16-bit code segment. 
SEGMENT CODE16 USE16
ORG 0h

Start16: 


; Initialization of our segments
 CLI
  mov ax,DATA16
  mov ds,ax
  mov es,ax
  mov ax,STACK16
  mov ss,ax
  STI

; Find Linear Address for 64-bit startup
; Because there is no segmentation 
  xor eax,eax
  mov ax,CODE64
  mov ebx,TestLabel
  
  shl eax,4
  add eax,Start64
  push fs
  mov bx,CODE32
  mov fs,bx
  mov dword [fs:PutLinearStart64],eax
  pop fs


; Clear the screen  

 call EnableA20
 ;call InitPageTable6416
 ;call InitPageTable64162
 ;call InitPageTable64163


 ; Test Long Mode Support
  mov eax, 80000000h ; Extended-function 8000000h.
  cpuid ; Is largest extended function
  cmp eax, 80000000h ; any function > 80000000h?
  jbe no_long_mode ; If not, no long mode.
  mov eax, 80000001h ; Extended-function 8000001h.
  cpuid ; Now EDX = extended-features flags.
  bt edx, 29 ; Test if long mode is supported.
  jnc no_long_mode ; Exit if not supported.  

  mov [ds:LongMode],1
  mov ax,0900h;
  lea dx,[LongModeSupport]
  int 21h
  jmp AfterLongTest
 no_long_mode:
  mov ax,0900h;
  lea dx,[NoLongModeSupport]
  int 21h
 AfterLongTest:

  ; = PM PREPARE =
  CALL TRFON

;  = GDT =
  CALL GDTInit
  
; = SAVE CURRENT IDT =
  mov bx,idt_RM_start
  sidt [bx]

; = IDT
  CALL IDTInit

; = Long
  cmp [ds:LongMode],0
  je OkLong
  mov ax,0900h
  mov dx,AskLongMsg
  int 21h
  mov ax,0100h
  int 21h
  cmp al,79h
  je OkLong
  mov [ds:LongMode],0
  OkLong:
 
; = Swat
  mov ax,0900h
  mov dx,AskSwatMsg
  int 21h
  mov ax,0100h
  int 21h
  cmp al,79h
  jnz NoSwat
  
  Call Swat
  NoSwat:
  
 
; = SWITCH =
  cli
  mov bx,gdt_start
  lgdt [bx]
  mov bx,idt_PM_start
; = NO DEBUG HERE =
  lidt [bx]
  mov eax,cr0
  or al,1
  mov cr0,eax 
  
 db  066h    ; We will jump to a USE32 segment
 db  0eah           ; JMP FAR
 dd  Start32
 dw  code32_idx

exit16:                 
	mov     eax,cr0         
	and     al,not 1        
  ; = NO DEBUG = (at least, never made the debugger to step this :)
	mov     cr0,eax         
	db      0eah
	dw      flush_ipq,CODE16
flush_ipq:
	mov     ax,STACK16               ; restore important registers
	mov     ss,ax
	mov     sp,stack16_end
	mov ax, DATA16
	mov     ds,ax
	mov     es,ax
	mov     di,idt_RM_start
	lidt    [di]
	sti                     ; enable interrupts
; = END NO DEBUG HERE =
  
  CALL TRFOFF

  ; FS has an unreal segment!
  mov ax,0
  mov fs,ax
  mov edi,20000h
  mov byte [fs:edi], 0xFE
  xor ax,ax
  mov al, [fs:edi]
  cmp al,0xFE
  jnz NoUnreal

  mov ax,0900h;
  mov dx,UnrealWorks
  int 21h
  
  NoUnreal:

; = GAME OVER =

  ; int 3
  mov ebx,[ds:MessageFrom64]
  cmp ebx,0x11111111
  jnz NoMessage
  mov ebx,[ds:MessageFrom64 + 4]
  cmp ebx,0x22222222
  jnz NoMessage


  mov ax,0900h;
  mov dx,MessageReceived
  int 21h

NoMessage:
  
  mov ax,DATA16
  mov ds,ax
  mov ax,0900h;
  mov dx,EndTest
  int 21h


mov ax,4c00h
int 21h;


InitPageTable64163:
	pushad
	push ds
	push es
	mov ax,4000h
	mov ds,ax
	mov es,ax

	xor     eax, eax
	mov     edi,0
	mov     ecx,03000h
	rep     stosb

	;top level page table
	mov     eax, 0 + 0x1000
	or              eax,3
	mov     [0],eax
	mov     eax, 0 + 0x2000
	or              eax,3
	mov     [0 + 0x1000],eax

	;2MB pages to identity map the first 16MB ram
	mov     eax,1
	shl             eax,7
	or              eax,3
	mov     [0 + 0x2000],eax
	add     eax,0x200000
	mov     [0 + 0x2008],eax
	add     eax,0x200000
	mov     [0 + 0x2010],eax
	add     eax,0x200000
	mov     [0 + 0x2018],eax
	add     eax,0x200000
	mov     [0 + 0x2020],eax
	add     eax,0x200000
	mov     [0 + 0x2028],eax
	add     eax,0x200000
	mov     [0 + 0x2030],eax
	add     eax,0x200000
	mov     [0 + 0x2038],eax

	pop es
	pop ds
	popad 
	ret


InitPageTable64162:
	; PageDirectory = PAGEORGBASE
	pushad
	push ds
	mov eax,PML4_64
	shr eax,4
	mov ds,ax
	
	xor edx,edx
	; DS:DX = PML4_64
	
	mov ecx,PDP_64
	sub ecx,PAGEORGBASE
	; DS:CX = PDP_64
	; Put PDP to PML4
	xor ebx,ebx
	mov bx,cx
	add ebx,PAGEORGBASE
	or bl,111b ; Mark it as read/write/user
	mov [ds:edx],ebx
	
	mov ebp,PD_64
	sub ebp,PAGEORGBASE
	; DS:BP = PD_64
	; Put PD to PDP
	xor ebx,ebx
	mov bx,bp
	add ebx,PAGEORGBASE
	or bl,111b ; Mark it as read/write/user
	mov [ds:ecx],ebx
	
	; 16MB map
	mov ecx,16777216
	shr ecx,12 ; Make it number of pages
	
	; Build 4096 Pages to map 16MB of memory
	; Each page takes 8 bytes

	
	mov edi,PAGE_64
	sub edi,PAGEORGBASE

	;       
	; DS:BP = PD_64
	; DS:DI = PAGE_64
	;
	xor esi,esi
	
	xor ebx,ebx
	mov bx,di
	add ebx,PAGEORGBASE
	
NextPageTable:

	; DS:DI holds the address of next page in memory
	; EBX   holds the absolute address of next page in memory
	; DS:BP holds the address of the page directory entry for this page
	
	; Put this page to page directory
	PUSH EBX
	or bl,111b ; Mark it as read/write/user
	mov [ds:ebp],ebx
	POP EBX
	ADD EBX,8

	ADD EBP,8
	
NextPage:
	lea eax,[esi + 0111b]
	; Configure this page to map memory
	mov [ds:edi],eax ; Configure This Page
	add edi,8                ; To next page, 8 bytes each
	add esi,1000h    ; Next 4096 bytes
	
	; Are we finished doing our pages ?
	dec ecx
	jecxz DoneMapping

	jmp NextPageTable
	
DoneMapping:
 
	pop ds
	popad
ret

InitPageTable6416:
 pushad
 push ds
 push es
 mov ax,08000h
 mov ds,ax
 mov es,ax

 xor bx,bx
 mov edi,0
 mov ax,0b00fh
 stosw

 xor ax,ax
 mov cx,07ffh
 rep stosw

 mov ax,0c00fh
 stosw

 xor ax,ax
 mov cx,07ffh
 rep stosw

 mov ax,018fh
 stosw

 xor ax,ax
 mov cx,07ffh
 rep stosw
 
 pop es
 pop ds
 popad
 ret




; Print16 function
Print16:
 pusha
 push ds
 push es
 mov ax,0b800h
 mov ds,ax
 mov es,ax

 mov ax,0352h
 mov di,0
 mov cx,80*10
 rep stosw

 pop es
 pop ds
 popa
 ret

WaitKBC:
mov cx,0ffffh
A20L:
in al,64h
test al,2
loopnz A20L
ret


EnableA20:
call WaitKBC
mov al,0d1h
out 64h,al
call WaitKBC
mov al,0dfh
out 60h,al
ret




; 3x fault handler
ShutdownProc:
CLI
MOV AX,DATA16
MOV DS,AX
MOV AX,STACK16
MOV SS,AX
MOV SP,stack16_end
MOV DX,SMsg
MOV AX,0900h
INT 21h
MOV AX,4C00h
INT 21h

; Preparation functions
TRFON:
  PUSHAD
  MOV AX,040h
  MOV ES,AX
  MOV DI,067h
  MOV AL,8Fh
  OUT 70h,AL
  MOV ax,ShutdownProc
  STOSW
  MOV ax,CODE16
  STOSW
  MOV al,0ah
  OUT 71h,al
  MOV al,8dh
  OUT 70h,al
  POPAD
  RET


TRFOFF:
  PUSHAD
  MOV al,8Fh
  OUT 70h,al
  MOV al,0
  JMP SHORT $ + 2
  JMP SHORT $ + 2
  OUT 71h,al
  MOV al,8dh
  OUT 70h,al
  POPAD
  RET


Swat:
pusha 
MOV AX,0def0h
int 67h

cmp ah,0
jnz errex

; Specify GDT entries
mov ax,DATA16
mov es,ax
mov di,swat_descriptor
mov bx,swat_idx
mov ax,0def2h
int 67h
cmp ah,00h
jnz errex

; Specify IDT entries
mov cx,01Fh
LoopX:

mov ax,0def3h
mov bx,cx
mov bp,cx
mov dx,8
mov ax,8
mul bp
mov bp,ax
mov ax,0def3h
mov di,interruptsall
add di,bp
int 67h

jcxz EndL

dec cx
jmp LoopX

EndL:
mov [IsSwat],1
mov ax,0900h
mov dx,OkSwatMsg
int 21h


errex:
popa

GDTInit:
  xor eax,eax
	mov     ax,CODE32
	shl     eax,4           ; make a physical address
	mov     [ds:code32_descriptor.b0_15],ax ; store it in the dscr
	shr     eax,8
	mov     [ds:code32_descriptor.b16_23],ah

  xor eax,eax
	mov     ax,DATA32       ; get 32-bit data segment into AX
	shl     eax,4           ; make a physical address
	mov     [ds:data32_descriptor.b0_15],ax ; store it in the dscr
	shr     eax,8
	mov     [ds:data32_descriptor.b16_23],ah

  xor eax,eax
	mov     ax,DATA32       ; get 32-bit data segment into AX
	shl     eax,4           ; make a physical address
	add     eax,tssdata
	mov     [ds:tssd32_descriptor.b0_15],ax ; store it in the dscr
	shr     eax,8
	mov     [ds:tssd32_descriptor.b16_23],ah
	mov     [ds:tssd32_descriptor.s0_15],1024
	

  xor eax,eax
	mov     ax,0       ; get 32-bit page segment into AX  = NOT PAGE32 = because it is assumed to be at 0!
	shl     eax,4           ; make a physical address
	xor     ebx,ebx
	mov     ebx,PageDir32
    add     ebx,eax
	mov     [ds:PhysicalPagingOffset32],ebx
	mov     [ds:page32_descriptor.b0_15],ax ; store it in the dscr
	shr     eax,8
	mov     [ds:page32_descriptor.b16_23],ah

  xor eax,eax
	mov     ax,STACK32 ; get 32-bit stack segment into AX
	shl     eax,4           ; make a physical address
	mov     [ds:stack32_descriptor.b0_15],ax ; store it in the dscr
	shr     eax,8
	mov     [ds:stack32_descriptor.b16_23],ah

  xor eax,eax
	mov     ax,CODE16 ; get 16-bit code segment into AX
	shl     eax,4           ; make a physical address
	mov     [ds:code16_descriptor.b0_15],ax ; store it in the dscr
	shr     eax,8
	mov     [ds:code16_descriptor.b16_23],ah

  xor eax,eax
	mov     ax,DATA16 ; get 16-bit code segment into AX
	shl     eax,4           ; make a physical address
	mov     [ds:data16_descriptor.b0_15],ax ; store it in the dscr
	shr     eax,8
	mov     [ds:data16_descriptor.b16_23],ah

  xor eax,eax
	mov     ax,STACK16 ; get 16-bit stack segment into AX
	shl     eax,4           ; make a physical address
	mov     [ds:stack16_descriptor.b0_15],ax ; store it in the dscr
	shr     eax,8
	mov     [ds:stack16_descriptor.b16_23],ah
	
  ; Create LDT entry
  xor eax,eax
	mov     ax,DATA32 ; get 32-bit data segment into AX, our LDT pointer resides into data32
	shl     eax,4           ; make a physical address
	add     eax,ldt_start
	mov     [ds:ldt_descriptor.b0_15],ax ; store it in the dscr
	shr     eax,8
	mov     [ds:ldt_descriptor.b16_23],ah
 
  nop
  xor eax,eax
	mov     ax,CODE64  ; get 64-bit code segment into AX (?0 because its linear?)
	xor ax,ax ; 64-bit are always 0 based!
	shl     eax,4           ; make a physical address
	mov     [ds:code64_descriptor.b0_15],ax ; store it in the dscr
	shr     eax,8
	mov     [ds:code64_descriptor.b16_23],ah

  xor eax,eax
       mov     ax,DATA64 ; get 64-bit data segment into AX
       xor ax,ax ; 64-bit are always 0 based!
       shl     eax,4           ; make a physical address
       mov     [ds:data64_descriptor.b0_15],ax ; store it in the dscr
       shr     eax,8
       mov     [ds:data64_descriptor.b16_23],ah

  
  xor eax,eax
	mov     ax,ABSD64 ; get 64-bit data segment into AX
	xor ax,ax ; 64-bit are always 0 based!
	shl     eax,4           ; make a physical address
	mov     [ds:absd64_descriptor.b0_15],ax ; store it in the dscr
	shr     eax,8
	mov     [ds:absd64_descriptor.b16_23],ah

  xor eax,eax
	mov     ax,0       ; get 32-bit page segment into AX  = NOT PAGE64 = because it is assumed to be at 0!
	shl     eax,4           ; make a physical address
	xor     ebx,ebx
	mov     ebx,PAGEORGBASE
    add     ebx,eax
	mov     [ds:PhysicalPagingOffset64],ebx
	mov     [ds:page64_descriptor.b0_15],ax ; store it in the dscr
	shr     eax,8
	mov     [ds:page64_descriptor.b16_23],ah

  xor eax,eax
	mov     ax,0
	shl     eax,4           ; make a physical address
	mov     [ds:absd32_descriptor.b0_15],ax ; store it in the dscr
	shr     eax,8
	mov     [ds:absd32_descriptor.b16_23],ah

	
  ; Set gdt ptr
  xor eax,eax
	mov     ax,DATA16       
	shl     eax,4
	add     ax,dummy_descriptor
	mov     [gdt_ptr],eax

  RET

  
IDTInit:

  ; 00h
  mov cx,255
  mov di,interruptsall
  
  Loop1:
  
  mov bp,8
  mov ax,cx
  mul bp
  mov bp,ax
  
  xor eax,eax
  add eax,intr00
  ;  mov [ds:interruptsall[bp].o0_15],ax
  ;shr     eax,16
  ;mov     [ds:interruptsall[bp].o16_31],ax
  ;mov [ds:interruptsall[bp].se0_15],code32_idx
  ;mov [ds:interruptsall[bp].zb],0
  ;mov [ds:interruptsall[bp].flags],08Eh ; 10001110 selector
  
  mov [di],ax
  shr eax,16
  mov [di + 6],ax
  mov ax,code32_idx
  mov [di + 2],ax
  xor ah,ah
  mov [di + 4],ah
  mov ah,08eh
  mov [di + 5],ah; 10001110 selector
  
  add di,8
 
  jcxz EndLoop1
  dec cx
  jmp Loop1
  EndLoop1:
  

  ; Set idt ptr
  xor eax,eax
	mov     ax,DATA16
	shl     eax,4
	add     ax,interruptsall
	mov     [idt_PM_ptr],eax

  RET
  



ClearScreen:
  pusha
  mov ah,06h
  mov al,0
  xor ecx,ecx
  xor ebx,ebx
  mov dx,174fh
  int 10h
  mov ah,2
  xor ebx,ebx
  xor dx,dx
  int 10h;
  popa
  ret
 
  


