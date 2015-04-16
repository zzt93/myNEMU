#include "exec/helper.h"

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

make_helper(test_r&rm_v) {
    return (suffix == 'l' ? test_r&rm_l(eip) : test_r&rm_w(eip));
}