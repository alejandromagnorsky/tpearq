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

int __shift_history(int direction){

}

int cpuid(int argc, char * argv[]){
	detect_cpu();
}


int moveCursorToStart(int i){
	__shift_terminal_cursor(-1,i);
	return 0;
}


int moveCursorToEnd(char * ans, int i){
	int j = 0;	
	int tmp = i;
	while(ans[tmp] != '\0'){ j++;tmp++;}
	__shift_terminal_cursor(1,j);
	return tmp;
}

/* This is a more restricted version of scanf,
 * oriented towards shell commands, history, etc.
*/
void getShellArguments(char * ans){
        int i = 0;
        char c;

	// Initialize ans
        for (i=0; i < MAX_ARGUMENT_LENGTH*MAX_ARGUMENTS ; i++)
		ans[i] = '\0';

	i = 0;
	while( (c = getchar()) != '\n' ){
		switch( c ){
			case '\b':
				if (i)     
	                        	ans[--i] = ' ';
				printf("%c",c);
				break;
			case  (char) 204:// RIGHT ARROW
				 if (i<MAX_ARGUMENT_LENGTH*MAX_ARGUMENTS + 1){

					if(ans[i] != '\0'){					
						++i;
						__shift_terminal_cursor(1,1);
					}
				} 
				break;
			case  (char) 185:// LEFT ARROW
				if(i){
					--i;
					__shift_terminal_cursor(-1,1);
				}
				break;
			case  (char) 202:// UP ARROW
				i = moveCursorToStart(i);
				break;
			case  (char) 203:// DOWN ARROW
				i = moveCursorToEnd(ans,i);
				break;
			default:
				 if (i<MAX_ARGUMENT_LENGTH*MAX_ARGUMENTS + 1)
			                ans[i++] = c;
		                putchar(c);
				break;
		}
        }
	i = moveCursorToEnd(ans,i);
	printf("\n",ans);
}


void shell(){

	__QTY_PROGRAMS = 0;
	__QTY_HISTORY_STATES = 0;
	__ACTUAL_HISTORY_STATE = 0;

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
	__register_program("reboot", reboot);

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
	__register_man_page("arnold","Arnold Alois Schwarzenegger, as John Matrix in Commando(1985)");
	__register_man_page("mkexc","Generates the exception corresponding to the second argument." \
				     "Valid values are numbers between 0 and 31.");
	__register_man_page("bingo","Bingo para dos jugadores");
	__register_man_page("reboot","Reinicia el sistema");
	

	// Data for user input
	char user_input[MAX_ARGUMENT_LENGTH*MAX_ARGUMENTS + 1];
	int i;
	// Main loop
	while(1){


		getShellArguments(user_input);

		char arg_data[MAX_ARGUMENTS][MAX_ARGUMENT_LENGTH];
		char argc = 0;
		int tmp = 0;

		// Get arguments, separated by ' '
		for(i=0;user_input[i] != NULL && argc < MAX_ARGUMENTS && tmp < MAX_ARGUMENT_LENGTH;i++){
			if(user_input[i] == ' '){
				if(tmp) // This way a string "     " is not considered as 6 arguments
					arg_data[argc++][tmp] = NULL;
				tmp = 0;
			} else 	arg_data[argc][tmp++] = user_input[i];
		}
		arg_data[argc++][tmp] = NULL;	// Last argument
		
		if (user_input[i] != NULL)
			printf("Error: argument too long or too much arguments: %d.\n", argc);
		else {
			// Convert data to pointer
			char * argv[MAX_ARGUMENTS] = { 0 };
			for(i=0;i<argc;i++)
				argv[i] = arg_data[i];
	
			__executable * exec = getExecutableByDescriptor(argv[0]);
			if(exec != NULL)
				exec->execute(argc, argv);
			else if(user_input[0] != NULL)
				printf("Error: invalid command. \n");
		}
		__printSystemSymbol();
	}
}
