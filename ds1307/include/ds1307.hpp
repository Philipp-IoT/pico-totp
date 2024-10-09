/**
 * @file
 * @date 2024-10-08
 * @author Philipp Bolte (philipp@bolte.engineer)
 *
 * @brief DS1307 RTC driver
 */
#pragma once

#include "interface_i2c.hpp"
#include "interface_rtc.hpp"

namespace Driver::Ds1307
{

    using RtcReadSuccessCallback = std::function<void(int64_t)>;

    class Ds1307 : public InterfaceRTC
    {
    public:
        Ds1307(InterfaceI2C &i2c);
        ~Ds1307();

        bool setTime(int64_t unixTimestamp) override;
        bool readTime(RtcReadSuccessCallback onSuccess) const override;

    private:
        InterfaceI2C &mI2c;

        uint8_t DecToBcd(uint8_t val) const;
        uint8_t BcdToDec(uint8_t val) const;
    };

}  // namespace Driver::Ds1307
