#include "../include/kernel.h"
#include "../include/kc.h"

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
	if ( (buffer[__KBUFFER_PTR_] & 0x0F) == 0 )
		while ( (c = buffer[__KBUFFER_PTR_] & 0x0F) == 0 ){ 
		//Esta función debería llamar a la interrupción de teclado
		// y esperar a que se escriba algo en caso de que el buffer
		//esté vacío
		}

	__KBUFFER_PTR_ += 2;

	if (__KBUFFER_PTR_ > 0x3D)
		__KBUFFER_PTR_ = 0x1E;
	
	return c;
}

int putc( int c ){
	return __write(stdout,&c, 1);
} 

int printf(const char * str, ...){
	// Hay muchas variables adelante para evitar conflictos
	// con el codigo inyectado en assembler
	int c;

	int wait = 0;
	void ** argv = (void **)(&str);

	for(c=0;str[c] != NULL;c++){
		if(wait){
			switch(str[c]){
			case 'd':
				putInt( *( (int*) argv));
				break;
			case 'c':
				putc(*((char*)argv));
				break;
			case 's':
				printf(*((char**)argv));
				break;
			default:
				break;
			}
			wait = 0;
		} else {
			if(str[c] == '%'){
				wait = 1;	
				argv++;
			}
			else 
				putc(str[c]);
		}
	}
}

int pow(int n, int exp){
	int i;
	int out = 1;
	for(i=0;i<exp;i++)
		out*=n;
	return out;
}

void putInt(int n){

	int i,j;
	for(i=0;n/pow(10,i) > 0 ;i++); //cantidad de caracteres
	for(j=i-1;j>0;j--){
		putc( (n/pow(10,j))+'0');
		n = n%pow(10,j);
	}
	putc((n%10)+'0');
}

/* Imprime símbolo de sistema */
void printSystemSymbol() 
{
	printf("Pikachu:/>", 4);
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