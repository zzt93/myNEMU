#include "ui/ui.h"

#include "cpu/int_flag.h"
#include "elf/loader.h"
#include "nemu.h"

#include <setjmp.h>

#define LOADER_START 0x100000
#define P_MEM_START 0x00
#define P_MEM_END 0x8000000

extern int quiet;

int exec(swaddr_t);

void init_dram();

void set_bp_in_mem(swaddr_t);
bool check_wp();

char assembly[40];
jmp_buf jbuf;	/* Make it easy to perform exception handling */

Int_state state_interrupt = NO;


void restart() {
	/* Perform some initialization to restart a program */
	load_prog();//where--
	memcpy(hwa_to_va(LOADER_START), loader, loader_len);

	cpu.eip = LOADER_START;//the program start from 0x100000
    cpu.ebp = P_MEM_START;
    cpu.esp = P_MEM_END;

	init_dram();//function? where?
}

static void print_bin_instr(swaddr_t eip, int len) {
	int i;
	printf("0x%-8x:   ", eip);
	for(i = 0; i < len; i ++) {
		printf("%02x ", swaddr_read(eip + i, 1));
	}
	printf("%*.s", 50 - (12 + 3 * len), "");
}

void cpu_exec(volatile uint32_t n) {
	volatile uint32_t n_temp = n;

	setjmp(jbuf);
	for(; n > 0; n --) {
		swaddr_t eip_temp = cpu.eip;
		int instr_len = exec(cpu.eip);//where -- exec/exec.c

        switch (state_interrupt) {
            case NO:
                break;
            case INT3:
                state_interrupt = AFTER_INT3;
                return;
            case AFTER_INT3:
                state_interrupt = NO;
                set_bp_in_mem(cpu.eip);
                break;
            default:
                puts("impossible");
                assert(0);
        }

		cpu.eip += instr_len;

		if(n_temp != -1 || (enable_debug && !quiet)) {
			print_bin_instr(eip_temp, instr_len);
			puts(assembly);
		}

        if (check_wp()) {
            return;
        }
		if(nemu_state == INT) {//Crtl-c signal
			printf("\n\nUser interrupt\n");
			return;
		}
		else if(nemu_state == END) { return; }//NEMU-trap??
	}
}
