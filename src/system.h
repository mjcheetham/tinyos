#pragma once
#include "types.h"

#define UNUSED_VAR(x) (void)(x)
#define UNUSED_FUNC __attribute__ ((unused))

// Basic IO
void memcpy(void *dest, const void *src, uint32_t length);
void memset(void *dest, byte_t value, uint32_t length);
byte_t inb(uint16_t port);
void outb(uint16_t port, byte_t data);

// Debugging
#define DEBUG_BREAK() asm volatile ("xchg %bx, %bx")

// Panic!
void panic(char *msg);
