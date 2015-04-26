#ifndef __EXEC_HELPER_H__
#define __EXEC_HELPER_H__

#include "cpu/reg.h"
#include "memory/memory.h"

/* All function defined with 'make_helper' return
   the numbers of byte of the instruction. */
#define make_helper(name) int name(swaddr_t eip)

extern char assembly[];
#define print_asm(...) assert(snprintf(assembly, 40, __VA_ARGS__) < 40)

#endif
