#include "exec/helper.h"

//use case as the signed extension
#define JCC(condition, rel)                     \
    if(condition) {                             \
        cpu.eip += (rel);                       \
    }


#define DATA_BYTE 1
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE


extern char suffix;

make_helper(je_v) {
    return (suffix == 'l' ? je_l(eip) : je_w(eip));
}