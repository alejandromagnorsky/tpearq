#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kernel.h"
#include "../include/keyboard.h"
#include "../include/interrupts.h"

void int_00_hand(){
	printf("Error al dividir por cero.\n");
}

void int_01_hand(){
	printf("Reserved.\n");
}

void int_02_hand(){
	printf("Reserved.\n");
}

void int_03_hand(){
	printf("Reserved.\n");
}

void int_04_hand(){
	printf("Reserved.\n");
}

void int_05_hand(){
	printf("Reserved.\n");
}

void int_06_hand(){
	printf("Reserved.\n");
}

void int_07_hand(){
	printf("Reserved.\n");
}

void int_08_hand(){
	printf("Reserved.\n");
}

void int_09_hand(){
	printf("Reserved.\n");
}

void int_0A_hand(){
	printf("Reserved.\n");
}

void int_0B_hand(){
	printf("Reserved.\n");
}

void int_0C_hand(){
	printf("Reserved.\n");
}

void int_0D_hand(){
	printf("Reserved.\n");
}

void int_0E_hand(){
	printf("Reserved.\n");
}

void int_0F_hand(){
	printf("Reserved.\n");
}

void int_10_hand(){
	printf("Reserved.\n");
}

void int_11_hand(){
	printf("Reserved.\n");
}

void int_12_hand(){
	printf("Reserved.\n");
}

void int_13_hand(){
	printf("Reserved.\n");
}

void int_14_hand(){
	printf("Reserved.\n");
}

void int_15_hand(){
	printf("Reserved.\n");
}

void int_16_hand(){
	printf("Reserved.\n");
}

void int_17_hand(){
	printf("Reserved.\n");
}

void int_18_hand(){
	printf("Reserved.\n");
}

void int_19_hand(){
	printf("Reserved.\n");
}

void int_1A_hand(){
	printf("Reserved.\n");
}

void int_1B_hand(){
	printf("Reserved.\n");
}

void int_1C_hand(){
	printf("Reserved.\n");
}

void int_1D_hand(){
	printf("Reserved.\n");
}

void int_1E_hand(){
	printf("Reserved.\n");
}

void int_1F_hand(){
	printf("Reserved.\n");
}

void int_20() {

}

void int_21(){
	int scanCode = _read_scancode();
	int asciiCode = getAscii(scanCode);
	
	if(asciiCode != -1){
		kbuffer.buf[kbuffer.__KBUFFER_PTR_WR] = asciiCode;
		kbuffer.__KBUFFER_PTR_WR = (kbuffer.__KBUFFER_PTR_WR+1) % KBUFFER_SIZE;	
	}
}

