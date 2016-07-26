#include "system.h"

void memcpy(byte *dest, byte *src, int32 count)
{
	for(; count != 0; count--)
	{
		*dest++ = *src++;
	}
}

void memset(byte *dest, byte value, int32 count)
{
	for(; count != 0; count--)
	{
		*dest++ = value;
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

