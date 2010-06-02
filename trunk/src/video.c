#include "../include/video.h"
#include "../include/kasm.h"

int __write_test(char c, int pos, int attr){

	if( pos > 80*25 || pos < 0)
		return -1;

	char * vidmem = (char*) 0xb8000;
	vidmem[pos*2] = c;
	vidmem[(pos*2)+1] = attr;

	_move_cursor(pos);

	return pos;
}

