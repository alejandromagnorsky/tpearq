#include "../include/kernel.h"
#include "../include/kc.h"

int getc(){
	int character = -1;
	while (character == -1)
		_int_80_hand(0, stdin, &character, 1);
	return character;
	
}

int scanf(const char * str, ...){
	int i, j, c, lengthStr = 0;
	int flagPorc = 0;
	int retFlag = 0;
	void ** argv = (void **) (&str);
	
/* DEBUGGEO --> LOS PRINTF SON PARA DEBUGGEO */
	if ( str[0] == ""){
		printf("%s", "string vacia");
		return -1;
	}

	/*  Falta validar si hay EOF despues de un %. Probar desues con strings cuyo ultimo caracter sea % */
	for (i=0 ;  (c=getc()) != '\n'; i++){
		if (!lengthStr || ( i!=0 && i == lengthStr) ){
			if ( str[i] == '%' )	
				flagPorc = 1;	
			else if( flagPorc ){		
				switch(str[i]){
				case 'd':
					*((int *)++argv) = c;
					break;
				case 'c':
					*((char *)++argv) = c;
					break;
				case 's':
					*((char **)++argv) = c;
					break;
				default:
					break;
				}
				flagPorc = 0;
			} else if ( c != str[i] ){
				printf("|| TECLA MAL: %c ||",c);
				retFlag = 1;
			} else if (!lengthStr && str[i+1] == '\0'){
				lengthStr = i+1;	//Reduzco la lengthStr en 1 así nunca va a ser igual
			}
		}
		printf("%c",c);
	}
	printf("%c",c);	//Si salio del for, este c es un barra n
	if (i==0 || retFlag || i != lengthStr){
		printf("DEBUGGEO i:%d  l:%d: no matcheó str de scanf", i, lengthStr);
		return -1;
	} else
		printf("%s","MATCHEO BIEN");
	
}

int putc( int c ){
	return _int_80_hand(1,stdout,&c, 1);
} 

int printf(const char * str, ...){
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
	
	if(n<0){
		putc('-');
		n = -n;
	}

	for(i=0;n/pow(10,i) > 0 ;i++); //cantidad de caracteres
	for(j=i-1;j>0;j--){
		putc( (n/pow(10,j))+'0');
		n = n%pow(10,j);
	}
	putc((n%10)+'0');
}

int strcmp(const char * str1, const char * str2){

	int i = 0;
	int out = 0;
	while(str1[i] != NULL || str2[i] != NULL){

		if( str1[i] != str2[i])
			return str1[i] - str2[i];

		i++;
	}

	if(str1[i] == NULL && str2[i] != NULL)
		return -str2[i];
	else if( str1[i] != NULL && str2[i] == NULL)
		return str1[i];

	return out;
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
	putc(((hours & 0xF0)>>4) + '0');
	putc((hours & 0xF) + '0');
	putc(':');

	putc(((minutes & 0xF0)>>4) + '0');
	putc((minutes & 0xF) + '0');
	putc(':');

	putc(((seconds & 0xF0)>>4) + '0');
	putc((seconds & 0xF) + '0');
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
