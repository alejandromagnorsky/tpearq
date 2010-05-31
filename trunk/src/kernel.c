#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kernel.h"
#include "../include/keyboard.h"

DESCR_INT idt[0xA];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */

int tickpos=30;

void int_08() {

}

void int_09(){
	//
	int scanCode = _read_scancode() & 0xFF;
	int i;
	for(i = 0; i < 8; i++){
		//if(scanCodes[i][1] == scanCode)
			printf("%d",scanCode);
	};
}

size_t __write(int fd, const void* buffer, size_t count){
	switch(fd){
		case stdout:
			__write_screen(buffer,count);
			break;
		case stdin:
			// LLamar al teclado
			break;
		default:
			break;
	}
	return fd;
}


/**********************************************
kmain() 
Punto de entrada de C
*************************************************/

kmain() 
{

        int i,num;

/* Borra la pantalla. */ 

	__clear_screen();
	printSystemSymbol();
	printf("ImpactTrueno hizo %d de danio!!! \n", 4);
	printf("Pikachu se cayo!! \n");
	printf("Nooooo! \n");
	printf("Esto es un %s \n", "pija");
	printf("Me gusta la barra n \n");
	printf("lala \n");
	printf("La hora es: ");
	printf("esto es un %c \n", 'B');
	_print_time();

//	printf("HOLA");
//	_print_time()

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


        while(1)
        {

        }
	
}

