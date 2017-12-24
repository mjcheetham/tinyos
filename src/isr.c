#include "isr.h"
#include "system.h"
#include "monitor.h"
#include "pic.h"

isr_t interrupt_handlers[256];

static void invoke_interrupt_handler(registers_t registers);

void isr_handler(registers_t registers)
{
    monitor_write("recieved interrupt: ");
    monitor_write_dec(registers.int_no);
    monitor_writeline("");

    invoke_interrupt_handler(registers);
}

void irq_handler(registers_t registers)
{
    monitor_write("recieved interrupt: ");
    monitor_write_dec(registers.int_no);
    monitor_write(" (IRQ");
    monitor_write_dec(registers.int_no - IRQ0);
    monitor_writeline(")");

    // Send an EOI (end of interrupt) signal to the PICs
    if (registers.int_no >= IRQ8)
    {
        // Send reset signal to the slave
        outb(PIC2_CMD, PIC_EOI);
    }
    // Send reset signal to the master
    outb(PIC1_CMD, PIC_EOI);

    invoke_interrupt_handler(registers);
}

void register_interrupt_handler(uint8 n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

static void invoke_interrupt_handler(registers_t registers)
{
    if (interrupt_handlers[registers.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[registers.int_no];
        handler(registers);
    }
}