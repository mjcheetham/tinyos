#include "io.h"
#include "types.h"
#include "isr.h"
#include "system.h"
#include "monitor.h"
#include "scancodes.h"

#define KB_DATA_PORT 0x60

static void  kb_isr(registers_t);
static uint8 kb_read_scancode(void);
static char  kb_translate_scancode(uint8);

void kb_init(void)
{
    register_interrupt_handler(IRQ1, &kb_isr);
}

static void kb_isr(registers_t registers)
{
    UNUSED(registers);

    uint8 sc = kb_read_scancode();
    char c = kb_translate_scancode(sc);
    monitor_put(c);
}

static uint8 kb_read_scancode(void)
{
    return inb(KB_DATA_PORT);
}

static char kb_translate_scancode(uint8 sc)
{
    // TODO: translate to Unicode (UTF-7) character
    return scancodes_down[sc];
}
