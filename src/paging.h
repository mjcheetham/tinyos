#pragma once
#include "types.h"

// Page size is 4 KiB
#define PAGE_SIZE 0x1000

// Number of entries in a page table
#define PAGE_TABLE_LENGTH 1024

#define IS_PAGE_ALIGNED(x) ((x) & 0xFFFFF000 == 0)
#define IS_NOT_PAGE_ALIGNED(x) ((x) & 0xFFFFF000 != 0)

// Represents a page table entry
typedef struct
{
	bool_t present  :  1; // page is present in memory
	bool_t rw       :  1; // read-write or read-only
	bool_t user     :  1; // supervisor level only if clear
	bool_t accessed :  1; // has page been accessed since last refresh?
	bool_t dirty    :  1; // has page been written to since last refresh?
	uint32_t        :  7; // unused and reserved bits
	uint32_t frame  : 20; // frame address (shifted right 12 bits)
} page_t;

// Represents a page table (containing 1024 entries)
typedef struct
{
	page_t pages[PAGE_TABLE_LENGTH];
} page_table_t;

// Represents a directory of (PAGE_TABLE_LENGTH) page tables
typedef struct
{
	// Pointers to page tables
	page_table_t *tables[PAGE_TABLE_LENGTH];
	// Physical locations to page tables above
	uint32_t tablesPhys[PAGE_TABLE_LENGTH];
	// Physical address of tablesPhys
	uint32_t physAddress;
} page_directory_t;

void paging_init(void);
