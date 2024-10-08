/**
 * @file 
 * @date 2024-10-08
 * @author Philipp Bolte (philipp@bolte.engineer)
 * 
 * @brief Generic interface for a real-time clock using callbacks
 */

#pragma once

#include <cstdint>
#include <functional>

class InterfaceRTC {
public:
    virtual ~InterfaceRTC() = default;

    // Set the time using a UNIX timestamp
    virtual bool setTime(int64_t unixTimestamp) = 0;

    // Read the current time as a UNIX timestamp
    virtual bool readTime(std::function<void(int64_t)> onSuccess) const = 0;
};
