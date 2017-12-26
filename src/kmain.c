#include "system.h"
#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "isr.h"
#include "io.h"

int32 kmain(void)
{
	monitor_clear();
	monitor_color_set(MONCOLOR_WHITE, MONCOLOR_BLUE);
	monitor_writeline("tinyOS");
	monitor_color_reset();
	monitor_writeline("");

	init_descriptor_tables();
	timer_init(50);
	kb_init();

	monitor_writeline("Ready.");
	monitor_writeline("");

DEBUG_BREAK();

	interrupt_enable();

	// asm volatile ("int $0x3");
	// asm volatile ("int $0x4");
	// asm volatile ("int $0x20");

	for(;;); // loop forever
}
