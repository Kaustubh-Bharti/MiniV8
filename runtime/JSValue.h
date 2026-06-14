#pragma once

#include <string>
#include <variant>

class JSValue
{
public:
    using ValueType =
        std::variant<
            double,
            bool,
            std::string
        >;

    ValueType value;

    JSValue()
        : value(0.0)
    {
    }

    explicit JSValue(double number)
        : value(number)
    {
    }

    explicit JSValue(bool boolean)
        : value(boolean)
    {
    }

    explicit JSValue(
        const std::string& string)
        : value(string)
    {
    }
};