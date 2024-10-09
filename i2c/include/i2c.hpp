#pragma once

#include "interface_i2c.hpp"
#include "hardware/i2c.h"
#include <span>

namespace Driver::I2c
{
    struct I2cConfig
    {
        i2c_inst_t *instance;
        uint sdaPin;
        uint sclPin;

        I2cConfig(i2c_inst_t *instance, uint sdaPin, uint sclPin)
            : instance(instance), sdaPin(sdaPin), sclPin(sclPin)
        {
        }
    };

    class I2c : public InterfaceI2C
    {
    public:
        I2c(const I2cConfig &config);
        ~I2c();

        bool read(uint8_t address, std::span<uint8_t>, bool continueTransmission = false) override;
        bool write(uint8_t address, std::span<const uint8_t>, bool continueTransmission = false) override;
        
        bool readRegister(uint8_t address, uint8_t reg, std::span<uint8_t>) override;
        bool writeRegister(uint8_t address, uint8_t reg, std::span<const uint8_t>) override;

    private:
        const I2cConfig mConfig;
    };
}