#include "common.h"
#include "memory/dram.h"

void check_len(size_t len) {
	assert(len == 1 || len == 2 || len == 4);
}
/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
    check_len(len);
	return dram_read(addr, len);
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
    check_len(len);
	dram_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
    check_len(len);
	return hwaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
    check_len(len);
	hwaddr_write(addr, len, data);
}

static uint32_t hwaddr_read_instr(hwaddr_t addr, size_t len) {
    check_len(len);
	return dram_read(addr, len);
}

uint32_t instr_fetch(swaddr_t addr, size_t len) {
    check_len(len);
	return hwaddr_read_instr(addr, len);
}

static int32_t hwaddr_read_signed(hwaddr_t addr, size_t len) {
    check_len(len);
    return dram_read_signed(addr, len);
}

int32_t fetch_signed(swaddr_t addr, size_t len) {
    check_len(len);
	return hwaddr_read_signed(addr, len);
}