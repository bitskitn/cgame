#include <stddef.h>

#include "memory.h"

typedef struct
{
    uint32_t size;
    uint32_t vramAddress;
    void *ramAddress;
} Page;

static Page page[32] = {0};

void *
virtualToPhysical(uint32_t vramAddress)
{
    for (int i = 0; i < sizeof(page) / sizeof(*page); i++)
    {
        if (page[i].vramAddress <= vramAddress &&
            page[i].vramAddress + page[i].size > vramAddress)
        {
            return (char *)page[i].ramAddress +
                   (vramAddress - page[i].vramAddress);
        }
    }
    return NULL;
}

void
mapMemory(int pageIndex, uint32_t size, uint32_t vramAddress, void *ramAddress)
{
    page[pageIndex].size = size;
    page[pageIndex].vramAddress = vramAddress;
    page[pageIndex].ramAddress = ramAddress;
}

void
unmapMemory(int pageIndex)
{
    page[pageIndex].size = 0;
    page[pageIndex].vramAddress = 0;
    page[pageIndex].ramAddress = NULL;
}

void
unmapAllMemory(void)
{
    for (int i = 0; i < sizeof(page) / sizeof(*page); i++)
    {
        page[i].size = 0;
        page[i].vramAddress = 0;
        page[i].ramAddress = NULL;
    }
}
