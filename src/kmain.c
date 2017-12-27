#include "system.h"
#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "isr.h"
#include "io.h"

static void printheader(void);

void kmain(void)
{
	printheader();

	gdt_init();
	idt_init();
	timer_init(1000);
	kb_init();

	monitor_writeline("Ready.");

	interrupt_enable();
}

static void printheader(void)
{
	monitor_clear();
	monitor_color_set(MONCOLOR_WHITE, MONCOLOR_BLUE);
	monitor_writeline("tinyOS");
	monitor_color_reset();
}
