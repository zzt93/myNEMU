#include "exec/helper.h"
#include "cpu/modrm.h"
#include "nemu.h"
#include "exec/all-instr.h"
#include "exec/exec.h"

char suffix = 'l';

make_helper(data_size) {
	suffix = 'w';
	int instr_len = exec(eip + 1);
	suffix = 'l';
	return instr_len + 1;
}

make_helper(esc2) {
    int instr_len = two_byte(eip + 1);
    return instr_len + 1;
}

/*
make_helper(eighty) {
    ModR_M m;
    m.val = instr_fetch(eip + 1, 1);
    switch(m.opcode) {
        case 0:
            //add
            break;
        default:
            assert(0);
            break;
    }
}
*/

make_helper(eighty3) {
    ModR_M m;
    m.val = instr_fetch(eip + 1, 1);
    switch(m.opcode) {
        case 7:
            return cmp_i8_rm_v(eip);
            break;
        default:
            assert(0);
            break;
    }
}
