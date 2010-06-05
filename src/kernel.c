#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kernel.h"
#include "../include/kc.h"
#include "../include/keyboard.h"

DESCR_INT idt[0x30];			/* IDT de 30 entradas*/
IDTR idtr;				/* IDTR */


/* PREGUNTAR SI SE PUEDE USAR LA MISMA PAGINA WEB QUE OTRO GRUPO
** Initialize the PICs and remap them
*/
void initializePics()
{
	/* Send ICW1 to both PICS 
	** ICW1 is used to tell the PICs if a ICW4 is following 
	** and if the PIC is working in a cascaded PIC environment
	*/

	_outport(0x20, 0x11);
	_outport(0xA0, 0x11);
	/* Send ICW2 to both PICS 
	** ICW2 tells the PICs where to map IRQ0 and IRQ8.
	*/
	_outport(0x21, 0x20);
	_outport(0xA1, 0x28);
	/* Send ICW3 to both PICS 
	** ICW3 is used for telling the PICs which IRQ to use for
	** the communication between each other
	*/
	_outport(0x21, 2);      // Set the master PIC's IRQ2 to be connected whit the slave PIC
	_outport(0xA1, 0);	// For the slave PIC, it means that it is the Slave0
	/* Send ICW4 to both PICS 
	** ICW4 is used for telling that we are working in a 80x86 architecture 
	** and if the interruption is handled automatically or if it needs help from software.
	*/
	_outport(0x21, 1);
	_outport(0xA1, 1);
}


void int_20() {

}

void int_21(){
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
	return count;
}




/**********************************************
kmain() 
Punto de entrada de C
*************************************************/

kmain() 
{
	initializePics();
      	kbuffer.__KBUFFER_PTR_RD = 0;
	kbuffer.__KBUFFER_PTR_WR = 0;
	// First initialize terminals.
	__INIT_TTY();

	_turn_cursor_on();

	__printSystemSymbol();

	printf("Trucos:\n \t SHIFT+Q: Proxima terminal.  \n \t SHIFT+W: Ultima terminal \n \t SHIFT+C: Borra terminal.\n");
	printf("Nota: SCROLLEA!!!");


/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0    */
//Rutina de atención del timer tick
	setup_IDT_entry (&idt[0x20], 0x08, (dword)&_int_20_hand, ACS_INT, 0);

//Rutina de atención del teclado
	setup_IDT_entry (&idt[0x21], 0x08, (dword)&_int_21_hand, ACS_INT, 0);
	

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

/* TESTEO SCANF */
	int in1 = -1;
	int in2 = -1;
	int ch1 = 'X';
	int ch2 = 'X';
	
	char c;
        while(1)
        {
		/* TESTEO SCANF
			c = scanf("Pija%c %d %d %c", &ch1, &in1, &in2, &ch2);
			printf("\n EN CH1 sE GUARDO: %c|||", ch1);	//PRINTF DE  DEBUGGEO//
			printf("\n EN IN1 sE GUARDO: %d|||", in1);	//PRINTF DE  DEBUGGEO//
			printf("\n EN IN2 sE GUARDO: %d|||", in2);	//PRINTF DE  DEBUGGEO//
			printf("\n EN CH2 sE GUARDO: %c|||", ch2);	//PRINTF DE  DEBUGGEO//*/
		c = getc();
		if( c != -1 ) 
			switch(c){

				case 'Q':
					__switch_next_terminal();
					break;
				case 'W':
					__switch_last_terminal();
					break;
				case 'C':
					__clear_terminal();
					break;
				default:
					printf("%c", c);
					break;
			}
        }
	
}

