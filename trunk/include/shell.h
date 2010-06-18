#ifndef _shell_
#define _shell_

#include "kc.h"

#define MAX_PROGRAMS 32
#define MAX_ARGUMENT_LENGTH 32
#define MAX_ARGUMENTS 8
#define MAX_HISTORY 10

typedef struct{
	char * descriptor;				// The name
	int (*execute)(int argc, char * argv[]);	// The executable
	char * man;				// The manual page 
} __executable;

__executable __executable_programs[MAX_PROGRAMS];
char history[MAX_ARGUMENT_LENGTH*MAX_ARGUMENTS + 1][MAX_HISTORY];
int __QTY_PROGRAMS;
int __QTY_HISTORY_STATES;
int __ACTUAL_HISTORY_STATE;

int __register_program(char * descriptor, int (*program)(int argc, char * argv[]));

__executable * getExecutableByDescriptor(char * descriptor);

int __register_man_page(char * descriptor, char * man);

int __shift_history(int direction);

void shell();


#endif
