#pragma once
#include "types.h"

#define UNUSED(x) (void)(x)

// Basic IO
void memcpy(byte *dest, const byte *src, uint32 length);
void memset(byte *dest, byte value, uint32 length);
byte inb(uint16 port);
void outb(uint16 port, byte data);

// Debugging
#define DEBUG_BREAK() asm volatile ("xchg %bx, %bx")
