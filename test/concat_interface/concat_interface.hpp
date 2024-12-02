#pragma once

#include <string>
#include <string_view>

// The class interface used by the plugin user.

class ConcatInterface
{
public:
    virtual ~ConcatInterface() = default;
    virtual std::string concat(std::string_view left_value, std::string_view right_value) const = 0;
};
