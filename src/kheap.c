#include "kheap.h"
#include "system.h"
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
static bool_t find_empty_block(kheap_t *heap, uint32_t size, bool_t align, uint32_t *index)
{
	uint32_t i;
	// Heaps created with kheap_create have their blockmaps sorted by increasing block size
	for (i = 0; i < heap->blockmap.size; i++)
	{
		kheap_block_header_t *header = ordered_array_get(&heap->blockmap, i);
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

// Get the last block in the given heap.
static bool_t get_last_block(kheap_t *heap, kheap_block_header_t *block)
{
	kheap_block_footer_t *footer = (kheap_block_footer_t*)(heap->end_address - sizeof(kheap_block_footer_t));
	if (footer->magic == KHEAP_MAGIC && footer->header->magic == KHEAP_MAGIC)
	{
		block = footer->header;
		return true;
	}

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
UNUSED_FUNC static uint32_t contract(kheap_t *heap, uint32_t new_size)
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
	ASSERT(IS_PAGE_ALIGNED(start_address));
	ASSERT(IS_PAGE_ALIGNED(end_address));

	// Allocate memory for the heap struct
	kheap_t *heap = (kheap_t*)kmalloc(sizeof(kheap_t));

	// Create the block map with the custom comparer
	heap->blockmap = ordered_array_place(KHEAP_BMAP_SIZE, &blockmap_comparison, (void*)start_address);

	// Heap data region will start right after the block map (page aligned)
	uint32_t data_start_address = start_address + sizeof(type_t) * KHEAP_BMAP_SIZE;
	if (IS_NOT_PAGE_ALIGNED(data_start_address))
	{
		data_start_address = NEXT_PAGE_BOUNDARY(data_start_address);
		ASSERT(IS_PAGE_ALIGNED(data_start_address));
	}

	heap->start_address = data_start_address;
	heap->end_address = end_address;
	heap->max_address = max_address;
	heap->kernel = kernel;
	heap->readonly = readonly;

	// Start with one empty block in the heap
	kheap_block_header_t *header = (kheap_block_header_t*)data_start_address;
	header->magic = KHEAP_MAGIC;
	header->is_empty = true;
	header->size = end_address - data_start_address - sizeof(kheap_block_header_t) - sizeof(kheap_block_footer_t);
	kheap_block_footer_t *footer = (kheap_block_footer_t*)(end_address - sizeof(kheap_block_footer_t));
	footer->header = header;
	footer->magic = KHEAP_MAGIC;

	ordered_array_insert(&heap->blockmap, header);

	return heap;
}

void *kheap_alloc(kheap_t *heap, uint32_t size, bool_t align)
{
	ASSERT(size > 0);

	kheap_block_header_t *free_block = NULL;
	uint32_t block_index;
	if (find_empty_block(heap, size, align, &block_index))
	{
		free_block = ordered_array_get(&heap->blockmap, block_index);
	}
	else // need to expand the heap
	{
		// Get the last block in the heap and check if it's free so that
		// we can expand it to cover the space in the expanded heap
		kheap_block_header_t *last_block = NULL;
		bool_t expand_last_block = get_last_block(heap, last_block) && last_block->is_empty;

		// Expand the heap to fit a block of the desired size
		uint32_t old_heap_endaddr = heap->end_address;
		uint32_t old_heap_size = heap->end_address - heap->start_address;
		uint32_t new_heap_size = old_heap_size + size + sizeof(kheap_block_header_t) + sizeof(kheap_block_footer_t);
		expand(heap, new_heap_size);

		// Expand the last block to fill the extra space
		if (expand_last_block)
		{
			// Update the size
			last_block->size += sizeof(kheap_block_header_t) + size;

			// Write a new footer at the end of the heap
			kheap_block_footer_t *footer = (kheap_block_footer_t*)(heap->end_address - sizeof(kheap_block_footer_t));
			footer->header = last_block;
			footer->magic = KHEAP_MAGIC;

			free_block = last_block;
		}
		else // create a new block in the extra space
		{
			free_block = (kheap_block_header_t*)(old_heap_endaddr);
			free_block->size = size;
			free_block->magic = KHEAP_MAGIC;
			kheap_block_footer_t *footer = (kheap_block_footer_t*)(heap->end_address - sizeof(kheap_block_footer_t));
			footer->header = free_block;
			footer->magic = KHEAP_MAGIC;

			ordered_array_insert(&heap->blockmap, free_block);
		}
	}

	ASSERT(free_block->magic == KHEAP_MAGIC);
	ASSERT(free_block->is_empty);

	// Check if the remaining space in the block could fit another block
	if (free_block->size - size >= sizeof(kheap_block_header_t) + sizeof(kheap_block_footer_t))
	{
		// TODO: create a new block in the remaining space
	}

	// TODO: align the block to a page boundary if required

	// TODO: mark the block as empty and remove it from the free blockmap

	// TODO: return pointer to start of user data
	return NULL;
}

void kheap_free(kheap_t *heap, void *p)
{
	UNUSED_VAR(heap);
	UNUSED_VAR(p);
}
