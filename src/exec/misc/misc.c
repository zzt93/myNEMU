#include "exec/helper.h"

#include "ui/ui.h"

#include "nemu.h"

#include "cpu/int_flag.h"
#include "ui/debugger/bp_pool_interface.h"

make_helper(inv) {
	/* invalid opcode */

	uint8_t temp[8];
	((uint32_t *)temp)[0] = instr_fetch(eip, 4);
	((uint32_t *)temp)[1] = instr_fetch(eip + 4, 4);
	printf("invalid opcode(eip = 0x%08x): %02x %02x %02x %02x %02x %02x %02x %02x ...\n", 
			eip, temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);

	assert(0);
}

make_helper(int3) {
	/* A breakpoint is hit! Do something here! */
    BP * bp = find_by_mem(eip);
    test(bp != NULL, "find_by_mem return null at 0x%08x", cpu.eip);

    //recover old instruction
    swaddr_write(cpu.eip, 1, bp->content);
    //modify eip
    /*
      cpu.eip = cpu.eip - 1; -- @see cpu-exec.c#exec() : for we just return
      when meeting a breakpint, so we don't need to minus one here.
     */
    //set flag of meeting int3
    state_interrupt = INT3;

    printf("Breakpoint %d, source: not connected yet, address: 0x%02x\n", bp->NO, eip);

	return 1;
}

make_helper(nemu_trap) {
	printf("nemu: HIT \33[1;31m%s\33[0m TRAP at eip = 0x%08x\n\n", (cpu.eax == 0 ? "GOOD" : "BAD"), cpu.eip);
	nemu_state = END;

	print_asm("nemu trap");
	return 1;
}
