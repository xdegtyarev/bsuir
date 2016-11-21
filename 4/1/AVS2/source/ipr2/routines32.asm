[bits 32]

VIDEO_MEMORY equ 0xb8500 ;writing directly to Video memory
WHITE_ON_BLACK equ 0x0f ; the color byte for each character

print32:
	pusha
	; mov ecx, edx ; STRING
	; mov ebx, ebx ; COULEUR
	mov edx, eax
	add edx, VIDEO_MEMORY


print32loop:
	mov al, [ebx] ; [ebx] is the address of our character
    mov ah, WHITE_ON_BLACK

	cmp al, 0 ; check for end of string
	je complete32

    mov [edx], ax ; store character + attribute in video memory
    add ebx, 1 ; next char
    add edx, 2 ; next video memory position
	jmp print32loop

complete32:
	popa
	ret
