#include "common.h"
#include "memory.h"
#include <stdlib.h>
#include <limits.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
//for use of cpu and register
#include <cpu/reg.h>

/*enum {
    NOTYPE = 256, EQ,
    NUM, DEREF, NEGA,
    LEE, LAE, NEQ, LOR,
    LAND, LS, RS, 

};
*/
#define PRE_GAP 10

extern CPU_state cpu;
/*
  follow the precedence of operator
*/
enum {
    NOTYPE = 256, NUM,
    HNUM, REG_NAME,//hexadecimal number, register name

    DELIMITER,// a number to seperate number and operator

    //operator
    LOR, LAND, BIOR, BEOR, BAND,// ||, &&, |, ^, &
    EQ = BAND+PRE_GAP, NEQ,
    LE = NEQ+PRE_GAP, LA, LEE, LAE,
    LS = LAE+PRE_GAP, RS, // <<, >>
    ADD = RS+PRE_GAP, MIN,
    MUL = MIN+PRE_GAP, DIV,

    ASS_DELIM,
    //the associativity of below is from right to left so the dominent operator may differ
    DEREF = ASS_DELIM+PRE_GAP, NEGA, LNEGA, BCOM, //unary *, -, !, ~
};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
    {"<=", LEE},
    {">=", LAE},
    {"!=", NEQ},
    {"\\|\\|", LOR},                  //logical or
    {"&&", LAND},
    {"<<", LS},                     //left shift
    {">>", RS},                     //right shift
    {"<", LE},
    {">", LA},
    {"\\|", BIOR},                    //bitwise or
    {"&", BAND},
    {"\\^", BEOR},
	{"\\+", ADD},					// plus
    {"\\-", MIN},                   // minus
    {"\\*", MUL},                   // multiply
    {"/", DIV},                   // divide
	{"==", EQ},						// equal
    //single operator
    {"!", LNEGA},                      //logical negative
    {"~", BCOM},
    //not operator
    {"\\(", '('},
    {"\\)", ')'},
	{" +",	NOTYPE},				// white space
    {"0x[0-9abcdefABCDEF]+", HNUM},//has to be first to be checked
    {"[0-9]+", NUM},                // number

    {"\\$[a-zA-Z]+", REG_NAME},
};
//TODO
#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			test(0, "regex compilation failed: %s\n%s\n", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static char null_char[32] = {0};

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				//Log("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);

				position += substr_len;


				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */

                assert(nr_token <= 32);
                assert(substr_len <= 32);
                
				switch(rules[i].token_type) {
                    case NOTYPE:
                        break;
                    case NUM:
                        tokens[nr_token].type = NUM;
                        strncpy(tokens[nr_token].str, null_char, 32);
                        strncpy(tokens[nr_token].str, substr_start, substr_len);
                        nr_token++;
                        break;
                    case HNUM:
                        tokens[nr_token].type = HNUM;
                        strncpy(tokens[nr_token].str, null_char, 32);
                        strncpy(tokens[nr_token].str, substr_start, substr_len);
                        nr_token++;
                        break;
                    case REG_NAME:
                        tokens[nr_token].type = REG_NAME;
                        strncpy(tokens[nr_token].str, null_char, 32);
                        strncpy(tokens[nr_token].str, substr_start, substr_len);
                        nr_token++;
                        break;
                    case LOR:
                        tokens[nr_token].type = LOR;
                        nr_token++;
                        break;
                    case LAND:
                        tokens[nr_token].type = LAND;
                        nr_token++;
                        break;
                    case BIOR:
                        tokens[nr_token].type = BIOR;
                        nr_token++;
                        break;
                    case BEOR:
                        tokens[nr_token].type = BEOR;
                        nr_token++;
                        break;
                    case BAND:
                        tokens[nr_token].type = BAND;
                        nr_token++;
                        break;
                    case EQ:
                        tokens[nr_token].type = EQ;
                        nr_token++;
                        break;
                    case NEQ:
                        tokens[nr_token].type = NEQ;
                        nr_token++;
                        break;
                    case LE:
                        tokens[nr_token++].type = LE;
                        break;
                    case LA:
                        tokens[nr_token++].type = LA;
                        break;
                    case LEE:
                        tokens[nr_token++].type = LEE;
                        break;
                    case LAE:
                        tokens[nr_token++].type = LAE;
                        break;
                    case LS:
                        tokens[nr_token++].type = LS;
                        break;
                    case RS:
                        tokens[nr_token++].type = RS;
                        break;
                    case ADD:
                        tokens[nr_token].type = ADD;
                        nr_token++;
                        break;
                    case MIN:{
                        //minus and negative number
                        tokens[nr_token].type = MIN;
                        nr_token++;
                        break;
                        
                        /*                        bool metSign = false, metNum = false;
                                                  int duT = nr_token;
                                                  while (duT) {
                                                  if (tokens[duT].type == NUM && !metSign){//means this '-' is a minus
                                                  tokens[nr_token++] = {'-', NULL};
                                                  break;
                                                  } else if (tokens[duT].type == NUM && metSign) {//means this '-' is a negative number
                                                  if (regexec(&re[8], e + position, 1, &pmatch, 0) == 0
                                                  && pmatch.rm_so == 0) {//match next number next to the negative sign
                                                  char *subs_start = e + position;
                                                  int subs_len = pmatch.rm_eo;

                                                  Log("match regex[%d] at position %d with len %d: %.*s", i, position, subs_len, subs_len, subs_start);
                                                  position += subs_len;

                                                  tokens[nr_token++] = {NUM, ""};
                                                  strncpy(tokens[nr_token-1].str, subs_start, subs_len);
                                                  } else {
                                                  Log("this negative number:%s doesn't match the format ", e+position);
                                                  }
                                                  } else if ((tokens[duT].type == '-'
                                                  || tokens[duT].type == '+'
                                                  || tokens[duT].type == '*'
                                                  || tokens[duT].type == '/')){
                                                  metSign = true;
                                                  }

                                                  }*/
                    }
                    case MUL:
                        tokens[nr_token].type = MUL;
                        nr_token++;
                        break;
                    case DIV:
                        tokens[nr_token].type = DIV;
                        nr_token++;
                        break;
                    case LNEGA:
                        tokens[nr_token++].type = LNEGA;
                        break;
                    case BCOM:
                        tokens[nr_token++].type = BCOM;
                        break;
                    case '(':
                        tokens[nr_token].type =  '(';
                        nr_token++;
                        break;
                    case ')':
                        tokens[nr_token].type = ')';
                        nr_token ++;
                        break;
					default: assert(0);
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}

