#include "../include/shell.h"


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

int echo(int argc, char * argv[]){

	int i;
	for(i=1;i<argc;i++)
		printf("%s ", argv[i]);
	printf("\n");
	return 0;
}

int clear(int argc, char * argv[]){
	__clear_terminal();
	return 0;
}

int help(int argc, char * argv[]){
	printf("Possible commands: \n");

	int i;
	for(i=0;i<__QTY_PROGRAMS;i++)
		printf("%s\n", __executable_programs[i].descriptor);
	
	return 0;
}

int man(int argc, char * argv[]){
	if(argc >= 1){
		int i, exists = 0;

		__executable * tmp = getExecutableByDescriptor(argv[1]);
		if(tmp!=NULL){
			printf("%s: %s \n", tmp->descriptor, tmp->man);
			return 0;
		} else {
			printf("Invalid argument. \n");
			return 1;
		}
	}	
	printf("Please enter a program name. \n");
	return 1;
}

int gcc(int argc, char * argv[]){
	printf("It's a joke! No gcc here. \n");
}

int arnold(int argc, char * argv[]){
	int random = rand()%32;
	switch(random){
		case 0:
			printf("- 'Vete al demonio'\n- 'Ni lo sueñes'\n");
			break;
		default:
			printf("\n");
			break;
	}
	return 1;
}

/* Prints hour, minutes and seconds */
int time(int argc, char * argv[])
{
	_outport(0x70, 4);
	int hours = _inport(0x71);
	putc(((hours & 0xF0)>>4) + '0');
	putc((hours & 0xF) + '0');
	putc(':');

	_outport(0x70, 2);
	int minutes = _inport(0x71);
	putc(((minutes & 0xF0)>>4) + '0');
	putc((minutes & 0xF) + '0');
	putc(':');
	
	_outport(0x70, 0);
	int seconds = _inport(0x71);
	putc(((seconds & 0xF0)>>4) + '0');
	putc((seconds & 0xF) + '0');
	putc('\n');
	return 0;
}

int tty(int argc, char * argv[]){

	// NECESITO UN PARSER DE INTS
	if(argc > 0 ){

		if(!strcmp(argv[1], "-c") && argv[2] != NULL && argv[3] != NULL)
			__change_color(*argv[2] - '0',*argv[3] - '0');

		if(!strcmp(argv[1], "-s") && argv[2] != NULL)
			__switch_terminal(*argv[2] - '0');

		if(!strcmp(argv[1], "-n"))
			__switch_next_terminal();

		if(!strcmp(argv[1], "-l"))
			__switch_last_terminal();

		if(!strcmp(argv[1], "-ss") && argv[2] != NULL)
			__changeSystemSymbol(argv[2]);



		return 0;
	}

	printf("Invalid arguments. \n");
	return 1;
}

void shell(){

	// Register of various functions
	__register_program("echo", echo);
	__register_program("clear", clear);
	__register_program("help", help);
	__register_program("man", man);
	__register_program("gcc", gcc);
	__register_program("tty", tty);
	__register_program("time", time);
	__register_program("arnold", arnold);

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
	__register_man_page("time","Imprime la hora, los minutos y los segundos.");
	__register_man_page("arnold","Date un baño de vapor, Bennet!");

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
		char * argv[MAX_ARGUMENTS];
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
