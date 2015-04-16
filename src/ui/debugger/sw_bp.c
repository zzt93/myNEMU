#include "ui/debugger/sw_bp.h"

void set_bp_in_mem(swaddr_t ad){
    swaddr_write(ad, INT3_CODE_SIZE, INT3_CODE);
}
/*
  args:
  return value:
  Usage: (it assume that the program is loaded at the same place, so the breakpoint's
  information about memory location is meaningful.)
  So it should be invoked when restart the program(ie, cmd_r())!!!
*/

void reset_all_in_mem(){
    BP* t = bp_head();
    while (t != NULL) {
        set_bp_in_mem(t->bp_addr);
        t = t->next;
    }
}

void recover_content_in_mem(BP* b){
    swaddr_write(b->bp_addr, INT3_CODE_SIZE, b->content);
}
