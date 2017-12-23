#include "system.h"
#include "monitor.h"
#include "descriptor_tables.h"

int32 kmain()
{
	monitor_clear();
	monitor_color_set(MONCOLOR_WHITE, MONCOLOR_BLUE);
	monitor_writeline("tinyOS");
	monitor_color_reset();
	monitor_writeline("");

	init_descriptor_tables();

	monitor_writeline("");
	monitor_writeline("Ready.");

	asm volatile ("int $0x3");
	asm volatile ("int $0x4");

	for(;;); // loop forever
}
