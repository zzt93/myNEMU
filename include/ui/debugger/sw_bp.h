#ifndef  __SW_BP_H__
#define  __SW_BP_H__

#include "memory.h"
#include "breakpoint.h"


void reset_all_in_mem();
void set_bp_in_mem(swaddr_t);
void recover_content_in_mem(BP*);


#endif /*  __SW_BP_H__ */