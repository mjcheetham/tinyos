#include "timer.h"
#include "system.h"
#include "isr.h"
#include "monitor.h"

uint32 tick = 0;

static void timer_callback(registers_t registers)
{
    UNUSED(registers);

    tick++;
    // monitor_write("Tick: ");
    // monitor_write_dec(tick);
    // monitor_writeline("");
}

void timer_init(uint32 frequency)
{
    monitor_write("Initialising platform timer... ");

    // Register timer callback
    register_interrupt_handler(IRQ0, &timer_callback);

    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    uint32 divisor = 1193180 / frequency;

    // Send the command byte (set repeating mode AND set divisor)
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    byte lo = (byte)(divisor & 0xFF);
    byte hi = (byte)( (divisor >> 8) & 0xFF );

    // Send the frequency divisor
    outb(0x40, lo);
    outb(0x40, hi);

    monitor_color_set(MONCOLOR_GREEN, MONCOLOR_BLACK);
    monitor_writeline("OK");
    monitor_color_reset();
}
