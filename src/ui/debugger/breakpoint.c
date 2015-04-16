#include "ui/debugger/breakpoint.h"
#include "ui/debugger/sw_bp.h"

#include "nemu.h"

char* f_mes[] = {
    "Success",
    "The number you entered is out of range",
    "The argument is null",
    "The breakpoint's number is invalid",
};

char* bp_info[] = {
    "Num",
    "Type",
    "Enabled",
    "Address",
    "what",
};

#define info_size (sizeof bp_info/sizeof bp_info[0])

#define NR_BP 32

static BP bp_pool[NR_BP];
static BP *head, *free_;

static void add(BP**, BP*);
BP* bp_head(){
    return head;
}
BP* free_head(){
    return free_;
}
void set_bp_head(BP* h){
    head = h;
}
void set_free_head(BP* h){
    free_ = h;
}

void init_bp_pool() {
	int i;
	for(i = 0; i < NR_BP - 1; i ++) {
		bp_pool[i].NO = i;
		bp_pool[i].next = &bp_pool[i + 1];
	}
    bp_pool[i].NO = i;
	bp_pool[i].next = NULL;

	set_bp_head(NULL);
	set_free_head(bp_pool);
}

/* TODO: Implement the function of breakpoint */
static BP* alloc_bp(){
    test(free_head() != NULL, "free_: %p", free_head());
    
    BP *ne;
    ne = free_head();
    set_free_head(ne->next);
    add(&head, ne);
    return ne;
}

/* Usage: free the BP struct which is the successor of bp point to
 ie, bp->next
*/
static void free_bp(BP* bp){
    test(bp != NULL, "bp: %p\n", bp);
    
    BP* this = bp->next;
    bp->next = this->next;
    add(&free_, this);
}

/* FIXME:  How to check the range of program*/
BP* find_by_mem(uint32_t ad){
    test(ad > 0, "address: 0x%x\n", ad);
    
    BP* h = bp_head();
    while(h != NULL){
        if (h->bp_addr == ad) {
            return h;
        }
        h = h->next;
    }
    return NULL;
}

Error free_byid(int no){
    Error res = {NULL, SUCCESS};
    if(no < 0 || no >= 32){
        res.id = OUT_RANGE;
        res.str = f_mes[OUT_RANGE];
        return res;
    }
    if (bp_head() == NULL){
        res.id = NULL_POINTER;
        res.str = f_mes[NULL_POINTER];
        return res;
    }

    BP* h = bp_head();
    // only one bp should not invoke free_bp
    if (h->NO == no) {
        recover_content_in_mem(h);
        //modify the breakpoint pool
        BP* temp = h->next;
        add(&free_, h);
        set_bp_head(temp);
        return res;
    }
    //more than one bp: find the bp before the one with number
    while (h->next != NULL) {
        if(h->next->NO == no){
            recover_content_in_mem(h->next);
            break;
        }
        h = h->next;
    }
    if(h->next == NULL){
        res.id = INVALID;
        res.str = f_mes[INVALID];
        return res;
    }
    free_bp(h);
    return res;
}

/*
  add a element at the head of list
*/
static void add(BP** h, BP* a){
    test(a != NULL, "head.NO: %d\n", (*h)->NO);
    a->next = *h;
    *h = a;
    return;
}

static int count_of_bp = 0;
BP* add_bp_pool(uint32_t add){
    test(add > 0, "address: 0x%x\n", add);
    /* FIXME:  how to test the correctness of address
    */
    BP* this = alloc_bp();
    this->bp_addr = add;
    this->enabled = true;
    this->NO = count_of_bp++;
    this->content = (uint8_t)swaddr_read(add, INT3_CODE_SIZE);
    return this;
}


static void print_bp(BP* bp){
    printf("#%-5d breakpoint... %c ... 0x%x\n", bp->NO, (bp->enabled?'y':'n'), bp->bp_addr);
}

void print_all_bp(){
    unsigned int i = 0;
    puts("Breakpoints");
    for (; i < info_size; ++i) {
        printf("%-10s   ", bp_info[i]);
    }
    puts("");
    BP *h = bp_head();
    while (h != NULL) {
        print_bp(h);
        h = h->next;
    }
}


