#include "exec/helper.h"

#include "exec/all-instr.h"

typedef int (*helper_fun)(swaddr_t);


/* TODO: Add more instructions!!! */

helper_fun opcode_table [256] = {
/* 0x00 */	inv, inv, inv, inv, 
/* 0x04 */	inv, inv, inv, inv, 
/* 0x08 */	inv, inv, inv, inv, 
/* 0x0c */	inv, inv, inv, esc2,
/* 0x10 */	inv, inv, inv, inv, 
/* 0x14 */	inv, inv, inv, inv, 
/* 0x18 */	inv, inv, inv, inv, 
/* 0x1c */	inv, inv, inv, inv, 
/* 0x20 */	inv, inv, inv, inv, 
/* 0x24 */	inv, inv, inv, inv,
/* 0x28 */	inv, inv, inv, inv, 
/* 0x2c */	inv, inv, inv, inv, 
/* 0x30 */	inv, inv, inv, inv, 
/* 0x34 */	inv, inv, inv, inv,
/* 0x38 */	inv, inv, inv, inv, 
/* 0x3c */	cmp_AXi_b, cmp_AXi_v, inv, inv, 
/* 0x40 */	inv, inv, inv, inv, 
/* 0x44 */	inv, inv, inv, inv,
/* 0x48 */	inv, inv, inv, inv, 
/* 0x4c */	inv, inv, inv, inv, 
/* 0x50 */	push_r_v, push_r_v, push_r_v, push_r_v,
/* 0x54 */	push_r_v, push_r_v, push_r_v, push_r_v,
/* 0x58 */	inv, inv, inv, inv, 
/* 0x5c */	inv, inv, inv, inv, 
/* 0x60 */	inv, inv, inv, inv,
/* 0x64 */	inv, inv, data_size, inv,
/* 0x68 */	inv, inv, inv, inv, 
/* 0x6c */	inv, inv, inv, inv, 
/* 0x70 */	inv, inv, inv, inv,
/* 0x74 */	je_b, inv, inv, inv,
/* 0x78 */	inv, inv, inv, inv, 
/* 0x7c */	inv, inv, inv, inv, 
/* 0x80 */	inv, inv, nemu_trap, eighty3,
/* 0x84 */	test_r7rm_b, test_r7rm_v, inv, inv,
/* 0x88 */	mov_r2rm_b, mov_r2rm_v, mov_rm2r_b, mov_rm2r_v,
/* 0x8c */	inv, inv, inv, inv, 
/* 0x90 */	inv, inv, inv, inv,
/* 0x94 */	inv, inv, inv, inv,
/* 0x98 */	inv, inv, inv, inv, 
/* 0x9c */	inv, inv, inv, inv, 
/* 0xa0 */	mov_moffs2a_b, mov_moffs2a_v, mov_a2moffs_b, mov_a2moffs_v,
/* 0xa4 */	inv, inv, inv, inv,
/* 0xa8 */	test_iAX_b, test_iAX_v, inv, inv,//iAX_b == iAL_b
/* 0xac */	inv, inv, inv, inv,
/* 0xb0 */	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb4 */	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb8 */	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v, 
/* 0xbc */	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v, 
/* 0xc0 */	inv, inv, inv, inv,
/* 0xc4 */	inv, inv, mov_i2rm_b, mov_i2rm_v,
/* 0xc8 */	inv, inv, inv, inv,
/* 0xcc */	int3, inv, inv, inv,
/* 0xd0 */	inv, inv, inv, inv,
/* 0xd4 */	inv, inv, inv, inv,
/* 0xd8 */	inv, inv, inv, inv,
/* 0xdc */	inv, inv, inv, inv,
/* 0xe0 */	inv, inv, inv, inv,
/* 0xe4 */	inv, inv, inv, inv,
/* 0xe8 */	inv, inv, inv, inv,
/* 0xec */	inv, inv, inv, inv,
/* 0xf0 */	inv, inv, inv, inv,
/* 0xf4 */	inv, inv, inv, inv,
/* 0xf8 */	inv, inv, inv, inv,
/* 0xfc */	inv, inv, inv, inv
};

