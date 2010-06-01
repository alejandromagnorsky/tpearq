#include "../include/keyboard.h"

int __KBUFFER_PTR_ = 0;	//Puntero al buffer del teclado
int __keyboard_buffer[KBUFFER_SIZE] = {0};

int makeCodes[53] =	 {-5, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', -5, '\b', '\t', 'q', 'w', 'e', 'r', 
			't', 'y', 'u', 'i', 'o', 'p', -5, '+', '\n', -5, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k',
			'l', -5, '{', '}', -5, '<', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/'};
int breakCodes[53] =	 {-5, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', -5, '\b', '\t', 'q', 'w', 'e', 'r', 
			't', 'y', 'u', 'i', 'o', 'p', -5, '+', '\n', -5, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k',
			'l', -5, '{', '}', -5, '<', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/'};

void printKey(){
	int index = _read_scancode() & 0xFF;
	if (index > 0x80)
		__keyboard_buffer[__KBUFFER_PTR_] = breakCodes[index-0x80-1];	
	else{
		__keyboard_buffer[__KBUFFER_PTR_] = makeCodes[index-1];
		printf("%c", __keyboard_buffer[__KBUFFER_PTR_]);
	}

	__KBUFFER_PTR_ = (__KBUFFER_PTR_+ 1) % KBUFFER_SIZE;
}

