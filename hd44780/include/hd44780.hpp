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

    /**
     * @brief Driver for HD44780 LCD controller
     *
     * @details Currently, only 4 bit mode and 2 line displays are supported.
     *          It is more like a proof of concept than a full-fledged driver.
     */
    class Hd44780
    {
    public:
        /**
         * @brief The controller is configured inside the constructor
         *
         * @param pio PIO to use
         * @param sm  State machine to use
         * @param pin First pin. See hd44780.pio for the pin configuration.
         */
        Hd44780(PIO pio, uint8_t sm, uint8_t pin);

        /**
         * @brief Disables the display and unclaims the state machine
         */
        virtual ~Hd44780();

        /**
         * @brief Clears the display
         */
        void ClearDisplay();

        /**
         * @brief Sets the cursor to the given position
         *
         * @param row Row (0, 1)
         * @param col Column (0-15)
         */
        void SetCursor(uint8_t row, uint8_t col);

        /**
         * @brief Writes the given text to the display
         *
         * @param text Text to write
         */
        void Write(std::string_view text);

    private:
        const PIO mPio;
        const uint8_t mSm;
        const uint8_t mPin;
        uint32_t mOffset{0};

        void SetRs(uint8_t rs);
    };

}