#include "system.h"
#include "descriptor_tables.h"
#include "isr.h"
#include "io.h"
#include "monitor.h"
#include "paging.h"
#include "timer.h"

static void printheader(void);

void kmain(void)
{
	printheader();

	gdt_init();
	idt_init();
	timer_init(1000);
	kb_init();
	paging_init();

	monitor_writeline("Ready.");

	interrupt_enable();

	// force page fault
	uint32_t *ptr = (uint32_t*)0xA0000000;
	uint32_t fault = *ptr;
	UNUSED_VAR(fault);
}

static void printheader(void)
{
	monitor_clear();
	monitor_color_set(MONCOLOR_WHITE, MONCOLOR_BLUE);
	monitor_writeline("tinyOS");
	monitor_color_reset();
}
