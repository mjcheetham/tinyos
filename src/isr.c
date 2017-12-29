#include "isr.h"
#include "monitor.h"
#include "pic.h"
#include "system.h"

isr_t interrupt_handlers[256];

static void invoke_handler(registers_t registers)
{
    if (interrupt_handlers[registers.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[registers.int_no];
        handler(registers);
    }
    else
    {
        monitor_write("unhandled interrupt: ");
        monitor_writei(registers.int_no, 'd');
        if (IRQ0 <= registers.int_no && registers.int_no <= IRQ15)
        {
            monitor_write(" (IRQ");
            monitor_writei(registers.int_no - IRQ0, 'd');
            monitor_write(")");
        }
        monitor_writeline("");
        panic("unhandled interrupt");
    }
}

void isr_handler(registers_t registers)
{
    invoke_handler(registers);
}

void irq_handler(registers_t registers)
{
    // Send an EOI (end of interrupt) signal to the PICs
    if (registers.int_no >= IRQ8)
    {
        // Send reset signal to the slave
        outb(PIC2_CMD, PIC_EOI);
    }
    // Send reset signal to the master
    outb(PIC1_CMD, PIC_EOI);

    invoke_handler(registers);
}

void interrupt_register(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

void interrupt_enable()
{
    asm volatile ("sti");
}

void interrupt_disable()
{
    asm volatile ("cli");
}