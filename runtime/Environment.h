#pragma once

#include <unordered_map>
#include <string>

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
        return variables.at(name);
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
        return functions.at(name);
    }
};