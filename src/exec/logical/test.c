#include "exec/helper.h"

void set_flag(uint32_t tmp, int size) {
    cpu.of = 0;
    cpu.cf = 0;
    set_zf(tmp);
    set_sf(tmp, size);
    set_pf(tmp);
}

#define DATA_BYTE 1
#include "test-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "test-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "test-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(test_iAX_v) {
    return (suffix == 'l' ? test_iAX_l(eip) : test_iAX_w(eip));
}

make_helper(test_r7rm_v) {
    return (suffix == 'l' ? test_r7rm_l(eip) : test_r7rm_w(eip));
}