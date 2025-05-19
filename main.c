#include <hd44780.h>
#include <system.h>
#include <encoder.h>
#include <gui.h>

void main(void)
{
    system_init();
    encoder_init();
    hd44780_init();
    gui_init();

    while (1) {
        gui_task();
    }
}
