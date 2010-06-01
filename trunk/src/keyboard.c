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


int makeCodes[73] =	 {-5, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '{', -5, '\b', '\t', 'q', 'w', 'e', 'r', 
			't', 'y', 'u', 'i', 'o', 'p', -5, '+', '\n', -5, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k',
			'l', 164, '\'', '}', -5, '|', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '.', '/', -5, -5, -5, ' ', -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, 'N', -5, -5, -5};

int shiftMakeCodes[54] = {-5, '!', '{', '#', '$', '%', '&', '&', '(', '(', ')', '=', '=', '\b', '\t', 'Q', 'W', 'E', 'R', 
			'T', 'Y', 'U', 'I', 'O', 'P', -5, '+', '\n', -5, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K',
			'L', ';', '"', '}', -5, 302, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '>', '>', '/', '/'};

int getAscii(int scanCode){
	int asciiCode = -1;
	
	if (scanCode == 170)
		__SHIFT_ = 0;  
	if (scanCode <= 83){
		if(scanCode == 42)
			__SHIFT_ = 1;
		else if (scanCode == 58)
			__CAPSLOCK_ = !__CAPSLOCK_;
		else if (__CAPSLOCK_ && makeCodes[scanCode-1] <= 'z' && makeCodes[scanCode-1] >= 'a'){
			if (!__SHIFT_)
				asciiCode = makeCodes[scanCode-1]+'A'-'a';
			else
				asciiCode = makeCodes[scanCode-1];
		}else if(__SHIFT_)
			asciiCode = shiftMakeCodes[scanCode-1];
		else
			asciiCode = makeCodes[scanCode-1];	
	}
	return asciiCode;
}
