#ifndef _shell_
#define _shell_

#include "kc.h"

#define MAX_PROGRAMS 32
#define MAX_ARGUMENT_LENGTH 16
#define MAX_ARGUMENTS 8

typedef struct{
	char * descriptor;				// The name
	int (*execute)(int argc, char * argv[]);	// The executable
	char * man;				// The manual page 
} __executable;

__executable __executable_programs[MAX_PROGRAMS];
int __QTY_PROGRAMS = 0;


int __register_program(char * descriptor, int (*program)(int argc, char * argv[]));

void shell();


#endif
