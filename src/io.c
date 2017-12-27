#include "io.h"
#include "types.h"
#include "isr.h"
#include "system.h"
#include "monitor.h"
#include "scancodes.h"

#define KB_DATA_PORT 0x60

static void  kb_isr(registers_t);
static uint8_t kb_read_scancode(void);
static char  kb_translate_scancode(uint8_t);

void kb_init(void)
{
    monitor_write("Initialising keyboard... ");

    interrupt_register(IRQ1, &kb_isr);

    monitor_color_set(MONCOLOR_GREEN, MONCOLOR_BLACK);
    monitor_writeline("OK");
    monitor_color_reset();
}

static void kb_isr(registers_t registers)
{
    UNUSED(registers);

    uint8_t sc = kb_read_scancode();
    char c = kb_translate_scancode(sc);
    monitor_put(c);
}

static uint8_t kb_read_scancode(void)
{
    return inb(KB_DATA_PORT);
}

static char kb_translate_scancode(uint8_t sc)
{
    // TODO: translate to Unicode (UTF-7) character
    return scancodes_down[sc];
}
