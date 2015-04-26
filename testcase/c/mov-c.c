#include "trap.h"

int A[10];
int b;

int main() {
	A[0] = 0;
	A[1] = 1;
	A[2] = 2;
	A[3] = 3;
	A[4] = 4;

	b = A[3];
	A[5] = b;

	nemu_assert(A[0] == 0);
	nemu_assert(A[1] == 1);
	nemu_assert(A[2] == 2);
	nemu_assert(A[3] == 3);
	nemu_assert(A[4] == 4);
	nemu_assert(b == 3);
	nemu_assert(A[5] == 3);

    A[6] = -1;
    A[7] = 255;//in this case, 255 is too large for signed imm8, compiler use imm32
    A[8] = 124;
    nemu_assert(A[6] == -1);
    nemu_assert(A[7] == 255);
    nemu_assert(A[8] == 124);

	HIT_GOOD_TRAP;

	return 0;
}
