#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

void *virtualToPhysical(uint32_t vramAddress);

void mapMemory(int pageIndex, uint32_t size, uint32_t vramAddress, void *ramAddress);
void unmapMemory(int pageIndex);
void unmapAllMemory(void);

#endif
