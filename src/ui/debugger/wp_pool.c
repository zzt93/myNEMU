#include "ui/debugger/watchpoint.h"
#include "ui/debugger/wp_pool.h"

char * wp_info[] = {
    "Num",
    "Type",
    "Expression",
    "last value",
};

#define info_size (sizeof wp_info/sizeof wp_info[0])

static WP wp_pool[NUM_WP];
static WP use, avail;

static WP* use_head(){
    return &use;
}
/*
static void set_use_next(WP* u){
    use.next = u;
}
static void set_avail_next(WP* a){
    avail.next = a;
}
*/
static WP* avail_head(){
    return &avail;
}


void wp_pool_init() {
    int i = 0;
    for (; i < NUM_WP - 1; ++i) {
        init_wp(&wp_pool[i], &wp_pool[i+1], i);
    }
    init_wp(&wp_pool[i], NULL, i);

    init_wp(use_head(), NULL, INVALID_NO);
    init_wp(avail_head(), wp_pool, INVALID_NO);
}

static void add_at_head(WP* head, WP* w){
    test(head != NULL && w != NULL, "head is %p and w is %p", head, w);
    
    w->next = head->next;
    head->next = w;
}

static WP* remove_before(WP* bef) {
    test(bef != NULL , "before node is %p", bef);
    WP* this = bef->next;
    test(this != NULL, "this node is null");
    bef->next = this->next;
    return this;
}

static WP* alloc_wp() {
    test(avail_head() != NULL, "avail head :%p", avail_head);

    WP* next = remove_before(avail_head());
    add_at_head(use_head(), next);
    return next;
}
/*
  args: {bef:if it is true, the function will return the node before the exact node;
  otherwise, return the node with exact id}
  return value:
  Usage:
*/
WP* find_by_id(unsigned int id, bool bef){
    test(id >= 0 && id < NUM_WP, "id is %u", id);
    
    WP* last = use_head();
    WP* this = last->next;
    while (this != NULL && this->NO != id) {
        last = this;
        this = this->next;
    }
    if (this == NULL) {
        return NULL;
    }

    if (bef) {
        return last;
    }
    return this;
}

bool del_by_id(unsigned int id){
    test(id >= 0 && id < NUM_WP, "id is %u", id);
    
    WP* last = find_by_id(id, true);
    if (last == NULL) {
        return false;
    }

    WP* this = remove_before(last);
    add_at_head(avail_head(), this);
    wp_des(this);
    return true;
}

static uint32_t count_of_wp = 0;
WP* add_wp_pool(char *ex){
    WP* this = alloc_wp();
    bool su = wp_num_ex(this, count_of_wp, ex);
    if (su) {
        count_of_wp++;
        return this;
    }
    // means the expression is invalid so free the allocated memory
    test( this == use_head()->next, "...");
    remove_before(use_head());
    add_at_head(avail_head(), this);
    wp_des(this);
    return NULL;
}

void print_all_wp(){
    unsigned int i = 0;
    puts("Watchpoints");
    for (; i < info_size; ++i) {
        printf("%-10s   ", wp_info[i]);
    }
    puts("");
    WP* h = use_head()->next;
    while (h != NULL) {
        print_wp(h);
        h = h->next;
    }
}

bool check_wp() {
    WP* he = use_head()->next;
    bool chan = false;
    while (he != NULL) {
        if (changed(he)) {
            chan = true;
        }
        he = he->next;
    }
    return chan;
}