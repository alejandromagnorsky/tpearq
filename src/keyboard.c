#include "../include/keyboard.h"
/*	PROBLEMAS
**	TECLAS QUE NO TIRAN INTERRUPCIÓN:
**		AltGr.	
**	CASOS IGUALES
**		| y 1
**	AVERIGUAR EL SCAN CODE DE
**		La tecla de al lado de la Ñ
**		Shift+|
*/

int __L_SHIFT_ = 0;	// Left Shift flag (0 = unabled)
int __R_SHIFT_ = 0; // Right Shift flag (0 = unabled)
int __CAPSLOCK_ = 0;	// Capslock flag (0 = unabled)
int __NUMLOCK_ = 0; // NumLock flag (0 = unabled)

int makeCodes[83] =	 {-1, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 39, 168, '\b', '\t', 'q', 'w', 'e', 'r', 
			  't', 'y', 'u', 'i', 'o', 'p', -5, '+', '\n', -5, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 
                          164, '{', '}', 124, -5, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', -5, -5, -5, ' ', -5,
			  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 'N', 'S', 'H', 202, -1, '-', 185, 206, 204, '+', 'E',
			  203, -1, 'I', 'D'};

int shiftMakeCodes[83] = {-1, '!', '"', '#', '$', '%', '&', '/', '(', ')', '=', '\?', 173, '\b', '\t', 'Q', 'W', 'E', 'R', 
			  'T', 'Y', 'U', 'I', 'O', 'P', -5, '*', '\n', -5, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 
			  165, '[', ']', 167, -5, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', -5, -5, -5, ' ', -5,
			  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 'N', 'S', 'H', 202, -1, '-', 185, 206, 204, '+', 'E',
			  203, -1, 'I', 'D'};

int numpadMakeCodes[13] = {'7', '8', '9', -1, '4', '5', '6', -1, '1', '2', '3', '0', '.'};

int getAscii(int scanCode){
	int asciiCode = -1;
	
	if (scanCode == 170)
		__L_SHIFT_ = 0;  
	else if(scanCode == 182)
		__R_SHIFT_ = 0;
	// Si es un make code
	else if (scanCode <= 128){
		if(scanCode == 42)
			__L_SHIFT_ = 1;
		else if(scanCode == 54)
			__R_SHIFT_ = 1;
		else if (scanCode == 58)
			__CAPSLOCK_ = !__CAPSLOCK_;
		else if(scanCode == 69)
			__NUMLOCK_ = !__NUMLOCK_;
		else if (__CAPSLOCK_ && isLetter(scanCode) ){
			if (!__L_SHIFT_ && !__R_SHIFT_)
				asciiCode = shiftMakeCodes[scanCode-1];
			else
				asciiCode = makeCodes[scanCode-1];
		}else if( (__L_SHIFT_ || __R_SHIFT_ || __NUMLOCK_) && isNumber(scanCode))
			asciiCode = numpadMakeCodes[scanCode-71];
		else if(__L_SHIFT_ || __R_SHIFT_)
			asciiCode = shiftMakeCodes[scanCode-1];
		
		else
			asciiCode = makeCodes[scanCode-1];	
	}
	return asciiCode;
}

char isLetter(int scanCode){
	return (makeCodes[scanCode-1] <= 'z' && makeCodes[scanCode-1] >= 'a') 
	         || makeCodes[scanCode-1] == 164;
}

char isNumber(int scanCode){
	return (scanCode >= 71 && scanCode <= 73) 
                 || (scanCode >= 75 && scanCode <= 77)
                 || (scanCode >= 79 && scanCode <= 83);
}
