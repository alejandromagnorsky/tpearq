/**********************
 kc.h
**********************/
#include "defs.h"

#ifndef _kc_
#define _kc_

#define NULL 0
#define MAX_STRLEN 64

/* Muestra la imagen de inicio */
void showSplashScreen();

/* Imprime un caracter en pantalla */
int putc( int character );

/* Imprime un string en pantalla */
int printf(const char * str,...);

/* Compara dos strings */
int strcmp(const char * str1, const char * str2);

/* Imprime un numero */
void putInt(int n);

/* Tiempo de espera */
void wait(int time);

/* Limpia la pantalla */
void k_clear_screen();

/* Imprime el símbolo de pantalla */
void printSystemSymbol();

/* Imprime horas, minutos y segundos en la pantalla */
void printTime(dword hours, dword minutes, dword seconds);


/* Inicializa la entrada del IDT */
void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero);

#endif
