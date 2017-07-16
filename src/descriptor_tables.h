#pragma once
#include "types.h"

// Describes one GDT entry
typedef struct
{
    uint16 limit_low;       // the lower 16 bits of the limit
    uint16 base_low;        // the lower 16 bits of the base
    uint8  base_middle;     // the next 8 bits of the base
    uint8  access;          // access flags, determines the segment's ring
    uint8  granularity;
    uint8  base_high;       // the last 8 bits of the base
} __attribute__((packed)) gdt_entry_t;

// Describes a pointer to the GDT
typedef struct
{
    uint16 limit;           // the upper 16 bits of all selector limits
    uint32 base;            // the address of the first GDT entry
} __attribute__((packed)) gdt_ptr_t;

// Describes one IDT entry
typedef struct
{
    uint16 base_low;        // the lower 16 bits of the address to jump on interrupt
    uint16 selector;        // kernel segment selector
    uint8  zero;            // always zero
    uint8  flags;           // more flags
    uint16 base_high;       // the upper 16 bits of the address to jump on interrupt
} __attribute__((packed)) idt_entry_t;

// Describes a pointer to the IDT
typedef struct
{
    uint16 limit;
    uint32 base;            // the address of the first IDT entry
} __attribute__((packed)) idt_ptr_t;

void init_descriptor_tables();

// ISR handlers
extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
