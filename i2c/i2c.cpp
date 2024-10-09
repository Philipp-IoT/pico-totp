#include "i2c.hpp"
#include "pico/stdlib.h"

namespace Driver::I2c
{
    I2c::I2c(const I2cConfig &config) : mConfig(config)
    {
        // Configure the I2c master
        i2c_init(mConfig.instance, 100000);
        gpio_set_function(mConfig.sdaPin, GPIO_FUNC_I2C);
        gpio_set_function(mConfig.sclPin, GPIO_FUNC_I2C);
        gpio_pull_up(mConfig.sdaPin);
        gpio_pull_up(mConfig.sclPin);
    }

    I2c::~I2c()
    {
        i2c_deinit(mConfig.instance);
    }

    bool I2c::read(uint8_t address, std::span<uint8_t> data, bool continueTransmission)
    {
        i2c_read_blocking(mConfig.instance, address, data.data(), data.size(), continueTransmission);
        return true;
    }

    bool I2c::write(uint8_t address, std::span<const uint8_t> data, bool continueTransmission)
    {
        i2c_write_blocking(mConfig.instance, address, data.data(), data.size(), continueTransmission);
        return true;
    }

    bool I2c::readRegister(uint8_t address, uint8_t reg, std::span<uint8_t> data)
    {
        i2c_write_blocking(mConfig.instance, address, &reg, 1, true);
        i2c_read_blocking(mConfig.instance, address, data.data(), data.size(), false);
        return true;
    }

    bool I2c::writeRegister(uint8_t address, uint8_t reg, std::span<const uint8_t> data)
    {
        i2c_write_blocking(mConfig.instance, address, &reg, 1, true);
        i2c_write_blocking(mConfig.instance, address, data.data(), data.size(), false);
        return true;
    }
}
