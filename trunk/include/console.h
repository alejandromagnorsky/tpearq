#include "../include/video.h"

#define __MAX_TERMINALS 10
#define __TAB_LENGTH 4
#define __BLOCK_ASCII -10


typedef struct{
	char buf[80*25];
	int ptr;
	int attr;
	int empty;
} __terminal;


// Console buffers
__terminal __tty[__MAX_TERMINALS];

// Index of visible console
int __TTY_INDEX = 0;



void __scroll_terminal();

void __printSystemSymbol();

void __INIT_TTY();

void __clear_terminal();

int __write_terminal( const char* buffer, int count);

void __flush_terminal(int append);

int __switch_terminal(int index);

void __switch_next_terminal();

void __switch_last_terminal();

