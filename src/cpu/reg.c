#include "nemu.h"

#include <stdlib.h>
#include <time.h>

CPU_state cpu = {.s1 = 1};

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void reg_test() {
	srand(time(0));
	uint32_t sample[8];
	int i;
	for(i = R_EAX; i <= R_EDI; i ++) {
		sample[i] = rand();
		reg_l(i) = sample[i];
		assert(reg_w(i) == (sample[i] & 0xffff));
	}

	assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
	assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
	assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
	assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
	assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
	assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
	assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
	assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

	assert(sample[R_EAX] == cpu.eax);
	assert(sample[R_ECX] == cpu.ecx);
	assert(sample[R_EDX] == cpu.edx);
	assert(sample[R_EBX] == cpu.ebx);
	assert(sample[R_ESP] == cpu.esp);
	assert(sample[R_EBP] == cpu.ebp);
	assert(sample[R_ESI] == cpu.esi);
	assert(sample[R_EDI] == cpu.edi);
}

/*      -------------------------NOTICE------------------------
        Signed or Unsigned is the way people interpret bits and
        there is no so called signed or unsigned in the computer,
        so I may not use arithmetic calculation that need it to
        be a `signed` or `unsigned` number.
        
        Lists of operand to avoid:
        x > 0; x < 0
        a < b (-1 < 1 but if use unsigned, this can be wrong)

        Lists of operand can use:
        a + b
        a - b (the 2's complements use the same way to signed or unsigned)
        a == 0
        a >> x
        a & 0xff

        ------------------------More--------------------------
        The bits of a register or memory can even treated as decimal, BCD
          0010 0101   25
        + 0100 1000   48
        -----------
          0110 1101   6D, ''intermediate result''
        +      0110   06, ''adjustment''
        -----------
          0111 0011   73, ''adjusted result''
        @see https://en.wikipedia.org/wiki/Binary-coded_decimal
*/
void set_pf(uint32_t n) {
    uint8_t num = n & 0xff;
    bool set = true;
    unsigned int i = 0;
    for (i = 0; i < 8; ++i) {
        if (num % 2 == 1) {
            set = !set;
        }
        num = num >> 1;
    }
    cpu.pf = set;
}

/* had better just shift unsigned integer*/
inline void set_sf(uint32_t num, int size) {
    assert(size == 1 || size == 2 || size == 4);
    cpu.sf = num >> (8 * size - 1);
}

inline void set_zf(uint32_t num) {
    cpu.zf = (num == 0 ? 1 : 0);
}

/*set on carry from or borrow to the low order
  four bits of AL; cleared otherwise. */
void set_af_b(uint32_t left, uint32_t right) {
    if (set_af_c(left, ~right + 1)) {
        cpu.af = 0;
    }
    cpu.af = 1;
}

bool set_af_c(uint32_t left, uint32_t right) {
    left = left & 0xf;
    right = right & 0xf;
    uint32_t tmp = left + right;
    int high = tmp >> 4;
    assert (high == 1 || high == 0);
    cpu.af = high;
    return cpu.af;
}

/* if left < right when computing left−right, and a borrow must be performed
   ie, this function is for subtract

   we may think it as:  left + (-right) = left + (~right + 1),
   if carry = 0, there is some borrow,
   if carry = 1, there is no borrow.
   Think it as a clock!!!
*/
void set_cf_b(uint32_t left, uint32_t right) {
    if (set_cf_c(4, left, ~right + 1)) {
        cpu.cf = 0;
    }
    cpu.cf = 1;
}

// for addition
bool set_cf_c(int size, long long left, long long right) {
    assert(sizeof left > 4);//make sure the following right shift is valid
    long long tmp = left + right;
    int high = tmp >> (size * 8);
    assert(high == 0 || high == 1);
    cpu.cf = high;
    return cpu.cf;
}

void set_of_minus(int size, uint32_t res, uint32_t left, uint32_t right) {
    set_of_add(size, res, left, (~right+1));
}
void set_of_add(int size, uint32_t res, uint32_t left, uint32_t right) {
    int left_sign = left >> (size * 8 - 1);
    int right_sign = right >> (size * 8 - 1);
    int res_sign = res >> (size * 8 - 1);
    assert(left_sign == 1 || left_sign == 0);
    assert(res_sign == 1 || res_sign == 0);
    assert(right_sign == 1 || res_sign == 0);
    if (right_sign == left_sign && left_sign == ~res_sign) {
        cpu.of = 1;
    }
    cpu.of = 0;
}