#pragma once
#include "types.h"
#include "ordered_array.h"

#define KHEAP_START        0xC0000000
#define KHEAP_INITIAL_SIZE 0x100000
#define KHEAP_MIN_SIZE     0x70000
#define KHEAP_INDEX_SIZE   0x20000
#define KHEAP_MAGIC        0x600DB10C // GOOD-BLOC(K)

typedef struct
{
	// Magic number used to detect a valid header block.
	// Should be the same as the footer magic number.
	uint32_t magic;

	// Flag if this block is empty.
	bool_t is_empty;

	// Size of the user-available space in the block (excludes header/footer).
	uint32_t size;
} kheap_block_header_t;

typedef struct
{
	// Magic number used to detect a valid footer block.
	// Should be the same as the header magic number.
	uint32_t magic;

	// Pointer to the header for this block.
	kheap_block_header_t *header;
} kheap_block_footer_t;

typedef struct
{
	// Map of the blocks in the heap.
	ordered_array_t blockmap;

	// Starting address of the heap.
	uint32_t start_address;

	// Current end address of the heap.
	uint32_t end_address;

	// Maximum address the heap can grow to.
	uint32_t max_address;

	// True if extra pages should be mapped as kernel-only, false otherwise.
	bool_t kernel;

	// True if extra pages should be mapped as read-only.
	bool_t readonly;
} kheap_t;

// Allocate memory.
uint32_t kmalloc(uint32_t size);

// Allocate memory, aligned to a page.
uint32_t kmalloc_align(uint32_t size);

// Allocate memory, also at the specified physical location.
uint32_t kmalloc_phys(uint32_t size, uint32_t *phys);

// Allocate memory, aligned to a page, at the specified physical location.
uint32_t kmalloc_alignphys(uint32_t size, uint32_t *phys);

// Create a new heap.
kheap_t *kheap_create(uint32_t start_address, uint32_t end_address, uint32_t max_address, bool_t kernel, bool_t readonly);

// Allocate memory on a heap of the specified size, optionally page-aligned.
void *kheap_alloc(kheap_t *heap, uint32_t size, bool_t align);

// Free memory allocated on a heap.
void kheap_free(kheap_t *heap, void *p);
