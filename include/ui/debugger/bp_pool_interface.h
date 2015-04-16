#ifndef __BP_INTERFACE_H__
#define __BP_INTERFACE_H__

#include "common.h"
#include "breakpoint.h"

//implemented in breakpoint.c
BP* add_bp_pool(uint32_t);
Error free_byid(int);
void init_bp_pool();
void print_all_bp();
BP* find_by_mem(swaddr_t);

#endif /* BP_INTERFACE_H */