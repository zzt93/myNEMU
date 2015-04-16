#include "ui/debugger/watchpoint.h"
#include "utility/expr.h"
#include <stdlib.h>
#include <string.h>


void init_wp(WP* w, WP* ne, uint32_t num){
    w->NO = num;
    w->next = ne;
}

bool wp_num_ex(WP* w, uint32_t num, char *s){
    w->NO = num;
    uint32_t len = strlen(s)+1;
    w->expr = malloc(len * sizeof (char));
    strncpy(w->expr, s, len);
    bool su = false;
    w->last = expr(s, &su);
    return su;
}

void wp_des(WP* w) {
    w->NO = INVALID_NO;
    free(w->expr);
    w->expr = NULL;
}

void print_wp(WP* w) {
    printf("#%-5d watchpoint... %10s last is  %16u\n", w->NO, w->expr, w->last);
}

bool changed(WP* this){
    bool su = false;
    uint32_t now = expr(this->expr, &su);
    if (now != this->last) {
        puts("Hit the watchpoint ");
        print_wp(this);
        printf("now %s is %u\n", this->expr, now);
        this->last = now;
        return true;
    }
    return false;
}