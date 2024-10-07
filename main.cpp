/**
 * @file 
 * @date 2024-10-07
 * @author Philipp Bolte (philipp@bolte.engineer)
 * 
 * @brief Entry point
 */
#include <stdio.h>
#include <string>
#include "pico/stdlib.h"
#include "hd44780.hpp"

int main()
{
    stdio_init_all();

    Driver::Hd44780::Hd44780 lcd(pio0, 0, 2);

    uint8_t counter = 0;
    while (true)
    {
        lcd.ClearDisplay();
        lcd.SetCursor(0, 0);
        lcd.Write("Counter: ");
        lcd.SetCursor(1, 0);
        lcd.Write(std::to_string(counter++));
        sleep_ms(1000);
    }
}
