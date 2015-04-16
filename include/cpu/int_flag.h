#ifndef __INT_FLAG_H__
#define __INT_FLAG_H__

#include "common.h"

typedef enum {
    NO, INT3, AFTER_INT3,
} Int_state;

extern Int_state state_interrupt;

#endif /* INT_FLAG_H */