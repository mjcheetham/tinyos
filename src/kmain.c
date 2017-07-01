#include "system.h"
#include "monitor.h"

int32 kmain()
{
	monitor_clear();
	monitor_color_set(FBCOLOR_GREEN, FBCOLOR_BLACK);
	monitor_writeline("tinyOS");
	monitor_color_reset();
	monitor_writeline("");
	monitor_write_hex(0xDEADBEEF);

	for(;;); // loop forever
}
