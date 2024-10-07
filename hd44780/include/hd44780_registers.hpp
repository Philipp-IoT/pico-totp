/**
 * @file
 * @date 2024-10-07
 * @author Philipp Bolte (philipp@bolte.engineer)
 *
 * @brief Register definitions for the HD44780 LCD controller
 */

#pragma once

#include <cstdint>

namespace Driver::Hd44780
{

    /// Register definitions
    namespace registers
    {
        constexpr uint8_t CLEAR_DISPLAY = 0x01; 
        constexpr uint8_t RETURN_HOME = 0x02;
        constexpr uint8_t ENTRY_MODE_SET = 0x04;
        constexpr uint8_t DISPLAY_CONTROL = 0x08;
        constexpr uint8_t CURSOR_SHIFT = 0x10;
        constexpr uint8_t FUNCTION_SET = 0x20;
        constexpr uint8_t SET_CGRAM_ADDR = 0x40;
        constexpr uint8_t SET_DDRAM_ADDR = 0x80;
    }

    /// Increment or decrement the DDRAM address after read/write
    enum class DdramMode : uint8_t
    {
        INCREMENT = 0x02, ///< Increment the DDRAM address after read/write
        DECREMENT = 0x00, ///< Decrement the DDRAM address after read/write
    };

    /// Shift the entire display or move the cursor
    enum class ShiftMode : uint8_t
    {
        DISPLAY_SHIFT = 0x01, ///< Shift the entire display
        CURSOR_MOVE = 0x00, ///< Move the cursor
    };

    /// Assemble the ENTRY_MODE_SET register
    constexpr uint8_t ENTRY_MODE_SET(DdramMode mode, ShiftMode shift)
    {
        return registers::ENTRY_MODE_SET | static_cast<uint8_t>(mode) | static_cast<uint8_t>(shift);
    }

    /// Display enabled state
    enum class Display : uint8_t
    {
        ON = 0x04,  ///< Enable display
        OFF = 0x00, ///< Disable display
    };

    /// Cursor enabled state
    enum class Cursor : uint8_t
    {
        ON = 0x02,  ///< Enable cursor
        OFF = 0x00, ///< Disable cursor
    };

    /// Blink enabled state
    enum class Blink : uint8_t
    {
        ON = 0x01,  ///< Enable blink
        OFF = 0x00, ///< Disable blink
    };

    /// Assemble the DISPLAY_CONTROL register
    constexpr uint8_t DISPLAY_CONTROL(Display display, Cursor cursor, Blink blink)
    {
        return registers::DISPLAY_CONTROL | static_cast<uint8_t>(display) | static_cast<uint8_t>(cursor) | static_cast<uint8_t>(blink);
    }

    /// Interface width
    enum class Interface : uint8_t
    {
        EIGHT_BIT = 0x10, ///< 8 bit interface
        FOUR_BIT = 0x00,  ///< 4 bit interface
    };

    /// Number of lines
    enum class Lines : uint8_t
    {
        TWO = 0x08, ///< 2 lines
        ONE = 0x00, ///< 1 line
    };

    /// Font size
    enum class Font : uint8_t
    {
        FIVE_BY_TEN = 0x04,   ///< 5x10 font
        FIVE_BY_EIGHT = 0x00, ///< 5x8 font
    };

    /// Assemble the FUNCTION_SET register
    constexpr uint8_t FUNCTION_SET(Interface interface, Lines lines, Font font)
    {
        return registers::FUNCTION_SET | static_cast<uint8_t>(interface) | static_cast<uint8_t>(lines) | static_cast<uint8_t>(font);
    }

    // 4 bit interface need special initialization
    constexpr uint8_t INIT_8BIT {0x03};
    constexpr uint8_t INIT_4BIT {0x02};

}  // namespace hd44780