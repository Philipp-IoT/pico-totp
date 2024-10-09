/**
 * @file 
 * @date 2024-10-08
 * @author Philipp Bolte (philipp@bolte.engineer)
 * 
 * @brief Generic I2C interface
 */
#pragma once

#include <cstdint>
#include <span>

class InterfaceI2C{
public:
    virtual ~InterfaceI2C() = default;

    // Read data from the I2C device
    virtual bool read(uint8_t address, std::span<uint8_t> data, bool continueTransmission = false) = 0;

    // Write data to the I2C device
    virtual bool write(uint8_t address, std::span<const uint8_t> data, bool continueTransmission = false) = 0;

    // Read a register from the I2C device
    virtual bool readRegister(uint8_t address, uint8_t reg, std::span<uint8_t> data) = 0;

    // Write a register to the I2C device
    virtual bool writeRegister(uint8_t address, uint8_t reg, std::span<const uint8_t> data) = 0;
};
