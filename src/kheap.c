#include "kheap.h"
#include "types.h"
#include "paging.h"

// end is defined in linker script
extern uint32_t end;
uint32_t placement_address = (uint32_t)&end;

static uint32_t kmalloc_int(uint32_t size, bool_t align, uint32_t *phys)
{
    // TODO: implement a heap for the kernel to use!
    // For now, just assign memory at placement_address
    // and increment it by size.
    if (align && (placement_address & 0xFFFFF000))
    {
        // Page-align the address!
        placement_address &= 0xFFFFF000;
        placement_address += PAGE_SIZE;
    }

    if (phys)
    {
        *phys = placement_address;
    }

    uint32_t temp = placement_address;
    placement_address += size;
    return temp;
}

uint32_t kmalloc_align(uint32_t size)
{
    return kmalloc_int(size, true, NULL);
}

uint32_t kmalloc_phys(uint32_t size, uint32_t *phys)
{
    return kmalloc_int(size, false, phys);
}

uint32_t kmalloc_alignphys(uint32_t size, uint32_t *phys)
{
    return kmalloc_int(size, true, phys);
}

uint32_t kmalloc(uint32_t size)
{
    return kmalloc_int(size, false, NULL);
}
