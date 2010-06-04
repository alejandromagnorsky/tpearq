#ifndef _keyboard_
#define _keyboard_

/* Recives an scancode and return his ASCII value
** If is a break code, returns -1
*/
int getAscii(int scanCode);

/*
** Returns 1 if the scanCode is a letter (includes ñ) or 0 otherwise
*/
char isLetter(int scanCode);

/*
** Returns 1 if the scanCode is a number or a ".", or 0 otherwise
*/
char isNumber(int scanCode);
#endif
