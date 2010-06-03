#include "../include/console.h"

void __INIT_TTY(){

	__register_special_ascii(__BLOCK_ASCII, ' ');
	__register_special_ascii(__TAB_ASCII, ' ');
	__register_special_ascii(__ENTER_ASCII, ' ');

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
	printf("dio%d/:%c", __TTY_INDEX, __BLOCK_ASCII);
}

void __enter(){
	__terminal * act_tty = __tty + __TTY_INDEX;
	int row = (act_tty->ptr/80) % 25;
	int append = 0;

	while(act_tty->ptr<80*(row+1)){
		act_tty->buf[act_tty->ptr++] = __ENTER_ASCII;
		append++;
	}

	if(row == 24 )
		__scroll_terminal();
	__flush_screen(act_tty->buf,act_tty->ptr-append, act_tty->ptr, act_tty->attr);
}

void __tab(){
	int i;
	__terminal * act_tty = __tty + __TTY_INDEX;
	for(i=0;i<__TAB_LENGTH;i++){
		if(act_tty->ptr == 80*25 - 1 )
			__scroll_terminal();
		act_tty->buf[act_tty->ptr++] = ' ';	
	}
	act_tty->buf[act_tty->ptr-1] = __TAB_ASCII;
}

void __backspace(){
	__terminal * act_tty = __tty + __TTY_INDEX;
	char c = act_tty->buf[act_tty->ptr-1];
	int append = 0;
	switch(c){
	case __BLOCK_ASCII:
		break;
	case __TAB_ASCII:
		for(;append<__TAB_LENGTH;append++)
			act_tty->buf[--act_tty->ptr] = ' ';
		break;
	case __ENTER_ASCII:
		for(;act_tty->buf[act_tty->ptr-1] == __ENTER_ASCII;append++)
			act_tty->buf[--act_tty->ptr] = ' ';
		break;
	default:
		act_tty->buf[--act_tty->ptr] = ' ';		
	}

	__flush_screen(act_tty->buf,act_tty->ptr, act_tty->ptr+append, act_tty->attr);
	return;
}

int __write_terminal( const char* buffer, int count){

	int i = 0;
	int j;
	int append = count;

	// The pointer to the active terminal
	__terminal * act_tty = __tty + __TTY_INDEX;
	act_tty->empty = 0;

	while(i<count){

		// Si el cursor se pasa de la pantalla, vuelvo a 0
		if(act_tty->ptr >= 80*25 || act_tty->ptr < 0)
			act_tty->ptr = 0;

		switch( buffer[i] ){
			
			case '\n':
				__enter();
				__printSystemSymbol();
				break;
			case '\t':
				__tab();
				break;
			case '\b':
				__backspace();
				break;
			default:
				act_tty->buf[act_tty->ptr++] = buffer[i];	
				if(act_tty->ptr == 80*25 )
					__scroll_terminal();

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
	// The pointer to the active terminal
	__terminal * act_tty = __tty + __TTY_INDEX;
	
	if(append) 
		__flush_screen(act_tty->buf, act_tty->ptr-append, act_tty->ptr, act_tty->attr);
	 else
		__flush_screen(act_tty->buf,0, 80*25,act_tty->attr);

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
