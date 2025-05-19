#include "encoder.h"
#include <mcs51/8051.h>
#include <system.h>

#define ENCODER_PIN_A P3_2
#define ENCODER_PIN_B P3_0
#define ENCODER_PIN_C P3_1

#define ENCODER_DEBOUNCE_TIME_MS 40

static volatile uint16_t count;
static volatile uint32_t last_tick;

void encoder_init(void)
{
    /* Enable falling edge triggered external interrupt INT0 */
    IT0 = 1;
    EX0 = 1;

    /* Enable interrupts */
    EA = 1;
}

void encoder_reset(void)
{
    count = 0;
}

uint16_t encoder_get_count(void)
{
    uint16_t current_count;

    /* Disable INT0 and make local copy to prevent race condition */
    IT0 = 0;
    current_count = count;
    IT0 = 1;

    return current_count;
}

bool encoder_get_button_state(void)
{
    return ENCODER_PIN_C;
}

void encoder_isr(void) __interrupt(0)
{
    const uint32_t current_tick = system_get_ticks();

    if ((current_tick - last_tick) < ENCODER_DEBOUNCE_TIME_MS) {
        return; 
    }

    if (ENCODER_PIN_A ^ ENCODER_PIN_B) {
        ++count;
    }
    else {
        --count;
    }

    last_tick = current_tick;
}
