#include "common.h"

#include <unistd.h>

void load_table();//load the table of string and symbol table
void main_loop();//loop of UI: implemetation is in src/ui/ui.c
void set_main_args(int, char * []);
void init_bp_pool();//initialization of the breakpoint pool
void wp_pool_init();// @see debugger/wp_pool.c
void init_regex();
void init_signal();//set the signal of 'Ctrl+C'
void reg_test();

int enable_debug = false;
int quiet = false;

static void process_args(int argc, char *argv[]) {//the work like terminal: find out the parameters 'gcc -O1 -S'
	int opt;
	while( (opt = getopt(argc, argv, "dq")) != -1) {
		switch(opt) {
			case 'd':
				enable_debug = true;
				break;
			case 'q':
				quiet = true;
				break;
			default:
				test(0, "bad option = %s\n", optarg);
				break;
		}
	}

	test(argc > optind, "Program is not given\n");

	set_main_args(argc - optind, argv + optind);
}

int main(int argc, char *argv[]) {
	process_args(argc, argv);

	/* Perform some global initialization */
	init_regex();
	init_signal();
	init_bp_pool();
    wp_pool_init();
	load_table();


	/* Test whether the 'CPU_state' structure is organized correctly. */
	reg_test();
	//code is in the src/ui/ui.c
	main_loop();

	return 0;
}
