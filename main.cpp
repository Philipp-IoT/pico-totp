/**
 * @file 
 * @date 2024-10-07
 * @author Philipp Bolte (philipp@bolte.engineer)
 * 
 * @brief Entry point
 */
#include <stdio.h>
#include <string>
#include <format>
#include "pico/stdlib.h"
#include "hd44780.hpp"
#include "ds1307.hpp"

#include "interface_rtc.hpp"

#include "i2c.hpp"

constexpr Driver::Hd44780::CustomCharacterData loading_left {
	0b11111,
	0b10000,
	0b10000,
	0b10000,
	0b10000,
	0b10000,
	0b10000,
	0b11111
};

int main()
{
    stdio_init_all();

    Driver::Hd44780::Hd44780 lcd(pio0, 0, 2);

    Driver::I2c::I2cConfig i2cConfig(i2c0, 12, 13);
    Driver::I2c::I2c i2c(i2cConfig);

    uint8_t counter = 0;

    lcd.SetCustomCharacter(1, loading_left);

    // std::array<uint8_t, 8> data;
    // std::array<uint8_t, 1> command = {0x00};
    // std::array<uint8_t, 5> command2 = {0x00, 0x16, 0x20, 0x04, 0x05};
    // // i2c.write(0x68, command, true);
    // // i2c.read(0x68, data, false);

    // i2c.write(0x68, command, true);
    // i2c.write(0x68, command2, false);

    Driver::Ds1307::Ds1307 rtc(i2c);

    rtc.setTime(1728492434);


    while (true)
    {
        lcd.ClearDisplay();
        lcd.SetCursor(0, 0);
        lcd.Write("Counter: ");
        lcd.SetCursor(1, 0);
        lcd.Write("\001");
        lcd.Write(std::to_string(counter++));

        // i2c.write(0x68, command, true);
        // i2c.read(0x68, data, false);

        rtc.readTime([&lcd](int64_t timestamp) {
            lcd.SetCursor(1, 0);
            lcd.Write("Time: ");
            lcd.Write(std::to_string(timestamp));
        });
        
        sleep_ms(1000);
    }
}
