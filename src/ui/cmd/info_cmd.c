#include <stdlib.h>
#include "ui/ui.h"
#include "nemu.h"
#include "ui/debugger/bp_pool_interface.h"
#include "ui/debugger/sw_bp.h"
#include "ui/debugger/wp_pool.h"

uint32_t expr(char *, bool *);//expr.c

void cmd_info_r() {
    if(nemu_state == END) {
		puts("The Program does not start. \
Use 'r' command to start the program.");
        return;
	} else if(nemu_state == STOP) {
        puts("The program is now stopped");
        //print eax, ecx, edx, ebx, esp, ebp, esi, edi, cs, ss, ds, es, fs
        printf("eax\t0x%x %d\n", cpu.eax, cpu.eax);
        printf("ecx\t0x%x %d\n", cpu.ecx, cpu.ecx);
        printf("edx\t0x%x %d\n", cpu.edx, cpu.edx);
        printf("ebx\t0x%x %d\n", cpu.ebx, cpu.ebx);
        printf("esp\t0x%x %d\n", cpu.esp, cpu.esp);
        printf("ebp\t0x%x %d\n", cpu.ebp, cpu.ebp);
        printf("esi\t0x%x %d\n", cpu.esi, cpu.esi);
        printf("edi\t0x%x %d\n", cpu.edi, cpu.edi);
        /*
          printf("cs\t%x %d\n", cpu.cs, cpu.cs);
          printf("ss\t%x %d\n", cpu.ss, cpu.ss);
          printf("ds\t%x %d\n", cpu.ds, cpu.ds);
          printf("fs\t%x %d\n", cpu.fs, cpu.fs);
        */
        fflush(stdin);
        return;
    }

}

static bool state_check(){
    if(nemu_state == END) {
		puts("The Program does not start. \
Use 'r' command to start the program.");
		return false;
	}

    if(nemu_state == INT) {
        puts("The program is now interrupted.");
        return false;
    }
    return true;
}

static int count_of_p = 0;
void cmd_p(char *s){
    if(!state_check()){
        return;
    }
    //only print when STOP
    //print the expression
    bool su = true;
    uint32_t res = expr(s, &su);
    if (su) {
        printf("$%d = %u\n", ++count_of_p, res);
    } else {
        puts("Fail to parse the expression");
    }

}

void cmd_b(char *s){
    if(nemu_state == INT){
        puts("The program is now interrupted.");
        return;
    }
    //calculate the expression
    bool su = true;
    uint32_t res = expr(s, &su);
    if (!su) {
        puts("Fail to parse the expression: it should be an address");
        return;
    }
    BP* this = add_bp_pool(res);
    set_bp_in_mem(res);
    printf("set breakpoint #%d, old opcode: 0x%2x\n", this->NO, this->content);
}

void cmd_d(char *s){
    if(!state_check()){
        return;
    }
    bool su = true;
    uint32_t res = expr(s, &su);
    if (su) {
        Error err = free_byid(res);
        switch (err.id) {
            case SUCCESS:
                printf("Succeed deleting #%s breakpoint\n", s);
                break;
            case OUT_RANGE:
            case NULL_POINTER:
            case INVALID:
                printf("%s, your input %s\n", err.str, s);
                break;
            default:
                puts("how could it be");
                assert(0);
                break;
        }
    } else {
        puts("Fail to parse the expression");
    }

}

/* FIXME:  how to disguise the change of INT3*/
void cmd_x(int block, char *s){
    test(block > 0 && s != NULL, "block: %d, s: %p", block, s);
    if(!state_check()){
        return;
    }
    //only scan when STOP
    //calculate the expression
    bool su = true;
    uint32_t res = expr(s, &su);
    if (su) {
        int i = 0;
        for (;i < block; ++i) {//print every byte
            if (i%4 == 0){
                printf("0x%02x: 0x", res+i);
            }
            uint8_t mem = (uint8_t)swaddr_read(res+i, 1);
            printf("%02x", mem);
            if ((i+1)%4 == 0){
                puts("");
            }
        }
        puts("");
    } else {
        puts("Fail to parse the leading address");
    }
}

void cmd_w(char* ex){
    if(nemu_state == INT){
        puts("The program is now interrupted.");
        return;
    }
    if (ex == NULL) {
        puts("Invalid expression");
        return;
    }

    WP* this = add_wp_pool(ex);
    if (this == NULL) {
        puts("Invalid expression");
        return;
    }

    printf("set watchpoint #%d, watch expression: %s\n", this->NO, ex);
}

void cmd_dw(char* num){
    if(!state_check()){
        return;
    }
    uint32_t id = atoi(num);
    if (id < 0 || id > NUM_WP || del_by_id(id)) {
        printf("Invalid number %u\n", id);
    }
}

void cmd_info_b(){
    print_all_bp();
    print_all_wp();
}