#include "ds1307.hpp"

namespace Driver::Ds1307
{

    Ds1307::Ds1307()
    {
    }

    Ds1307::~Ds1307()
    {
    }

    bool Ds1307::setTime(int64_t unixTimestamp)
    {
        return false;
    }

    bool Ds1307::readTime(RtcReadSuccessCallback onSuccess) const
    {
        return false;
    }

}  // namespace Driver::Ds1307
