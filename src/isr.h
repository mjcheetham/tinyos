#pragma once
#include "types.h"

#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

// TODO: consolidate with definitions in descriptor_tables.h
// Base address for master PIC
#define PIC1 0x20
// Base address for slave PIC
#define PIC2 0xA0
#define PIC1_CMD PIC1
#define PIC1_DATA (PIC1_CMD + 1)
#define PIC2_CMD PIC2
#define PIC2_DATA (PIC2_CMD + 1)

typedef struct
{
    uint32 ds; // data segment selector
    uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // pushed by pusha
    uint32 int_no, err_code; // interrupt number and error code (if applicable)
    uint32 eip, cs, eflags, useresp, ss; // pushed by the processor automatically (on iret)
} registers_t;

typedef void (*isr_t)(registers_t);
void register_interrupt_handler(uint8 n, isr_t handler);
