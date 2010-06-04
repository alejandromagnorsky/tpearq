#include "../include/kernel.h"
#include "../include/kc.h"

int getc(){
	int character = -1;
	while (character == -1)
		_int_80_hand(0, stdin, &character, 1);
	return character;
	
}

void getString(char * str){
	int i;
	char c;
	char * ret;
	for (i=0; (c = getc()) != '\n' ; i++){
		printf("%c", c);	/* PRINTF  DE DEBUGGEO */
		str[i] = c;
	}
	str[i] = '\0';
	str = ret;
	printf("\n");	/* PRINTF  DE DEBUGGEO */
}

int scanf(const char * str, ...){
	int i, j, k, exponents, acum = 0, strLen, strTrueLen, strInLen;
	/*	strLen:		longitud de la string str.
	**	strTrueLen:	longitud de la string str una vez reemplazados los % por los valores ingresados por el usuario.
	**	strInLen:	longitud de la string strIn, ingresada por el usuario
	*/
	int retFlag = 0;	/* retFlag = 0 --> string inválida. reFlag = 1 --> string válida */
	char * strIn;
	void ** argv = (void **)(&str);
	
	getString(strIn); /* El usuario ingresa una string */
	strLen = strlen(str);
	strTrueLen = strLen;	/* Inicialmente la string real tiene la misma longitud que la string del scanf */
	strInLen = strlen(strIn);

	for (i=0, j=0; i<strLen && j<strInLen; i++, j++){
		if ( str[i] == '%' )
			switch(str[++i]){
				case 'd':					/* VER SI PUEDO ARREGLAR LA CHANCHADA ESTA */
					for(k=0; isDigit(strIn[i-1-k]); k++, j++)	/* Primer for para contar k . j sincroniza strings str y strIn*/
					exponents = k;
					printf("K: %d", k);	//PRINTF DE  DEBUGGEO//
					for(k=0; isDigit(strIn[i-1-k]); k++)
						acum = acum + ((strIn[i-1-k]-'0') * pow(10, exponents-k-1) );
					printf("ACUM: %d|||", acum);	//PRINTF DE  DEBUGGEO//
					*((int *)++argv) = acum;
					strTrueLen = strTrueLen - 2 + exponents + 1;	/* Resto 2 a strTrueLen (% y d) y sumo exp+1 (cant. dígitos leidos)*/
					break;
				case 'c':
					printf("ACUM: %c", strIn[i-1]);
					*((int *)++argv) = strIn[i-1];
					strTrueLen = strTrueLen - 2 + 1;
					break;
				case 's':			/* FIJARME BIEN COMO REACCIONAR CON STRINGS */
					break;
				default:
					i--;
					strTrueLen--;
					break;
			}
		else
			if (str[i] != strIn[j]){
				printf("%d", -1);	/* PRINTF  DE DEBUGGEO */
				return -1;
			}
	}
	if (strInLen != strTrueLen){
		printf("%d", -1);	/* PRINTF  DE DEBUGGEO */
		return -1;
	}
	printf("%d", 0);	/* PRINTF  DE DEBUGGEO */
	return 0;
}

int isDigit(int a){
	if( (a >= '0') && (a <= '9') )
		return 1;
	return 0;
}

int strlen(const char * str){
	int i;
	for (i = 0; str[i] != '\0'; i++);
		return i;
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
