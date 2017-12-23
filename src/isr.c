#include "isr.h"
#include "monitor.h"

isr_t interrupt_handlers[256];

void isr_handler(registers_t registers)
{
    monitor_write("recieved interrupt: ");
    monitor_write_hex(registers.int_no);
    monitor_write("\r\n");
}

void irq_handler(registers_t registers)
{
    // Send an EOI (end of interrupt) signal to the PICs
    if (registers.int_no >= 40)
    {
        // Send reset signal to the slave
        outb(PIC2_CMD, 0x20);
    }
    // Send reset signal to the master
    outb(PIC1_CMD, 0x20);

    if (interrupt_handlers[registers.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[registers.int_no];
        handler(registers);
    }
}

void register_interrupt_handler(uint8 n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}
