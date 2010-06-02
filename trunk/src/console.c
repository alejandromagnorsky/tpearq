#include "../include/console.h"


void __INIT_TTY(){

	int i=0,j=0;

	// Initialize all consoles
	for(;i<__MAX_TERMINALS;i++){
		__tty[i].ptr = 0;
		__tty[i].attr = GREEN_FG | BLACK_BG;
		__tty[i].empty = 1;
	
		while(j<80*25)
			__tty[i].buf[j++] = ' ';
	}

	__TTY_INDEX = 0;
	__flush_terminal(0);
}

void __clear_terminal() {
	int i = 0;
	while( i< (80*25))
		__tty[__TTY_INDEX].buf[i++] = ' ';
	__tty[__TTY_INDEX].ptr = 0;
	__tty[__TTY_INDEX].empty = 1;
		
	__flush_terminal(0);
	__printSystemSymbol();
}

void __printSystemSymbol(){
	printf("tty%d/:", __TTY_INDEX);
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

		act_tty->empty = 0;

		switch( buffer[i] ){
			
			case '\n':
				row = (act_tty->ptr/80) % 25;
				if(row == 24 )
					__scroll_terminal();
				else while(act_tty->ptr<80*(row+1))
					act_tty->buf[act_tty->ptr++] = ' ';
				__printSystemSymbol();
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


void __scroll_terminal(){

	int i=0;
	for(;i<80*24;i++)
		__tty[__TTY_INDEX].buf[i] = __tty[__TTY_INDEX].buf[i+80];
	for(i=0;i<80;i++)
		__tty[__TTY_INDEX].buf[80*24+i]= ' ';
	__tty[__TTY_INDEX].ptr = 80*24;
	__flush_terminal(0);
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

void __switch_next_terminal(){
	if(++__TTY_INDEX == __MAX_TERMINALS)
		__TTY_INDEX = 0;
	__flush_terminal(0);
	if(__tty[__TTY_INDEX].empty)
		__printSystemSymbol();
}



void __switch_last_terminal(){
	if(--__TTY_INDEX < 0)
		__TTY_INDEX = __MAX_TERMINALS-1;
	__flush_terminal(0);
	if(__tty[__TTY_INDEX].empty)
		__printSystemSymbol();
}

int __switch_terminal(int index){

	if(index >= __MAX_TERMINALS )
		return -1;
	else {
		__TTY_INDEX = index;
		__flush_terminal(0);
		__printSystemSymbol();
	}
	
	return index;
}
