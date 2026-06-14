#pragma once

#include <unordered_map>
#include <string>

#include "JSValue.h"

class Environment
{
private:
    std::unordered_map<
        std::string,
        JSValue
    > variables;

public:

    void define(
        const std::string& name,
        const JSValue& value)
    {
        variables[name] = value;
    }

    bool exists(
        const std::string& name)
    {
        return variables.find(name)
            != variables.end();
    }

    JSValue get(
        const std::string& name)
    {
        return variables.at(name);
    }
};