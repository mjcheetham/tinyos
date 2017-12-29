#include "descriptor_tables.h"
#include "monitor.h"
#include "pic.h"
#include "system.h"

// Number of GDT entries
#define GDT_COUNT 5
// Number of IDT entries
#define IDT_COUNT 256

extern void gdt_flush(uint32_t);
extern void idt_flush(uint32_t);

static void gdt_set_gate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);
static void idt_set_gate(uint8_t, uint32_t, uint16_t, uint8_t);
static void pic_remap(void);

gdt_entry_t gdt_entries[GDT_COUNT];
gdt_ptr_t   gdt_ptr;
idt_entry_t idt_entries[IDT_COUNT];
idt_ptr_t   idt_ptr;

void gdt_init(void)
{
    monitor_write("Initialising GDT... ");

    gdt_ptr.limit = (sizeof(gdt_entry_t) * GDT_COUNT) - 1;
    gdt_ptr.base  =  (uint32_t)&gdt_entries;

    gdt_set_gate(0, 0, 0,          0,    0);    // null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // user mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // user mode data segment

    gdt_flush((uint32_t)&gdt_ptr);

    monitor_color_set(MONCOLOR_GREEN, MONCOLOR_BLACK);
    monitor_writeline("OK");
    monitor_color_reset();
}

void idt_init(void)
{
    monitor_write("Initialising IDT... ");

    idt_ptr.limit = (sizeof(idt_entry_t) * IDT_COUNT) - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    memset((byte_t *)&idt_entries, 0, sizeof(idt_entry_t)*IDT_COUNT);

    idt_set_gate( 0, (uint32_t)isr0 , 0x08, 0x8E);
    idt_set_gate( 1, (uint32_t)isr1 , 0x08, 0x8E);
    idt_set_gate( 2, (uint32_t)isr2 , 0x08, 0x8E);
    idt_set_gate( 3, (uint32_t)isr3 , 0x08, 0x8E);
    idt_set_gate( 4, (uint32_t)isr4 , 0x08, 0x8E);
    idt_set_gate( 5, (uint32_t)isr5 , 0x08, 0x8E);
    idt_set_gate( 6, (uint32_t)isr6 , 0x08, 0x8E);
    idt_set_gate( 7, (uint32_t)isr7 , 0x08, 0x8E);
    idt_set_gate( 8, (uint32_t)isr8 , 0x08, 0x8E);
    idt_set_gate( 9, (uint32_t)isr9 , 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

    pic_remap();

    idt_set_gate(32, (uint32_t)irq0 , 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1 , 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2 , 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3 , 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4 , 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5 , 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6 , 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7 , 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8 , 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9 , 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);

    idt_flush((uint32_t)&idt_ptr);

    monitor_color_set(MONCOLOR_GREEN, MONCOLOR_BLACK);
    monitor_writeline("OK");
    monitor_color_reset();
}

static void gdt_set_gate(int32_t number, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    gdt_entries[number].base_low     = (base & 0xFFFF);
    gdt_entries[number].base_middle  = (base >> 16) & 0xFF;
    gdt_entries[number].base_high    = (base >> 24) & 0xFF;

    gdt_entries[number].limit_low    = (limit & 0xFFFF);

    gdt_entries[number].granularity  = (limit >> 16) & 0x0F;
    gdt_entries[number].granularity |= granularity & 0xF0;

    gdt_entries[number].access       = access;
}

static void idt_set_gate(uint8_t number, uint32_t base, uint16_t selector, uint8_t flags)
{
    idt_entries[number].base_low  = base & 0xFFFF;
    idt_entries[number].base_high = (base >> 16) & 0xFFFF;

    idt_entries[number].selector  = selector;
    idt_entries[number].zero      = 0;

    // TODO: set interrupt gate to ring 3
    idt_entries[number].flags     = flags /* | 0x60 */;
}

static void pic_remap(void)
{
    // Restart both PICs
    outb(PIC1_CMD, 0x11);
    outb(PIC2_CMD, 0x11);

    // Start PIC1 at 32
    outb(PIC1_DATA, 0x20);

    // Start PIC2 at 40
    outb(PIC2_DATA, 0x28);

    // Set up cascading
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    // Finish
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    outb(PIC1_DATA, 0x0);
    outb(PIC2_DATA, 0x0);
}
