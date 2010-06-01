#include "../include/console.h"


void __INIT_TTY(){

	int i=0,j=0;

	// Initialize all consoles
	for(;i<__MAX_TERMINALS;i++){
		__tty[i].ptr = 0;
		__tty[i].attr = GREEN_FG | BLACK_BG;
	
		while(j<80*25)
			__tty[i].buf[j++] = ' ';
	}

	__TTY_INDEX = 0;
}

void __clear_terminal() {
	int i;
	while( i< (80*25))
		__tty[__TTY_INDEX].buf[i++] = ' ';
	
	__flush_terminal(0);
}

int __write_terminal( const char* buffer, int count){

	int i = 0;
	int row,j;
	int append = count;
	// The pointer to the active terminal
	__terminal * act_tty = __tty + __TTY_INDEX;
	while(i<count){

		// Si el cursor se pasa de la pantalla, vuelvo a 0
		if(act_tty->ptr >= 80*25)
			act_tty->ptr = 0;

		// Si es negativo, vuelvo a 0
		if(act_tty->ptr < 0)
			act_tty->ptr = 0;

		switch( buffer[i] ){
			
			case '\n':
				row = (act_tty->ptr/80) % 25;
				act_tty->ptr = (80*(row+1));
				break;
			case '\t':
				for(j=0;j<__TAB_LENGTH;j++)
					act_tty->buf[act_tty->ptr++] = ' ';	
				break;
			case '\b':
				act_tty->buf[--act_tty->ptr] = ' ';
				append--;				
				break;
			default:
				act_tty->buf[act_tty->ptr++] = buffer[i];	
				break;
		}
		i++;
	}

	__flush_terminal(append);

	return count;
}



void __flush_terminal(int append){
	int i;
	// The pointer to the active terminal
	__terminal * act_tty = __tty + __TTY_INDEX;
	if(append){
		for(i=-append*2;i<0;i++)
			__write_test(act_tty->buf[act_tty->ptr+i], act_tty->ptr+i, act_tty->attr);
	} else for(i = 0;i<80*25*2;i++)
		__write_test(act_tty->buf[i], i, act_tty->attr);
	return;
}

int __switch_terminal(int index){

	if(index >= __MAX_TERMINALS )
		return -1;
	else {
		__TTY_INDEX = index;
		__flush_terminal(0);
	}
	
	return index;
}