/* All function defined with 'make_helper' return the length of the instruction.
 #define make_helper(name) int name(swaddr_t eip) */
make_helper(exec) {
	return opcode_table[ instr_fetch(eip, 1) ](eip);//function execute the instruction here
}


helper_fun two_opcode [256] = {
/* 0x00 */	inv, inv, inv, inv, 
/* 0x04 */	inv, inv, inv, inv, 
/* 0x08 */	inv, inv, inv, inv, 
/* 0x0c */	inv, inv, inv, inv,
/* 0x10 */	inv, inv, inv, inv, 
/* 0x14 */	inv, inv, inv, inv, 
/* 0x18 */	inv, inv, inv, inv, 
/* 0x1c */	inv, inv, inv, inv, 
/* 0x20 */	inv, inv, inv, inv, 
/* 0x24 */	inv, inv, inv, inv,
/* 0x28 */	inv, inv, inv, inv, 
/* 0x2c */	inv, inv, inv, inv, 
/* 0x30 */	inv, inv, inv, inv, 
/* 0x34 */	inv, inv, inv, inv,
/* 0x38 */	inv, inv, inv, inv, 
/* 0x3c */	inv, inv, inv, inv, 
/* 0x40 */	inv, inv, inv, inv, 
/* 0x44 */	inv, inv, inv, inv,
/* 0x48 */	inv, inv, inv, inv, 
/* 0x4c */	inv, inv, inv, inv, 
/* 0x50 */	inv, inv, inv, inv,
/* 0x54 */	inv, inv, inv, inv,
/* 0x58 */	inv, inv, inv, inv, 
/* 0x5c */	inv, inv, inv, inv, 
/* 0x60 */	inv, inv, inv, inv,
/* 0x64 */	inv, inv, inv, inv,
/* 0x68 */	inv, inv, inv, inv, 
/* 0x6c */	inv, inv, inv, inv, 
/* 0x70 */	inv, inv, inv, inv,
/* 0x74 */	inv, inv, inv, inv,
/* 0x78 */	inv, inv, inv, inv, 
/* 0x7c */	inv, inv, inv, inv, 
/* 0x80 */	inv, inv, inv, inv,
/* 0x84 */	je_v, inv, inv, inv,
/* 0x88 */	inv, inv, inv, inv,
/* 0x8c */	inv, inv, inv, inv, 
/* 0x90 */	inv, inv, inv, inv,
/* 0x94 */	inv, inv, inv, inv,
/* 0x98 */	inv, inv, inv, inv, 
/* 0x9c */	inv, inv, inv, inv, 
/* 0xa0 */	inv, inv, inv, inv,
/* 0xa4 */	inv, inv, inv, inv,
/* 0xa8 */	inv, inv, inv, inv,//iAX_b == iAL_b
/* 0xac */	inv, inv, inv, inv,
/* 0xb0 */	inv, inv, inv, inv,
/* 0xb4 */	inv, inv, inv, inv,
/* 0xb8 */	inv, inv, inv, inv,
/* 0xbc */	inv, inv, inv, inv,
/* 0xc0 */	inv, inv, inv, inv,
/* 0xc4 */	inv, inv, inv, inv,
/* 0xc8 */	inv, inv, inv, inv,
/* 0xcc */	inv, inv, inv, inv,
/* 0xd0 */	inv, inv, inv, inv,
/* 0xd4 */	inv, inv, inv, inv,
/* 0xd8 */	inv, inv, inv, inv,
/* 0xdc */	inv, inv, inv, inv,
/* 0xe0 */	inv, inv, inv, inv,
/* 0xe4 */	inv, inv, inv, inv,
/* 0xe8 */	inv, inv, inv, inv,
/* 0xec */	inv, inv, inv, inv,
/* 0xf0 */	inv, inv, inv, inv,
/* 0xf4 */	inv, inv, inv, inv,
/* 0xf8 */	inv, inv, inv, inv,
/* 0xfc */	inv, inv, inv, inv
};

make_helper(two_byte) {
    return two_opcode [instr_fetch(eip, 1)](eip);
}