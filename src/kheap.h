#pragma once
#include "types.h"

// Allocate memory.
uint32_t kmalloc(uint32_t size);

// Allocate memory, aligned to a page.
uint32_t kmalloc_align(uint32_t size);

// Allocate memory, also returning the physical address.
uint32_t kmalloc_phys(uint32_t size, uint32_t *phys);

// Allocate memory, aligned to a page, at the specified physical location.
uint32_t kmalloc_alignphys(uint32_t size, uint32_t *phys);
