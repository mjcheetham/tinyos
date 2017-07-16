#include "isr.h"
#include "monitor.h"

void isr_handler(registers_t registers)
{
    monitor_write("recieved interrupt: ");
    monitor_write_hex(registers.int_no);
    monitor_write("\r\n");
}
