#include "system.h"
#include "monitor.h"
#include "isr.h"

void memcpy(void *dest, const void *src, uint32_t length)
{
	const byte_t *srcPtr = (const byte_t *)src;
	byte_t *destPtr = (byte_t *)dest;

	for(; length != 0; length--)
	{
		*destPtr++ = *srcPtr++;
	}
}

void memset(void *dest, byte_t value, uint32_t length)
{
	byte_t *ptr = (byte_t *)dest;

	for(; length != 0; length--)
	{
		*ptr++ = value;
	}
}

byte_t inb(uint16_t port)
{
	byte_t ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

void outb(uint16_t port, byte_t value)
{
	asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

void panic(char *msg, char *file, uint32_t line)
{
	interrupt_disable();

	monitor_color_set(MONCOLOR_WHITE, MONCOLOR_RED);
	monitor_write("PANIC");
	monitor_color_reset();
	monitor_write(" ");
	monitor_writeline(msg);
	monitor_write(" (");
	monitor_write(file);
	monitor_put(':');
	monitor_writei(line, 'd');
	monitor_writeline(")");
	for (;;);
}

void panic_assert(char *msg, char *file, uint32_t line)
{
	interrupt_disable();

	monitor_color_set(MONCOLOR_WHITE, MONCOLOR_LBROWN);
	monitor_write("ASSERTION FAILED");
	monitor_color_reset();
	monitor_write(" ");
	monitor_writeline(msg);
	monitor_write(" (");
	monitor_write(file);
	monitor_put(':');
	monitor_writei(line, 'd');
	monitor_writeline(")");
	for (;;);
}
