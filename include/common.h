#ifndef __COMMON_H__
#define __COMMON_H__

#include "debug.h"
#include "macro.h"

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

typedef uint8_t bool;
typedef uint32_t hwaddr_t;//phsical address -- hardware
typedef uint32_t lnaddr_t;//linear address
typedef uint32_t swaddr_t;//virtual address -- software

#define false 0
#define true 1

typedef enum {
    SUCCESS, OUT_RANGE, NULL_POINTER, INVALID,
} ERR_MSG;

typedef struct {
    char *str;
    ERR_MSG id;
} Error;
  /*s[] = {
    "fine", FINE,
    "the argument is out of range", OUT_RANGE,
    "the argument is null", NULL_POINTER,
};
  */
#endif
