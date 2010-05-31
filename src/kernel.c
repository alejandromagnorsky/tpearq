#include "../include/kasm.h"
#include "../include/defs.h"

DESCR_INT idt[0xA];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */

int tickpos=30;
int scanCodes[8][2] = {{'a', 0x1E61}, {'A', 0x1E41}, {'A', 0x1E01}, {'A', 0x1E00}, {'b', 0x3062}, {'B', 0x3042}, {'B', 0x3002}, {'B', 0x3000}};




void int_08() {

	char *video = (char *) 0xb8000;
	video[tickpos+=2]='H';

}

void int_09(dword scanCode) {

	char *video = (char *) 0xb8000;
	int i;
	for(i = 0; i < 8; i++){
		if(scanCodes[i][1] != scanCode)
			video[tickpos+=2] = scanCodes[i][0];
	};
}

/**********************************************
kmain() 
Punto de entrada de cóo C.
*************************************************/

kmain() 
{

        int i,num;

/* Borra la pantalla. */ 

	k_clear_screen();
	printSystemSymbol();
	printf("ImpactTrueno hizo %d de danio!! \n");
	printf("Pikachu se cayo!! \n");
	printf("Nooooo! \n");
	printf("Esto es un %s \n");
	printf("Me gusta la barra n \n");
	printf("lala \n");
	printf("La hora es: ");
	printTime();


//	for(i=0;i<79*25;i++)
//		putc('A');

//	printf("HOLA");
//	_print_time()

/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0    */

//Rutina de atención del timer tick
//	setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_hand, ACS_INT, 0);

//Rutina de atención del teclado
//	setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_hand, ACS_INT, 0);
	
/* Carga de IDTR    /

	idtr.base = 0;  
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt (&idtr);	

	_Cli();
/* Habilito interrupcion del teclado/

	_mascaraPIC1(0xFD);
	_mascaraPIC2(0xFF);
        
	_Sti();
*/

        while(1)
        {

        }
	
}

