/*********************************************
kasm.h

************************************************/

#include "defs.h"

#ifndef _kasm_
#define _kasm_
unsigned int    _read_msw();

void            _lidt (IDTR *idtr);

void		_mascaraPIC1 (byte mascara);  /* Escribe mascara de PIC1 */
void		_mascaraPIC2 (byte mascara);  /* Escribe mascara de PIC2 */

void		_Cli(void);        /* Deshabilita interrupciones  */
void		_Sti(void);	 /* Habilita interrupciones  */

void		_int_20_hand();      /* Timer tick */
void		_int_21_hand();	     /* Teclado */

int		_read_scancode();
int		_inport(dword port);
void 		_outport(dword port, dword data);

void 		_turn_cursor_on();
void 		_turn_cursor_off();
void		_move_cursor(unsigned int pos);
void		_out_vga(int port, int value);

void		_debug (void);
#endif
