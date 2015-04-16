#ifndef __UI_BREAKPOINT_H__
#define __UI_BREAKPOINT_H__

#include "common.h"

#define INT3_CODE 0xcc
#define INT3_CODE_SIZE 1

typedef struct breakpoint {
	int NO;
	struct breakpoint *next;

	/* TODO: Add more members if necessary */
    swaddr_t bp_addr;
    bool enabled;
    unsigned char content;
} BP;

BP* bp_head();
BP* free_head();

#endif
