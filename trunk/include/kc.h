/**********************
 kc.h
**********************/
#include "defs.h"

#ifndef _kc_
#define _kc_

#define WHITE_TXT 0x07 // Atributo de video. Letras blancas, fondo negro

int __CONSOLE_PTR_ = 0;
int __KBUFFER_PTR_ = 0x1E;	//Puntero al buffer del teclado

/* Muestra la imagen de inicio */
void showSplashScreen();

/* Imprime un caracter en pantalla */
int putc( int character );

/* Imprime un string en pantalla */
int printf(const char * str);

/* Tiempo de espera */
void wait(int time);

/* Limpia la pantalla */
void k_clear_screen();

/* Imprime el símbolo de pantalla */
void printSystemSymbol();

/* Imprime horas, minutos y segundos en la pantalla */
void printTime();


/* Inicializa la entrada del IDT */
void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero);

#endif
