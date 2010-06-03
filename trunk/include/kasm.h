/*********************************************
kasm.h

************************************************/

#include "defs.h"


unsigned int    _read_msw();

void            _lidt (IDTR *idtr);

void		_mascaraPIC1 (byte mascara);  /* Escribe mascara de PIC1 */
void		_mascaraPIC2 (byte mascara);  /* Escribe mascara de PIC2 */

void		_Cli(void);        /* Deshabilita interrupciones  */
void		_Sti(void);	 /* Habilita interrupciones  */

void		_print_time();

void		_int_08_hand();      /* Timer tick */
void		_int_09_hand();	     /* Teclado */

void 		_turn_cursor_on();
void 		_turn_cursor_off();
void		_move_cursor(unsigned int pos);

void		_debug (void);
