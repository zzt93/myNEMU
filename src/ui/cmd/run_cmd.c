#include <stdio.h>
#include "common.h"
#include "ui/ui.h"

void cpu_exec(uint32_t);
void restart();//initialize where--src/cpu/cpu-exec.c
void reset_all_in_mem();// -- in sw_bp.c


//enum { END, STOP, RUNNING, INT -- interrupt};
int nemu_state = END;

void cmd_c() {
	if(nemu_state == END) {
		puts("The Program does not start. \
Use 'r' command to start the program.");
		return;
	}

    //STOP, INT then RUNNING
	nemu_state = RUNNING;
	cpu_exec(-1);//for the prototype of it is 'volatile uint32_t', the -1 become the UINT_MAX
	if(nemu_state != END) { nemu_state = STOP; }
}

void cmd_r() {
	if(nemu_state != END) {//STOP or INT is already running
		char c;
		while(1) {
			printf("The program is already running. Restart the program? (y or n)");
			scanf("%c", &c);
			switch(c) {
				case 'y': goto restart_;
				case 'n': return;
				default: puts("Please answer y or n.");
			}
		}
	}

restart_:
	restart();	//initialize? where--src/cpu/cpu-exec.c
    reset_all_in_mem();
    nemu_state = STOP;
	cmd_c();
}

void cmd_si(int i) {
    if(nemu_state == END) {
		puts("The Program does not start. You can still use this command or use 'r' command to start the program.");
		nemu_state = RUNNING;
        restart();
	}

    printf("%d instructions\n", i);
	cpu_exec(i);
	if(nemu_state != END) {
        nemu_state = STOP;
    }
}

