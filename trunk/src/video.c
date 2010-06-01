#include "../include/video.h"


void __INIT_VIDEO(){

	int i=0,j=0;

	// Initialize all consoles
	for(;i<__MAX_CONSOLES;i++){
		__consoles[i].ptr = 0;
		__consoles[i].attr = GREEN_FG | BLACK_BG;
	
		while(j<80*25*2){
			__consoles[i].buf[j++] = ' ';
			__consoles[i].buf[j++] = __consoles[i].attr;
		}
	}

	// Set as screen the first console
	__SCREEN = __consoles;
}

void __clear_screen() {
	int i;
	while( i< (80*25*2)){
		__SCREEN->buf[i++] = ' ';
		__SCREEN->buf[i++] = __SCREEN->attr;
	}
	__flush_screen(0);
}

int __flush_screen(int append){
	char * vidmem = (char*) 0xb8000;
	int i;
	if(append){
		for(i=-append*2;i<0;i++)
			vidmem[__SCREEN->ptr+i] = __SCREEN->buf[__SCREEN->ptr+i];
	} else for(i = 0;i<80*25*2;i++)
		vidmem[i] = __SCREEN->buf[i];
	return append;
}

int __write_screen( const char* buffer, int count){

	int i = 0;
	while(i<count){
		if(__SCREEN->ptr >= 80*25*2)
			__SCREEN->ptr = 0;

		// Si el puntero en la consola no es par, esta corrupto
		if(__SCREEN->ptr % 2 != 0){
			__SCREEN->ptr = 0;
			return -1;
		}


		if(buffer[i] == '\n'){
			int row = (__SCREEN->ptr/160) % 25;
			__SCREEN->ptr = (80*(row+1)*2);
			i++;
		} else {
			__SCREEN->buf[__SCREEN->ptr++] = buffer[i++];	
			__SCREEN->buf[__SCREEN->ptr++] = __SCREEN->attr;
		}
	}

	__flush_screen(count);

	return 1;
}
