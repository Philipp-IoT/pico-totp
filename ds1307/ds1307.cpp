#include "ds1307.hpp"
#include <ctime>

namespace Driver::Ds1307
{

    Ds1307::Ds1307(InterfaceI2C &i2c) : mI2c(i2c)
    {

    }

    Ds1307::~Ds1307()
    {

    }

    bool Ds1307::setTime(int64_t unixTimestamp)
    {
        tm *time = localtime(&unixTimestamp);

        std::array<uint8_t, 8> data = {
            0x00,
            DecToBcd(time->tm_sec),
            DecToBcd(time->tm_min),
            DecToBcd(time->tm_hour),
            DecToBcd(time->tm_wday),
            DecToBcd(time->tm_mday),
            DecToBcd(time->tm_mon + 1),
            DecToBcd(time->tm_year - 100)
        };

        return mI2c.write(0x68, data);

    }

    bool Ds1307::readTime(RtcReadSuccessCallback onSuccess) const
    {
        std::array<uint8_t, 7> data;

        mI2c.write(0x68, std::array<uint8_t, 1>{0x00}, true);

        if (mI2c.read(0x68, data, false))
        {
            // tm time = {
            //     BcdToDec(data[0]),
            //     BcdToDec(data[1]),
            //     BcdToDec(data[2]),
            //     BcdToDec(data[3]),
            //     BcdToDec(data[4]),
            //     BcdToDec(data[5]),
            //     BcdToDec(data[6]) + 100
            // };

            tm time;

            time.tm_sec = BcdToDec(data[0]);
            time.tm_min = BcdToDec(data[1]);
            time.tm_hour = BcdToDec(data[2]);
            time.tm_wday = BcdToDec(data[3]);
            time.tm_mday = BcdToDec(data[4]);
            time.tm_mon = BcdToDec(data[5]) - 1;
            time.tm_year = BcdToDec(data[6]) + 100;

            int64_t unixTimestamp = mktime(&time);

            onSuccess(unixTimestamp);

            return true;
        }
        return false;
    }

    uint8_t Ds1307::DecToBcd(uint8_t val) const
    {
        return ((val / 10 * 16) + (val % 10));
    }

    uint8_t Ds1307::BcdToDec(uint8_t val) const
    {
        return ((val / 16 * 10) + (val % 16));
    }

}  // namespace Driver::Ds1307
