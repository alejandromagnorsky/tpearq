#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kernel.h"
#include "../include/kc.h"
#include "../include/keyboard.h"

DESCR_INT idt[0xA];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */

void int_08() {

}

void int_09(){
	int scanCode = _read_scancode() & 0xFF;
	int asciiCode = getAscii(scanCode);
	
	if(asciiCode != -1){
		kbuffer.buf[kbuffer.__KBUFFER_PTR_WR] = asciiCode;
		kbuffer.__KBUFFER_PTR_WR = (kbuffer.__KBUFFER_PTR_WR+1) % KBUFFER_SIZE;	
	}
}

size_t __read(int fd, void* buffer, size_t count){
	int i;
	int * vec = buffer;
	for(i = 0; i <= count; i++){
		if( kbuffer.__KBUFFER_PTR_RD ==  kbuffer.__KBUFFER_PTR_WR )
			return i;
		vec[i] = kbuffer.buf[kbuffer.__KBUFFER_PTR_RD];
		kbuffer.__KBUFFER_PTR_RD = (kbuffer.__KBUFFER_PTR_RD+1) % KBUFFER_SIZE;
	}
	return i;
}

size_t __write(int fd, const void* buffer, size_t count){
	if(fd == stdout)
			__write_terminal(buffer,count);
	return fd;
}




/**********************************************
kmain() 
Punto de entrada de C
*************************************************/

kmain() 
{

        int i,num;

	kbuffer.__KBUFFER_PTR_RD = 0;
	kbuffer.__KBUFFER_PTR_WR = 0;
	// First initialize terminals.
	__INIT_TTY();

	_turn_cursor_on();

	__printSystemSymbol();

	printf("Trucos:\n \t SHIFT+T: Nueva terminal (circular). \n \t SHIFT+C: Borra terminal.\n");


/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0    */
//Rutina de atención del timer tick
	setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_hand, ACS_INT, 0);

//Rutina de atención del teclado
	setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_hand, ACS_INT, 0);
	

/* Carga de IDTR    */

	idtr.base = 0;  
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt (&idtr);	

	_Cli();

/* Habilito interrupcion del teclado */

	_mascaraPIC1(0xFD);
	_mascaraPIC2(0xFF);

	_Sti();


       char c;
        while(1)
        {
		c = getc();
		if( c != -1 ) 
			printf("%c", c);
		if( c == 'T')
			__switch_next_terminal();
		if( c == 'C')
			__clear_terminal();
        }
	
}

