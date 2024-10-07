/**
 * @file
 * @date 2024-10-07
 * @author Philipp Bolte (philipp@bolte.engineer)
 *
 * @brief
 */

#include "pico/stdlib.h"

#include "hd44780.hpp"
#include "hd44780.pio.h"

namespace Driver::Hd44780
{
    Hd44780::Hd44780(PIO pio, uint8_t sm, uint8_t pin)
        : mPio(pio), mSm(sm), mPin(pin)
    {
        // We want to use the 4 bit interface. A special initialization is needed, as the current state of
        // the display is unknown. First, the 8 bit mode is enabled, then the 4 bit mode is enabled.
        mOffset = pio_add_program(mPio, &hd44780_init_program);
        hd44780_load_init(mPio, mSm, mOffset, pin);

        pio_sm_put_blocking(mPio, mSm, INIT_8BIT);
        sleep_ms(5);
        pio_sm_put_blocking(mPio, mSm, INIT_8BIT);
        sleep_ms(1);
        pio_sm_put_blocking(mPio, mSm, INIT_8BIT);
        sleep_ms(1);
        pio_sm_put_blocking(mPio, mSm, INIT_4BIT);
        sleep_ms(1);

        pio_sm_set_enabled(mPio, mSm, false);
        pio_remove_program_and_unclaim_sm(&hd44780_init_program, mPio, mSm, mOffset);

        // Now, the display is in 4 bit mode. We can now use the normal program to interact with the display.
        mOffset = pio_add_program(mPio, &hd44780_4bit_program);
        hd44780_load_4bit(mPio, mSm, mOffset, pin);

        // Set the function set to 4 bit mode, two lines and a 5x8 font
        pio_sm_put_blocking(mPio, mSm, FUNCTION_SET(Interface::FOUR_BIT, Lines::TWO, Font::FIVE_BY_EIGHT));
        sleep_ms(1);

        // Enable the display
        pio_sm_put_blocking(mPio, mSm, DISPLAY_CONTROL(Display::ON, Cursor::ON, Blink::ON));
        sleep_ms(1);

        // Entry mode set
        pio_sm_put_blocking(mPio, mSm, ENTRY_MODE_SET(DdramMode::INCREMENT, ShiftMode::CURSOR_MOVE));
        sleep_ms(1);

        // Clear the display
        pio_sm_put_blocking(mPio, mSm, registers::CLEAR_DISPLAY);
        sleep_ms(1);
    }

    Hd44780::~Hd44780()
    {
        // Disable the display
        pio_sm_put_blocking(mPio, mSm, DISPLAY_CONTROL(Display::OFF, Cursor::OFF, Blink::OFF));
        sleep_ms(1);

        // Disable the state machine
        pio_sm_set_enabled(mPio, mSm, false);

        // Release the PIO and the state machine
        pio_remove_program_and_unclaim_sm(&hd44780_4bit_program, mPio, mSm, mOffset);
    }

    void Hd44780::ClearDisplay()
    {
        pio_sm_put_blocking(mPio, mSm, registers::CLEAR_DISPLAY);
        sleep_ms(1);
    }

    void Hd44780::SetCursor(uint8_t row, uint8_t col)
    {
        uint8_t address = registers::SET_DDRAM_ADDR;
        if (row == 1)
        {
            address += 0x40;
        }
        address += col;

        pio_sm_put_blocking(mPio, mSm, address);
        sleep_ms(1);
    }

    void Hd44780::Write(std::string_view text)
    {
        SetRs(1);
        for (const auto& c : text)
        {
            pio_sm_put_blocking(mPio, mSm, c);
            // sleep_ms(1);
        }
        SetRs(0);
    }

    void Hd44780::SetRs(uint8_t rs)
    {
        // Wait for PIO state machine to reach stable state based on its PC
        while ((!pio_sm_is_tx_fifo_empty(mPio, mSm)) || (pio_sm_get_pc(mPio, mSm) != mOffset + 0))
        {
        }
        pio_sm_set_enabled(mPio, mSm, false);
        pio_sm_put_blocking(mPio, mSm, rs);
        pio_sm_exec(mPio, mSm, pio_encode_pull(false, false));
        pio_sm_exec(mPio, mSm, pio_encode_mov(pio_y, pio_osr));
        pio_sm_set_enabled(mPio, mSm, true);
        sleep_ms(1);
    }

}  // namespace Driver::Hd44780
