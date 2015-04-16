#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>

#include "utility/input_handler.h"

/* We use the readline library to provide more flexibility to read from stdin. */
char* get_pure_input() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {//line_read is not NULL and content in it is not '\0'
		add_history(line_read);
	}

	return line_read;
}

char* skip_heading_space(char* in){
    while (isspace(*in)) {
        in++;
    }
    return in;
}

char* get_input(){
    return skip_heading_space(get_pure_input());
}