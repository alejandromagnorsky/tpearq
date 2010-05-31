#include "../include/video.h"

void __clear_screen() {
	int i;
	while( i< (80*25*2)){
		__video_buffer[i++] = ' ';
		__video_buffer[i++] = __CONSOLE_ATTR_;
	}
	__flush_screen(0);
}

int __flush_screen(int append){
	char * vidmem = (char*) 0xb8000;
	int i;
	if(append){
		for(i=-append*2;i<0;i++)
			vidmem[__CONSOLE_PTR_+i] = __video_buffer[__CONSOLE_PTR_+i];
	} else for(i = 0;i<80*25*2;i++)
		vidmem[i] = __video_buffer[i];
	return append;
}

int __write_screen( const char* buffer, int count){

	int i = 0;
	while(i<count){
		if(__CONSOLE_PTR_ >= 80*25*2)
			__CONSOLE_PTR_ = 0;

		// Si el puntero en la consola no es par, esta corrupto
		if(__CONSOLE_PTR_ % 2 != 0){
			__CONSOLE_PTR_ = 0;
			return -1;
		}


		if(buffer[i] == '\n'){
			int row = (__CONSOLE_PTR_/160) % 25;
			__CONSOLE_PTR_ = (80*(row+1)*2);
			i++;
		} else {
			__video_buffer[__CONSOLE_PTR_++] = buffer[i++];	
			__video_buffer[__CONSOLE_PTR_++] = __CONSOLE_ATTR_;
		}
	}

	__flush_screen(count);

	return 1;
}
