#pragma once

#include <unordered_map>
#include <string>
#include <stdexcept>
#include "JSValue.h"

#include <memory>

class FunctionDeclaration;

class Environment
{
private:
    std::unordered_map<
        std::string,
        JSValue
    > variables;

    std::unordered_map<
        std::string,
        FunctionDeclaration*
    > functions;

public:

    void define(
        const std::string& name,
        const JSValue& value)
    {
        variables[name] = value;
    }

    void assign(
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
        auto it =
            variables.find(name);

        if (it == variables.end())
        {
            throw std::runtime_error(
                "Undefined variable: " + name
            );
        }

        return it->second;
    }

    void defineFunction(
        const std::string& name,
        FunctionDeclaration* function)
    {
        functions[name] = function;
    }

    FunctionDeclaration* getFunction(
        const std::string& name)
    {
        auto it =
            functions.find(name);

        if (it == functions.end())
        {
            throw std::runtime_error(
                "Undefined function: " + name
            );
        }

        return it->second;
    }
};