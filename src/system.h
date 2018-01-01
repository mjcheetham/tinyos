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
#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(#b, __FILE__, __LINE__))

extern void panic(char *msg, char *file, uint32_t line);
extern void panic_assert(char *msg, char *file, uint32_t line);
