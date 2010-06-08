GLOBAL  _read_msw,_lidt
GLOBAL	_read_scancode
GLOBAL  _turn_cursor_on
GLOBAL  _turn_cursor_off
GLOBAL 	_move_cursor
GLOBAL	_int_00_hand
GLOBAL  _int_20_hand
GLOBAL  _int_21_hand
GLOBAL  _int_80_hand
GLOBAL	_inport
GLOBAL  _outport
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  _debug

EXTERN int_00_hand
EXTERN  int_20
EXTERN  int_21
EXTERN __write
EXTERN __read
EXTERN getAscii
EXTERN __keyboard_buffer
EXTERN __KBUFFER_PTR_

SECTION .text


_Cli:
	cli			; limpia flag de interrupciones
	ret

_Sti:

	sti			; habilita interrupciones por flag
	ret

_mascaraPIC1:			; Escribe mascara del PIC 1
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	21h,al
        pop     ebp
        retn

_mascaraPIC2:			; Escribe mascara del PIC 2
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	0A1h,al
        pop     ebp
        retn

_read_msw:
        smsw    ax		; Obtiene la Machine Status Word
        retn


_lidt:				; Carga el IDTR
        push    ebp
        mov     ebp, esp
        push    ebx
        mov     ebx, [ss: ebp + 6] ; ds:bx = puntero a IDTR 
	rol	ebx,16		    	
	lidt    [ds: ebx]          ; carga IDTR
        pop     ebx
        pop     ebp
        retn


_int_20_hand:				; Handler de INT 20h (Timer tick)
        push    ds
        push    es                      ; Se salvan los registros
        pusha                           
        mov     ax, 10h			
        mov     ds, ax			; Carga de DS y ES con el valor del selector a utilizar.
        mov     es, ax                  
        call    int_20                 
        mov	al, 20h			; Envio de EOI generico al PIC
	out	20h, al
	popa                            
        pop     es
        pop     ds
        iret

_int_21_hand:				; Handler de INT 21h (Teclado)
        push    ds
        push    es                      ; Se salvan los registros
        pusha      
                     
        mov     ax, 10h			
        mov     ds, ax			; Carga de DS y ES con el valor del selector a utilizar.
        mov     es, ax	

	call	int_21

        mov	al, 20h			; Envio de EOI generico al PIC
	out	20h, al
	popa                            
        pop     es
        pop     ds
        iret


_int_80_hand:
	push	ebp
	mov	ebp, esp
	push    ds
        push    es                      ; Se salvan los registros

	push dword	[ebp+12]		; Pushea los parámetros de read y write
	push dword	[ebp+16]
	push dword	[ebp+20]
	mov	eax, [ebp+8]		; ebp+8 = 0 --> Read. ebp+20 = 1 --> Write
	cmp	eax, 0
	je	read
	jmp	write

continue:
	add	esp, 12	
	pop     es
        pop     ds
	leave
	ret
read:
	call	__read
	jmp	continue
write:
	call	__write
	jmp	continue



_inport:
	push ebp
	mov ebp, esp
	push dx	

	mov eax, 0
	mov dx, [ebp+8]
	in al, dx
	
	pop dx
	leave
	ret

_outport:
	push ebp
	mov ebp, esp
	pusha

	mov dx, [ebp+8]
	mov ax, [ebp+12]
	out dx, al
	
	popa
	leave
	ret

_read_scancode:
	push	ebp
	mov	ebp, esp
	in	al, 60h
	leave
	ret       

_move_cursor:
	push ebp
	mov ebp, esp

	mov cx, [ebp+8]

	mov dx, 3D4h
	mov ax, 0Fh
	out dx, ax

	mov dx, 3D5h
	mov ax, cx
	out dx, ax

	sar cx, 8

	mov dx, 3D4h
	mov ax, 0Eh
	out dx, ax

	mov dx, 3D5h
	mov ax, cx
	out dx, ax


	leave
	ret

_turn_cursor_on:
	push ebp
	mov ebp, esp
	
	mov eax, 0
	mov edx, 0

	mov dx, 3D4h
	mov ax, 0Ah
	out dx, ax

	mov dx, 3D5h
	mov ax, 0h
	out dx, ax


	leave
	ret


_turn_cursor_off:
	push ebp
	mov ebp, esp
	
	mov eax, 0
	mov edx, 0

	mov dx, 3D4h
	mov ax, 0Ah
	out dx, ax

	mov dx, 3D5h
	mov ax, 020h
	out dx, ax


	leave
	ret



; Debug para el BOCHS, detiene la ejecución; Para continuar colocar en el BOCHSDBG: set $eax=0
;


_debug:
        push    bp
        mov     bp, sp
        push	ax
vuelve:	mov     ax, 1
        cmp	ax, 0
	jne	vuelve
	pop	ax
	pop     bp
        retn
