#include "timer.h"
#include "system.h"
#include "isr.h"
#include "monitor.h"

uint32_t tick = 0;

static void timer_callback(registers_t registers)
{
    UNUSED_VAR(registers);

    tick++;
    // monitor_write("Tick: ");
    // monitor_writeline_dec(tick);
}

void timer_init(uint32_t frequency)
{
    monitor_write("Initialising platform timer... ");

    // Register timer callback
    interrupt_register(IRQ0, &timer_callback);

    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    uint32_t divisor = 1193180 / frequency;

    // Send the command byte (set repeating mode AND set divisor)
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    byte_t lo = (byte_t)(divisor & 0xFF);
    byte_t hi = (byte_t)( (divisor >> 8) & 0xFF );

    // Send the frequency divisor
    outb(0x40, lo);
    outb(0x40, hi);

    monitor_color_set(MONCOLOR_GREEN, MONCOLOR_BLACK);
    monitor_writeline("OK");
    monitor_color_reset();
}
