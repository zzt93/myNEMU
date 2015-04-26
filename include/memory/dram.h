#ifndef __DRAM_H__
#define __DRAM_H__


uint32_t dram_read(hwaddr_t addr, size_t len);
void dram_write(hwaddr_t addr, size_t len, uint32_t data);
int32_t dram_read_signed(hwaddr_t addr, size_t len);


#endif /* __DRAM_H__ */
