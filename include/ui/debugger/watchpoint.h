#ifndef  __WATCHPOINT_H__
#define  __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
    uint32_t NO;
    struct watchpoint *next;

    char *expr;
    uint32_t last;//TODO @see expr.c: this should be the value of eval() return, ie, many other option
} WP;

#define INVALID_NO -1

void init_wp(WP*, WP*, uint32_t);
bool wp_num_ex(WP*, uint32_t, char*);
void print_wp();
bool changed(WP*);
void wp_des(WP*);

#endif /*  __WATCHPOINT_H__ */