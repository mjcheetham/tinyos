#include "system.h"
#include "monitor.h"

int32 kmain()
{
	monitor_clear();
	monitor_write("tinyOS", FBCOLOR_GREEN, FBCOLOR_BLACK);

	for(;;); // loop forever
}