int dominent_op(int start, int end){
    int in_paren = 0;
    int result = -1;
    int min_op = INT_MAX;
    for (;start < end; start++) {
        if (tokens[start].type == NUM) {
        } else if(tokens[start].type == '(') {
            in_paren++;
        } else if (tokens[start].type == ')'){
            in_paren--;
        } else {//just signs
            if (in_paren) {//in the parenthesis
                //not record
            } else {
                if (tokens[start].type < ASS_DELIM) {//associativity of those oeprator are from left to right, so last is dominent
                    if (tokens[start].type-min_op <= PRE_GAP) {
                        min_op = tokens[start].type;
                        result = start;
                    }
                } else {
                    if (tokens[start].type-min_op < PRE_GAP) {
                        min_op = tokens[start].type;
                        result = start;
                    }
                }

            }
        }

    }
    return result;
}

static int stack[128];
static int sp;

bool pop(int *x){
    if (sp == 0) {
        puts("nothing to pop");
        return false;
    } else {
        *x = stack[--sp];
        return true;
    }
}
bool push(int x){
    if (sp > 128) {
        puts("to many elements");
        return false;
    } else if (x != '('){
        puts("not ( to push");
        return false;
    } else {
        stack[sp++] = x;
        return true;
    }
}
bool empty(){
    return sp == 0;
}

bool checkParen(int s, int e){
    //push the first token, if it is not a (, the empty will be true
    bool test = push(tokens[s++].type);
    int x;
    while (s < e) {
        // 1. the first isn't a '('
        // 2. the parenthesis are not in pair
        // 3. the leftmost and rightmost are not in pair
        if (empty()){
            return false;
        }
        if (tokens[s].type == '(') {
            test = push('(');
        } else if (tokens[s].type == ')'){
            test = pop(&x);
        }
        ++s;
        if (!test){
            return false;
        }
    }
    return true;
}

