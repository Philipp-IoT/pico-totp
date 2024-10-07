/**
 * @file
 * @date 2024-10-07
 * @author Philipp Bolte (philipp@bolte.engineer)
 *
 * @brief
 */

#pragma once

#include <string_view>

#include "hardware/pio.h"

#include "hd44780_registers.hpp"

namespace Driver::Hd44780
{

    class Hd44780
    {
    public:
        Hd44780(PIO pio, uint8_t sm, uint8_t pin);

        virtual ~Hd44780();

        void ClearDisplay();

        void SetCursor(uint8_t row, uint8_t col);

        void Write(std::string_view text);

    private:
        const PIO mPio;
        const uint8_t mSm;
        const uint8_t mPin;
        uint32_t mOffset{0};

        void SetRs(uint8_t rs);
    };

}