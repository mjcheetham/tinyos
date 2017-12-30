#pragma once

// Base address for master PIC
#define PIC1 0x20
// Base address for slave PIC
#define PIC2 0xA0
#define PIC1_CMD PIC1
#define PIC1_DATA (PIC1_CMD + 1)
#define PIC2_CMD PIC2
#define PIC2_DATA (PIC2_CMD + 1)

#define PIC_EOI 0x20
#define PIC_READ_ISR 0x0B
