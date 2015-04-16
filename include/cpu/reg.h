#ifndef __CPU_REG_H__
#define __CPU_REG_H__

#include "common.h"

/* TODO: Re-organize the 'CPU_state' structure to match
 * the GPR encoding scheme in i386 instruction format.
 * For example, if we access reg_w(R_BX) we will get the 'bx' register;
 * if we access reg_b(R_CH), we will get the 'ch' register.
 * Hint: Use 'union'.
 * For more details about the GPR encoding scheme, see i386 manual.
 */

typedef union {
	union {
		uint32_t _32;
		uint16_t _16;
		uint8_t _8[2];
	} gpr[8];//general purpose register

	/* Do NOT change the order of the GPRs' definitions. 
	 * They match the register encoding scheme used in i386 instruction format.
	 * See i386 manual for more details.
	 */
    struct{
        uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;

        swaddr_t eip;

        unsigned int cs : 1;
        unsigned int ss : 1;
        unsigned int ds : 1;
        unsigned int fs : 1;
        
        uint8_t cf : 1;
        const uint8_t s1 : 1;
        uint8_t pf : 1;
        const uint8_t s2 : 1;
        uint8_t af : 1;
        const uint8_t s3 : 1;
        uint8_t zf : 1;
        uint8_t sf : 1;
        uint8_t tf : 1;
        uint8_t If : 1;
        uint8_t df : 1;
        uint8_t of : 1;
        
    };
    
} CPU_state;

extern CPU_state cpu;

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

#define reg_l(index) (cpu.gpr[index]._32)
#define reg_w(index) (cpu.gpr[index]._16)
#define reg_b(index) (cpu.gpr[index & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

void set_pf(uint32_t);
void set_zf(uint32_t);
void set_sf(uint32_t);
void set_cf_b(uint32_t, uint32_t);
bool set_cf_c(int, long long, long long);
void set_af_b(uint32_t, uint32_t);
bool set_af_c(uint32_t, uint32_t);
void set_of_add(int size, uint32_t, uint32_t, uint32_t);
void set_of_minus(int, uint32_t, uint32_t, uint32_t);
#endif
