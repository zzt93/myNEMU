#include "exec/helper.h"

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