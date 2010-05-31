#include "../include/kc.h"

#define NULL 0

/***************************************************************
*k_clear_screen
*
* Borra la pantalla en modo texto color.
****************************************************************/

void k_clear_screen() 
{
	int i=0;
        while(i < (80*25))
        {
		putc(' ');
		i++;
        };
	__CONSOLE_PTR_ = 0;
}

int getc(void)
{
	int c;
	char * buffer = (char *) 0x40;
	
	if(__KBUFFER_PTR_ % 2 != 0)
		return -1;

	/* Si el ASCII en el buffer es 0000 0000 (la última tecla apretada fue una
	** tecla de control o no se apretó ninguna), entonces llama a la interrupción 
	** de lectura de teclado MIENTRAS se presionen teclas de control.
	*/
	if ( (buffer[__KBUFFER_PTR_] & 0xFF) == 0 )
		while ( (c = buffer[__KBUFFER_PTR_] & 0xFF) == 0 ){ 
		//Esta función debería llamar a la interrupción de teclado
		// y esperar a que se escriba algo en caso de que el buffer
		//esté vacío
		}

	__KBUFFER_PTR_ += 2;

	if (__KBUFFER_PTR_ > 0x3D)
		__KBUFFER_PTR_ = 0x1E;
	
	return c;
}

int putc( int character ){
	char * vidmem = (char*) 0xb8000;


	// Si el puntero en la consola no es par, esta corrupto
	if(__CONSOLE_PTR_ % 2 != 0)
		return -1;

	if( __CONSOLE_PTR_ >= 80*25*2)
		__CONSOLE_PTR_ = 0;

	if(character == '\n'){
		int row = (__CONSOLE_PTR_/160) % 25;
		__CONSOLE_PTR_ = (80*(row+1)*2);
		return character;
	}


	vidmem[__CONSOLE_PTR_++] = character;
	vidmem[__CONSOLE_PTR_++] = WHITE_TXT;

	return character;
} 

int printf(const char * str){
	int c;
	int wait = 0;
	for(c=0;str[c] != NULL;c++){
		if(wait){
			switch(str[c]){

			case 'd':
				printf("NUMERO");
				break;
			case 'c':
				putc(str[c]);
				break;
			case 's':
				printf("STRING");
				break;
			default:
				break;
			}

			wait = 0;
		} else {
			if(str[c] == '%')
				wait = 1;
			else 
				putc(str[c]);
		}
	}
}


/* Imprime símbolo de sistema */
void printSystemSymbol() 
{
	printf("Pikachu:/>");
}


/* Imprime la hora */
// Los argumentos están en BCD 
void printTime(dword hours, dword minutes, dword seconds)
{
	putc(((hours & 0x000000F0)>>4) + '0');
	putc((hours & 0x0000000F) + '0');
	putc(':');

	putc(((minutes & 0x000000F0)>>4) + '0');
	putc((minutes & 0x0000000F) + '0');
	putc(':');

	putc(((seconds & 0x000000F0)>>4) + '0');
	putc((seconds & 0x0000000F) + '0');
}




/***************************************************************
*setup_IDT_entry
* Inicializa un descriptor de la IDT
*
*Recibe: Puntero a elemento de la IDT
*	 Selector a cargar en el descriptor de interrupcion
*	 Puntero a rutina de atencion de interrupcion	
*	 Derechos de acceso del segmento
*	 Cero
****************************************************************/

void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero) {
  item->selector = selector;
  item->offset_l = offset & 0xFFFF;
  item->offset_h = offset >> 16;
  item->access = access;
  item->cero = cero;
}
