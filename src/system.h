#pragma once
#include "types.h"

// Basic IO
void memcpy(byte *dest, byte *src, int32 count);
void memset(byte *dest, byte value, int32 count);
byte inb(uint16 port);
void outb(uint16 port, byte data);