uint32_t get_reg(char *reg){
    if (strcmp("eip", reg) == 0) {
        return cpu.eip;
    }
    switch(reg[2]) {
        case '\0':
            switch(reg[1]) {
                case 's'://segament register
                    if (strcmp("cs", reg) == 0) {
                        return cpu.cs;
                    } else if (strcmp("ss", reg) == 0) {
                        return cpu.ss;
                    } else if (strcmp("ds", reg) == 0) {
                        return cpu.ds;
                    } else if (strcmp("fs", reg) == 0) {
                        return cpu.fs;
                    }
                    break;
                case 'f'://flag register
                    if (strcmp("cf", reg) == 0) {
                        return cpu.cs;
                    } else if (strcmp("pf", reg) == 0) {
                        return cpu.ss;
                    } else if (strcmp("af", reg) == 0) {
                        return cpu.ds;
                    } else if (strcmp("zf", reg) == 0) {
                        return cpu.fs;
                    } else if (strcmp("sf", reg) == 0) {
                        return cpu.sf;
                    } else if (strcmp("tf", reg) == 0) {
                        return cpu.tf;
                    } else if (strcmp("If", reg) == 0) {
                        return cpu.If;
                    } else if (strcmp("df", reg) == 0) {
                        return cpu.df;
                    } else if (strcmp("of", reg) == 0) {
                        return cpu.of;
                    }
                    break;
                default:
                    goto UNKNOWN;
            }
            break;
        case 'x'://common register
            if (strcmp("eax", reg) == 0) {
                return cpu.eax;
            } else if (strcmp("ecx", reg) == 0) {
                return cpu.ecx;
            } else if (strcmp("edx", reg) == 0) {
                return cpu.edx;
            } else if(strcmp("ebx", reg) == 0) {
                return cpu.ebx;
            }
            break;
        case 'p'://pointer register
        case 'i'://index register
            if (strcmp("esp", reg) == 0) {
                return cpu.esp;
            } else if (strcmp("ebp", reg) == 0) {
                return cpu.ebp;
            } else if (strcmp("esi", reg) == 0) {
                return cpu.esi;
            } else if (strcmp("edi", reg) == 0) {
                return cpu.edi;
            }
            break;
        default:
            goto UNKNOWN;
    }
UNKNOWN:
    printf("Unknown register: %s", reg);
    return -1;
}

static bool su = true;
/* FIXME:  the return value of this function
   should have many choices: signed int, double
*/
/*
  evaluate the interval from s to e
  **[s, e)
  */
static uint32_t eval(int s, int e){
    //printf("start:%d end:%d\n", s, e);
    if (s >= e) {
        // wrong
        su = false;
        return -1;
    } else if (s == e-1) {
        //single token
        if (tokens[s].str[0] == '0'
            && tokens[s].str[1] == 'x'){
            //printf("str: %s, hexadecimal: %ld\n", tokens[s].str, strtol(tokens[s].str, NULL, 16));
            return strtol(tokens[s].str, NULL, 16);
        } else if (tokens[s].str[0] == '$'){
            char *reg = &(tokens[s].str[1]);
            return get_reg(reg);
        } else {//variable
        }
        //decimal
        //printf("str: %s, number :%d\n", tokens[s].str, atoi(tokens[s].str));
        return atoi(tokens[s].str);
    } else if (checkParen(s, e) == true) {
        // ( expression )
        return eval(s+1, e-1);
    } else {
        int op = dominent_op(s, e);
        printf("dominent operator is %d\n", op);
        int val1 = eval(s, op);
        int val2 = eval(op+1, e);
        printf("dominent operator is %d, val1: %d, val2: %d\n", op, val1, val2);

        switch(tokens[op].type) {
            case ADD:
                return val1+val2;
            case MIN:
                return val1-val2;
            case MUL:
                return val1*val2;
            case DIV:
                return val1/val2;
            case LEE:
                return val1<=val2;
            case LAE:
                return val1>=val2;
            case NEQ:
                return val1!=val2;
            case LOR:
                return val1||val2;
            case LAND:
                return val1&&val2;
            case LS:
                return val1<<val2;
            case RS:
                return val1>>val2;
            case LE:
                return val1<val2;
            case LA:
                return val1>val2;
            case LNEGA:
                return !val2;
            case BIOR:
                return val1|val2;
            case BAND:
                return val1&val2;
            case BEOR:
                return val1^val2;
            case BCOM:
                return ~val2;
            case EQ:
                return val1==val2;
            case DEREF:
                puts("Hasn't figure out the read size");
                assert(false);
                return swaddr_read(val2, 1);//TODO sizeof??
            case NEGA:
                return -val2;
            default:
                su = false;
        }
    }
    su = false;
    return -1;
}


static bool is_operator(int type){
    return type>DELIMITER ;
}


/*
  args: {:}
  return value: return the value of expression (now only uint32) if the `e` is valid, or -1
  Usage:
*/
uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
    
    int i = 0;
    for (;i < nr_token; ++i) {
        if (tokens[i].type == MIN &&
            (i == 0 || (i != 0 && is_operator(tokens[i-1].type) ))) {
            tokens[i].type = NEGA;
            puts("in negation");
        } else if (tokens[i].type == MUL &&
            (i == 0 || (i != 0 && is_operator(tokens[i-1].type) ))) {
            tokens[i].type = DEREF;
        }

    }

    su = true;
    uint32_t res = eval(0, nr_token);
    *success = su;
    return res;
}

