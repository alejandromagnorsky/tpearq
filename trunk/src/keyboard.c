#include "../include/keyboard.h"
/*	PROBLEMAS
**	TECLAS QUE NO TIRAN INTERRUPCIÓN:
**		'ñ', '¿', la de al lado de la P, AltGr.	
**	NO TIENE FUNCIÓN SHIFT:
**		tecla de pipe,
**	CASOS IGUALES
**		SHIFT+7 = SHIFT+(tecla al lado del cero)
**		EL '<' Y LA TECLA DE LA COMA APUNTAN A LA MISMA POSICION EN LA TABLA (el signito < de la tabla sin shift)
**		SHIFT+(tecla de punto y coma) = SHIFT+(tecla de punto)
**		SHIFT+cero = SHIFT+(tecla al lado del punto) 		
**	TAB FALTA ES EL 999 EN AMBAS TABLAS
*/

int __SHIFT_ = 0;	// Flag Shift (0 = deshabilitado)
int __CAPSLOCK_ = 0;	// Flag Capslock (0 = dehabilitado)
int __KBUFFER_PTR_ = 0;	// Puntero al buffer del teclado
int __keyboard_buffer[KBUFFER_SIZE] = {0};

int makeCodes[73] =	 {-5, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '{', -5, '\b', '\t', 'q', 'w', 'e', 'r', 
			't', 'y', 'u', 'i', 'o', 'p', -5, '+', '\n', -5, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k',
			'l', 164, '\'', '}', -5, '|', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '.', '/', -5, -5, -5, ' ', -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, 'N', -5, -5, -5};

int shiftMakeCodes[54] = {-5, '!', '{', '#', '$', '%', '&', '&', '(', '(', ')', '=', '=', '\b', '\t', 'Q', 'W', 'E', 'R', 
			'T', 'Y', 'U', 'I', 'O', 'P', -5, '+', '\n', -5, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K',
			'L', ';', '"', '}', -5, 302, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '>', '>', '/', '/'};

void printKey(){
	int index = _read_scancode() & 0xFF;
	if ( index == 170)
		__SHIFT_ = 0;  
	if (index <= 83){
		__keyboard_buffer[__KBUFFER_PTR_] = makeCodes[index-1];
		
		if(index == 42)
			__SHIFT_ = 1;
		else if (index == 58)
			__CAPSLOCK_ = !__CAPSLOCK_;
		else if (__CAPSLOCK_ && __keyboard_buffer[__KBUFFER_PTR_] <= 'z' && __keyboard_buffer[__KBUFFER_PTR_] >= 'a')
			if (!__SHIFT_)
				printf("%c", __keyboard_buffer[__KBUFFER_PTR_]+'A'-'a');
			else
				printf("%c", __keyboard_buffer[__KBUFFER_PTR_]);
		else if(__SHIFT_){
			__keyboard_buffer[__KBUFFER_PTR_] = shiftMakeCodes[index-1];
			printf("%c", __keyboard_buffer[__KBUFFER_PTR_]);
		}
		else
			printf("%c", __keyboard_buffer[__KBUFFER_PTR_]);

		__KBUFFER_PTR_ = (__KBUFFER_PTR_+ 1) % KBUFFER_SIZE;
	}
}
