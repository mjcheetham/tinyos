#include "isr.h"
#include "monitor.h"
#include "pic.h"
#include "system.h"

// Number of spurious interrupts received
uint64_t spurious_interrupts;

isr_t interrupt_handlers[256];

static uint16_t get_pic_isr(void)
{
	// Read the In-Service Register (ISR) of the PICs.
	// This is done by first writing the OCW3 command word
	// to the PIC command port, then immediately reading from
	// *command* (not data) port.
	outb(PIC1_CMD, PIC_READ_ISR); // master
	outb(PIC2_CMD, PIC_READ_ISR); // slave
	return (inb(PIC2_CMD) << 8) | inb(PIC1_CMD);
}

static bool_t is_spurious(registers_t registers)
{
	uint16_t isr = get_pic_isr();
	if (registers.int_no == IRQ7)
	{
		// Check the master PIC ISR
		return (!(0x00FF & isr));
	}
	else if (registers.int_no == IRQ15)
	{
		// Check the slave PIC ISR
		return (!(0xFF00 & isr));
	}

	return false;
}

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
	// Noise on the IRQ or INTR lines, or software sending
	// an EOI at the wrong time can lead to spurious interrupts.
	// We must check for spurious interrupts here so we can
	// ignore them and *NOT* send an EOI.
	if (is_spurious(registers))
	{
		spurious_interrupts++;
		monitor_write("spurious interrupt: IRQ");
		monitor_writelinei(registers.int_no - IRQ0, 'd');
		return;
	}

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