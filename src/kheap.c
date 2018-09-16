#include "kheap.h"
#include "paging.h"

// end is defined in linker script
extern uint32_t end;
uint32_t placement_address = (uint32_t)&end;

// defined in paging.c
extern page_directory_t *kernel_directory;

static uint32_t kmalloc_int(uint32_t size, bool_t align, uint32_t *phys)
{
	// TODO: implement a heap for the kernel to use!
	// For now, just assign memory at placement_address
	// and increment it by size.
	if (align && IS_NOT_PAGE_ALIGNED(placement_address))
	{
		// Page-align the address!
		placement_address = NEXT_PAGE_BOUNDARY(placement_address);
	}

	if (phys)
	{
		*phys = placement_address;
	}

	uint32_t temp = placement_address;
	placement_address += size;
	return temp;
}

// If |a| < |b| return -1, if |a| > |b| return +1, otherwise return 0. Where |x| := is size of heap x.
static int8_t blockmap_comparison(void *a, void *b)
{
	uint32_t a_size = ((kheap_block_header_t*)a)->size;
	uint32_t b_size = ((kheap_block_header_t*)b)->size;
	if (a_size < b_size)
	{
		return -1;
	}
	if (a_size > b_size)
	{
		return 1;
	}

	// Equal sized
	ASSERT(a_size == b_size);
	return 0;
}

// Find the blockmap index of the smallest block larger than the specified size in the given heap.
static bool_t find_smallest_hole(kheap_t *heap, uint32_t size, bool_t align, uint32_t *index)
{
	uint32_t i;
	// Heaps created with kheap_create have their blockmaps sorted by increasing block size
	for (i = 0; i < heap->blockmap.size; i++)
	{
		kheap_block_header_t *header = ordered_array_get(i, &heap->blockmap);
		uint32_t offset = 0;
		if (align)
		{
			// Calculate the offset from the start of the block's data region
			// to the next page boundary.
			uint32_t data_location = (uint32_t)header + sizeof(kheap_block_header_t);
			if (IS_NOT_PAGE_ALIGNED(data_location))
			{
				offset = NEXT_PAGE_BOUNDARY(data_location) - data_location;
			}
		}

		// Check if the block is large enough
		if (header->size - offset >= size)
		{
			// This block is large enough to fulfill the caller's desired size
			*index = i;
			return true;
		}
	}

	// Reached the end of the blockmap without finding a large enough block
	return false;
}

// Expand the heap to the specified size, rounding up to the next page boundary.
static uint32_t expand(kheap_t *heap, uint32_t new_size)
{
	uint32_t old_size = heap->end_address - heap->start_address;
	ASSERT(old_size < new_size);

	if (IS_NOT_PAGE_ALIGNED(new_size))
	{
		new_size = NEXT_PAGE_BOUNDARY(new_size);
		ASSERT(IS_PAGE_ALIGNED(new_size));
	}

	// Update end address and allocate new frames
	// for the extra space in the heap.
	uint32_t old_end = heap->end_address;
	heap->end_address = heap->start_address + new_size;
	for (uint32_t addr = old_end; addr < heap->end_address; addr += PAGE_SIZE)
	{
		page_t *page = paging_get_page(kernel_directory, addr, false);
		paging_alloc_frame(page, heap->kernel, !(heap->readonly));
	}

	return new_size;
}

// Contract the heap to the specified size, rounding up to the next page boundary.
static uint32_t contract(kheap_t *heap, uint32_t new_size)
{
	uint32_t old_size = heap->end_address - heap->start_address;
	ASSERT(new_size < old_size);

	if (IS_NOT_PAGE_ALIGNED(new_size))
	{
		new_size = NEXT_PAGE_BOUNDARY(new_size);
		ASSERT(IS_PAGE_ALIGNED(new_size));
	}

	// Update end address and free allocated frames
	// that are not longer part of the heap.
	uint32_t old_end = heap->end_address;
	heap->end_address = heap->start_address + new_size;
	for (uint32_t addr = old_end; addr >= heap->end_address; addr -= PAGE_SIZE)
	{
		page_t *page = paging_get_page(kernel_directory, addr, false);
		paging_free_frame(page);
	}

	return new_size;
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

kheap_t *kheap_create(uint32_t start_address, uint32_t end_address, uint32_t max_address, bool_t kernel, bool_t readonly)
{

}

void *kheap_alloc(kheap_t *heap, uint32_t size, bool_t align)
{

}

void kheap_free(kheap_t *heap, void *p)
{

}
