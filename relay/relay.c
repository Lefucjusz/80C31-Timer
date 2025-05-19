#include "relay.h"
#include <mcs51/8051.h>

#define RELAY_PIN P1_0

void relay_enable(bool enable)
{
    RELAY_PIN = !enable;
}
