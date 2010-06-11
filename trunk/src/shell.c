#include "../include/shell.h"
#include "../include/interrupts.h"
#include "../include/programs.h"

int __register_program(char * descriptor, int (*execute)(int argc, char * argv[])){
	__executable exec;
	exec.descriptor = descriptor;
	exec.execute = execute;
	exec.man = "No manual available.";

	if(__QTY_PROGRAMS < MAX_PROGRAMS){
		__executable_programs[__QTY_PROGRAMS++] = exec;
		return 1;
	}
	else return 0;
}

__executable * getExecutableByDescriptor(char * descriptor){

	int i;
	__executable * out = NULL;
	// Search program
	for(i=0;i<__QTY_PROGRAMS;i++)
		if(! strcmp(descriptor, __executable_programs[i].descriptor))
			out = __executable_programs+i;

	return out;
}

int __register_man_page(char * descriptor, char * man){

	__executable * tmp = getExecutableByDescriptor(descriptor);
	if( tmp == NULL )
		return 1;
	tmp->man = man;
	return 0;
}



int cpuid(int argc, char * argv[]){
	detect_cpu();
}

void shell(){
	__QTY_PROGRAMS = 0;
	// Register of various functions
	__register_program("echo", echo);
	__register_program("clear", clear);
	__register_program("help", help);
	__register_program("man", man);
	__register_program("gcc", gcc);
	__register_program("tty", tty);
	__register_program("time", time);
	__register_program("arnold", arnold);
	__register_program("mkexc", mkexc);
	__register_program("cpuid", cpuid);
	__register_program("bingo", bingo);

	__register_man_page("echo","Prints the string received.");
	__register_man_page("clear", "Clears the screen.");
	__register_man_page("help", "Prints all the possible commands known.");
	__register_man_page("man", "Shows the manual for any program");
	__register_man_page("gcc", "GNU C Compiler.");
	__register_man_page("tty", "Interface for changing terminal settings. \n"  \
				"Arguments: \n" \
				" \t [-s terminal_index] | _Switches terminal \n"
				" \t [-l] | Switches to the _last terminal \n"
				" \t [-n] | Switches to the _next terminal \n"
				" \t [-ss string] | Changes the _system _symbol to string \n"
				" \t [-c foreground background] | Changes terminal _color.");
	__register_man_page("time","Prints hour, minutes and seconds.");
	__register_man_page("arnold","Date un baño de vapor, Bennet!");
	__register_man_page("mkexc","Generates the exception corresponding to the second argument." \
				     "Valid values are numbers between 0 and 31.");
	__register_man_page("bingo","Bingo para dos jugadores");
	

	// Data for user input
	char user_input[MAX_ARGUMENT_LENGTH*MAX_ARGUMENTS];
	int i;
	// Main loop
	while(1){

		user_input[0] = NULL;
		scanf("%s", user_input);

		char arg_data[MAX_ARGUMENTS][MAX_ARGUMENT_LENGTH];
		char argc = 0;
		int tmp = 0;

		// Get arguments, separated by ' '
		for(i=0;user_input[i] != NULL;i++){
			if(user_input[i] == ' '){
				arg_data[argc++][tmp] = NULL;
				tmp = 0;
			} else 	arg_data[argc][tmp++] = user_input[i];
		}
		arg_data[argc++][tmp] = NULL;	// Last argument

		// Convert data to pointer
		char * argv[MAX_ARGUMENTS] = {0 };
		for(i=0;i<argc;i++)
			argv[i] = arg_data[i];

		__executable * exec = getExecutableByDescriptor(argv[0]);
		if(exec != NULL)
			exec->execute(argc, argv);
		else if(user_input[0] != NULL)
			printf("Error: invalid command. \n");
		else printf("\n");

		__printSystemSymbol();

	}
}