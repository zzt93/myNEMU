#ifndef  __WP_POOL_H__
#define  __WP_POOL_H__

#include "watchpoint.h"

WP* add_wp_pool(char *);
bool del_by_id(uint32_t);
void wp_pool_init();
void print_all_wp();
bool check_wp();

#define NUM_WP 32
#endif /*  __WP_POOL_H__ */