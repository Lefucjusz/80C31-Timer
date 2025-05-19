#include "system.h"
#include <mcs51/8051.h>

/* Gives interrupt every 10ms with 10MHz XTAL */
#define SYSTEM_TIMER_RELOAD_VALUE 57203
#define SYSTEM_TIMER_PERIOD_MS 10

static volatile uint32_t ticks;

void system_init(void)
{
    /* Disable interrupts and stop timer */
    EA = 0;
    TR0 = 0;
    TF0 = 0;

    /* Timer 0 in Mode 1, use as timer */
    TMOD &= 0xF0;
    TMOD |= 0x01;

    /* Load count value */
    TH0 = (SYSTEM_TIMER_RELOAD_VALUE >> 8) & 0xFF;
    TL0 = SYSTEM_TIMER_RELOAD_VALUE & 0xFF;

    /* Start Timer 0 */
    TR0 = 1;

    /* Enable global interrupts and Timer 0 interrupt */
    EA = 1;
    ET0 = 1;
}

uint32_t system_get_ticks(void)
{
    uint32_t current_ticks;

    /* Disable timer interrupt and make local copy to prevent race condition */
    ET0 = 0;
    current_ticks = ticks;
    ET0 = 1;

    return current_ticks;
}

void system_delay_ms(uint16_t ms)
{
    for (uint16_t i = 0; i < ms; ++i) {
        for (uint8_t j = 0; j < 62; ++j) {
            continue;
        }
    }
}

void system_timer_isr(void) __interrupt(1)
{
    ticks += SYSTEM_TIMER_PERIOD_MS;

    /* Reload timer */
    TH0 = (SYSTEM_TIMER_RELOAD_VALUE >> 8) & 0xFF;
    TL0 = SYSTEM_TIMER_RELOAD_VALUE & 0xFF;
}
