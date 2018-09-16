#include "paging.h"
#include "isr.h"
#include "kheap.h"
#include "monitor.h"
#include "system.h"
#include "types.h"

// Assume physical memory is 16 MiB
#define PHYS_MEM_SIZE 0x1000000

// Bitmap of frames (used/free)
uint32_t *frames;
uint32_t nframes;

// Kernel's page directory
page_directory_t *kernel_directory = 0;

// Current page directory
page_directory_t *current_directory = 0;

// Defined in kheap.c
extern uint32_t placement_address;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

static void set_frame_state(uint32_t frame_address, bool_t value)
{
	uint32_t frame  = frame_address / PAGE_SIZE;
	uint32_t index  = INDEX_FROM_BIT(frame);
	uint32_t offset = OFFSET_FROM_BIT(frame);
	if (value)
	{
		frames[index] |= (0x1 << offset);
	}
	else
	{
		frames[index] &= ~(0x1 << offset);
	}
}

UNUSED_FUNC static bool_t get_frame_state(uint32_t frame_address)
{
	uint32_t frame = frame_address / PAGE_SIZE;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	return (frames[idx] & (0x1 << off));
}

static uint32_t get_first_frame(void)
{
	for (uint32_t i = 0; i < INDEX_FROM_BIT(nframes); i++)
	{
		if (frames[i] != 0xFFFFFFFF) // nothing free, return
		{
			// at least one bit is free
			for (uint32_t j = 0; j < 32; j++)
			{
				uint32_t totest = 0x1 << j;
				if ( !(frames[i] & totest) )
				{
					return i * 4 * 8 + j;
				}
			}
		}
	}

	return -1;
}

static void switch_directory(page_directory_t *new)
{
	current_directory = new;
	asm volatile("mov %0, %%cr3" :: "r" (&new->tablesPhys));
	uint32_t cr0;
	asm volatile("mov %%cr0, %0" : "=r" (cr0));
	cr0 |= 0x80000000; // enable paging
	asm volatile("mov %0, %%cr0" :: "r" (cr0));
}

page_t *paging_get_page(page_directory_t *dir, uint32_t address, bool_t create)
{
	// Turn address into an index
	uint32_t index = address / PAGE_SIZE;

	// Find the page table containing this address
	uint32_t table_index = index / PAGE_TABLE_LENGTH;
	uint32_t page_index = index % PAGE_TABLE_LENGTH;
	if (dir->tables[table_index])
	{
		// Page table has already been created
		return &dir->tables[table_index]->pages[page_index];
	}
	else if (create)
	{
		// Create a new page table
		uint32_t new_address;
		dir->tables[table_index] = (page_table_t*)kmalloc_alignphys(sizeof(page_table_t), &new_address);
		memset(dir->tables[table_index], 0, PAGE_SIZE); // zero first page table entry in new table
		dir->tablesPhys[table_index] = new_address | 0x7; // present, rw, usermode
		return &dir->tables[table_index]->pages[page_index];
	}
	else
	{
		return 0;
	}
}

void paging_alloc_frame(page_t *page, bool_t kernel, bool_t writable)
{
	if (page->frame != 0)
	{
		return; // frame already allocated
	}

	uint32_t index = get_first_frame();
	if (index == (uint32_t)-1)
	{
		PANIC("out of frames");
	}

	set_frame_state(index * PAGE_SIZE, true);
	page->present = true;
	page->rw      = writable ? true : false;
	page->user    = kernel ? false : true;
	page->frame   = index;
}

UNUSED_FUNC void paging_free_frame(page_t *page)
{
	uint32_t frame;
	if (!(frame = page->frame))
	{
		return; // no allocated frame
	}

	set_frame_state(frame, false);
	page->frame = NULL;
}

static void handle_fault(registers_t registers)
{
	// Faulting address is stored in the CR2 register
	uint32_t faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

	bool_t present  = !(registers.err_code & 0x1); // page not present
	bool_t rw       =   registers.err_code & 0x2;  // write operation?
	bool_t usermode =   registers.err_code & 0x4;  // cpu in user-mode?
	bool_t reserved =   registers.err_code & 0x8;  // overwritten cpu-reserved bits of page entry?
	bool_t id       =   registers.err_code & 0x10; // caused by instruction fetch?

	UNUSED_VAR(id);

	monitor_write("page fault (");
	if (present)  monitor_write("present ");
	if (rw)       monitor_write("read-only ");
	if (usermode) monitor_write("user-mode ");
	if (reserved) monitor_write("reserved ");
	monitor_write(") at ");
	monitor_writelinei(faulting_address, 'x');
	PANIC("page fault");
}

void paging_init(void)
{
	monitor_write("Initialising paging... ");

	// Size of physical memory
	uint32_t mem_end_page = PHYS_MEM_SIZE;

	nframes = mem_end_page / PAGE_SIZE;
	frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
	memset(frames, 0, INDEX_FROM_BIT(nframes));

	// Make a page table directory
	kernel_directory = (page_directory_t*)kmalloc_align(sizeof(page_directory_t));
	memset(kernel_directory, 0, sizeof(page_directory_t));
	current_directory = kernel_directory;

	// Need to identify map (phys address = virtual address) from
	// 0x0 to the end of used memory, so we can access this transparently,
	// as if paging wasn't enabled.
	for (uint32_t i = 0; i < placement_address; i += PAGE_SIZE)
	{
		page_t *page = paging_get_page(kernel_directory, i, true);
		paging_alloc_frame(page, false, false);
	}

	// Register page fault handler
	interrupt_register(14, &handle_fault);

	// Enable paging
	switch_directory(kernel_directory);

	monitor_color_set(MONCOLOR_GREEN, MONCOLOR_BLACK);
	monitor_writeline("OK");
	monitor_color_reset();
}
