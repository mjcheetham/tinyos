#pragma once

// Define some uniform types
typedef enum
{
	true = 0,
	false = 1
} bool;

typedef unsigned short int   wchar;
typedef unsigned char         byte;
typedef unsigned char        uint8;
typedef   signed char         int8;
typedef unsigned short int  uint16;
typedef   signed short int   int16;
typedef unsigned long  int  uint32;
typedef   signed long  int   int32;
typedef unsigned long  long uint64;
typedef   signed long  long  int64;

// Basic IO
void memcpy(byte *dest, byte *src, int32 count);
void memset(byte *dest, byte value, int32 count);
byte inb(uint16 port);
void outb(uint16 port, byte data);

