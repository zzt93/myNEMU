#include "exec/helper.h"

void cmp_set_flag(int size, uint32_t l, uint32_t r, uint32_t tmp) {
    set_zf(tmp);
    set_pf(tmp);
    set_sf(tmp, size);
    //use carry bit as borrow bit
    set_cf_b(l, r);
    set_af_b(l, r);
    set_of_minus(size, tmp, l, r);
}

#define DATA_BYTE 1
#include "cmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmp-template.h"
#undef DATA_BYTE


extern char suffix;

make_helper(cmp_AXi_v) {
    return (suffix == 'l' ? cmp_AXi_l(eip) : cmp_AXi_w(eip));
}

make_helper(cmp_i8_rm_v) {
    return (suffix == 'l' ? cmp_i8_rm_l(eip) : cmp_i8_rm_w(eip));
}