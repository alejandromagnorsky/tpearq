GLOBAL  _read_msw,_lidt
GLOBAL	_print_time
GLOBAL  _int_08_hand
GLOBAL  _int_09_hand
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  _debug

EXTERN  int_08
EXTERN  int_09
EXTERN	printTime


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
	mov	ax, 0			; Lee los segundos
	out	70h, al
	in	al, 71h
	push 	ax

;	mov	al, 2			; Lee los minutos
;	out	70h, al
;	in	al, 71h
	push 	ax

;	mov	al, 4			; Lee las horas
;	out	70h, al
;	in	al, 71h
	push 	ax
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
	
	in	ax, 64h			; Leo el registro donde esta el scan code y lo paso como parámetro
        push	ax
        call    int_09
	pop	ax

                 
        mov	al, 20h			; Envio de EOI generico al PIC
	out	20h, al
	popa                            
        pop     es
        pop     ds
        iret



; Debug para el BOCHS, detiene la ejecució; Para continuar colocar en el BOCHSDBG: set $eax=0
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
