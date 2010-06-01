GLOBAL  _read_msw,_lidt
GLOBAL	_print_time
GLOBAL	_read_scancode
GLOBAL  _int_08_hand
GLOBAL  _int_09_hand
GLOBAL  _int_80_hand
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  _debug

EXTERN  int_08
EXTERN  int_09
EXTERN	printTime
EXTERN __write
EXTERN __read
EXTERN putc
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


_print_time:
	push	ebp
	mov	ebp, esp
	mov	eax, 0			; Lee los segundos
	out	70h, al
	in	al, 71h
	push	eax

	mov	eax, 2			; Lee los minutos
	out	70h, al
	in	al, 71h
	push	eax

	mov	eax, 4			; Lee las horas
	out	70h, al
	in	al, 71h
	push	eax
	call	printTime
	add	esp, 12
	leave
	ret




_int_08_hand:				; Handler de INT 8 (Timer tick)
        push    ds
        push    es                      ; Se salvan los registros
        pusha                           
        mov     ax, 10h			
        mov     ds, ax			; Carga de DS y ES con el valor del selector a utilizar.
        mov     es, ax                  
        call    int_08                 
        mov	al, 20h			; Envio de EOI generico al PIC
	out	20h, al
	popa                            
        pop     es
        pop     ds
        iret

_int_09_hand:				; Handler de INT 9 (Teclado)
        push    ds
        push    es                      ; Se salvan los registros
        pusha      
                     
        mov     ax, 10h			
        mov     ds, ax			; Carga de DS y ES con el valor del selector a utilizar.
        mov     es, ax
	

	call	int_09

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



_read_scancode:
	push	ebp
	mov	ebp, esp
	in	al, 60h
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
