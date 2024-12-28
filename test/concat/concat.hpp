#pragma once

#include <concat_interface/concat_interface.hpp>
#include <memory>

// The concrete class (implementing the class interface) provided by the plugin.

class Concat : public ConcatInterface
{
public:
    Concat();
    Concat(std::string_view left_decorator, std::string_view right_decorator);
    virtual ~Concat() = default;
    virtual std::string concat(std::string_view left_value, std::string_view right_value) const;

private:
    std::string left_decorator_;
    std::string right_decorator_;
};
