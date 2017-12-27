#include "system.h"
#include "monitor.h"

void memcpy(byte *dest, const byte *src, uint32 length)
{
	const byte *srcPtr = (const byte *)src;
	byte *destPtr = (byte *)dest;

	for(; length != 0; length--)
	{
		*destPtr++ = *srcPtr++;
	}
}

void memset(byte *dest, byte value, uint32 length)
{
	byte *ptr = (byte *)dest;

	for(; length != 0; length--)
	{
		*ptr++ = value;
	}
}

byte inb(uint16 port)
{
	byte ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

void outb(uint16 port, byte value)
{
	asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

void panic(char *msg)
{
	monitor_writeline(msg);
	for (;;);
}
