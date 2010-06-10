#include "../include/keyboard.h"
#include "../include/kasm.h"
/*      PROBLEMAS
**      TECLAS QUE NO TIRAN INTERRUPCIÓN:
**              AltGr.  
**      AVERIGUAR EL SCAN CODE DE
**              La tecla de al lado de la Ñ
**              Shift+| y |
**              < y >
**              }
**      AVERIGUAR QUE HACER CON LAS TECLAS INSERT, INICIO, SUPR y FIN cuando el numPad esta activado
**              
*/

/* Globales para los LEDS */
int __CURRENT_LEDS = 0;
int __SCROLL_LED = 1;
int __NUM_LED = 2;
int __CAPS_LED = 4;

int __L_SHIFT_ = 0;     // Left Shift flag (0 = disabled)
int __R_SHIFT_ = 0; // Right Shift flag (0 = disabled)
int __CAPSLOCK_ = 0;    // Capslock flag (0 = disabled)
int __NUMLOCK_ = 0; // NumLock flag (0 = disabled)
int __TILDE_ = 0; // Tilde flag (0 = disabled)
int __EXTENDED_ = 0;


int makeCodes[83] =	 {-1, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 39, 168, '\b', '\t', 'q', 'w', 'e', 'r', 
			  't', 'y', 'u', 'i', 'o', 'p', -5, '+', '\n', -1, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 
                          164, '{', '}', 124, -5, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', -5, -5, -1, ' ', -5,
			  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 'N', 'S', 'H', 202, -1, '-', 185, 206, 204, '+', 'E',
			  203, -1, 'I', 'D'};

int shiftMakeCodes[83] = {-1, '!', '"', '#', '$', '%', '&', '/', '(', ')', '=', '\?', 173, '\b', '\t', 'Q', 'W', 'E', 'R', 
			  'T', 'Y', 'U', 'I', 'O', 'P', -5, '*', '\n', -1, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 
			  165, '[', ']', 167, -5, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', -5, -5, -1, ' ', -5,
			  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 'N', 'S', 'H', 202, -1, '-', 185, 206, 204, '+', 'E',
			  203, -1, 'I', 'D'};

int numpadMakeCodes[13] = {'7', '8', '9', -1, '4', '5', '6', -1, '1', '2', '3', '0', '.'};

int extendedMakeCodes[83] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			     -1, -1, -1, -1, '\n', -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			     -1, -1, -1, -1, -1, -1, -1, '/', -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			     -1, -1, -1, 'H', 202, -1, '-', 185, 206, 204, -1, 'E', 203, -1, 'I', 'D'};

int getAscii(int scanCode){
	int asciiCode = -1;
	scanCode = scanCode & 0xFF;
	
	//For scanCodes with more than 1 byte
	if(__EXTENDED_ != 0){
		__EXTENDED_ = 0;
		if(scanCode <= 83)
			asciiCode = extendedMakeCodes[scanCode-1];
	} else if(scanCode == 0xE0) 
		__EXTENDED_ = 1;

        else if (scanCode == 170)
                __L_SHIFT_ = 0;  
        else if(scanCode == 182)
                __R_SHIFT_ = 0;
        // If the scanCode is a make code
        else if (scanCode <= 128){
                if(__TILDE_){
                        if(isVocal(scanCode))
                                asciiCode = getTildeVocal(scanCode);
                        __TILDE_ = 0;
                } else if(scanCode == 40){
                        __TILDE_ = 1;
                } else if(scanCode == 42)
                        __L_SHIFT_ = 1;
                else if(scanCode == 54)
                        __R_SHIFT_ = 1;
                else if (scanCode == 58){
                        __CURRENT_LEDS += __CAPS_LED;
                        keyboardLeds(__CURRENT_LEDS);
                        __CAPS_LED *= -1;
                        __CAPSLOCK_ = !__CAPSLOCK_;
                }
                else if(scanCode == 69){
                        __CURRENT_LEDS += __NUM_LED;
                        keyboardLeds(__CURRENT_LEDS);
                        __NUM_LED *= -1;
                        __NUMLOCK_ = !__NUMLOCK_;               
                }else if (__CAPSLOCK_ && isLetter(scanCode) ){
                        if (!__L_SHIFT_ && !__R_SHIFT_)
                                asciiCode = shiftMakeCodes[scanCode-1];
                        else
                                asciiCode = makeCodes[scanCode-1];
                }else if( (__L_SHIFT_ || __R_SHIFT_ || __NUMLOCK_) && isNumber(scanCode))
                        asciiCode = numpadMakeCodes[scanCode-71];
                else if(__L_SHIFT_ || __R_SHIFT_)
                        asciiCode = shiftMakeCodes[scanCode-1];
                
                else
                        asciiCode = makeCodes[scanCode-1];
        }
        return asciiCode;
}

char isLetter(int scanCode){
        return (makeCodes[scanCode-1] <= 'z' && makeCodes[scanCode-1] >= 'a') 
                 || makeCodes[scanCode-1] == 164;
}

char isNumber(int scanCode){
        return (scanCode >= 71 && scanCode <= 73) 
                 || (scanCode >= 75 && scanCode <= 77)
                 || (scanCode >= 79 && scanCode <= 83);
}

char isVocal(int scanCode){
        return (makeCodes[scanCode-1] == 'e' || makeCodes[scanCode-1] == 'u' || 
                makeCodes[scanCode-1] == 'i' || makeCodes[scanCode-1] == 'o' 
                || makeCodes[scanCode-1] == 'a');
}

char getTildeVocal(int scanCode){
        int ans;
        switch(makeCodes[scanCode-1]){
                case 'e':
                        ans = 130;
                        break;
                case 'u':
                        ans = 163;
                        break;
                case 'i':
                        ans = 161;
                        break;
                case 'o':
                        ans = 162;
                        break;
                case 'a':
                        ans = 160;
        }
        if(__CAPSLOCK_ || __L_SHIFT_ || __R_SHIFT_)
                ans = -1;
        return ans;
}

void keyboardLeds(int leds){
        while ( (_inport(0x64) & 2) != 0); //DESPUES COMENTAR BIEN: while (el buffer del teclado está lleno (bit 1 != 0))
        _outport(0x60, 0xED);   //Envío el comando ED que se prepara para recibir los bits de activación de LED
        while ( (_inport(0x64) & 2) != 0); //DESPUES COMENTAR BIEN: while (el buffer del teclado está lleno (bit 1 != 0))
        _outport(0x60, leds);
}

/* BORRAR SEGURAMENTE, ANDUVO COMO EL ORTO EN UNA PC REAL
** TODAVIA NO LA BORRO POR LAS DUDAS
*/
void fireWorks(){
        int i;
        for (i=0; i<10000; i++){
                if (i%4 == 3)
                        keyboardLeds(i+1);
                else
                        keyboardLeds(i);
        }
        for (i=0; i<10000; i++);
                keyboardLeds(7);
        for (i=0; i<5000; i++);
                keyboardLeds(0);
        for (i=0; i<10000; i++);
                keyboardLeds(7);
        for (i=0; i<5000; i++);
                keyboardLeds(0);
}

