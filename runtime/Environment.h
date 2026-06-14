#pragma once

#include <unordered_map>
#include <string>
#include <stdexcept>
#include <vector>
#include "JSValue.h"

#include <memory>

class FunctionDeclaration;

class Environment
{
private:
    struct Scope
    {
        std::unordered_map<
            std::string,
            JSValue
        > variables;

        std::unordered_map<
            std::string,
            FunctionDeclaration*
        > functions;
    };

    std::vector<Scope> scopes;

public:
    Environment()
    {
        // Start with a global scope
        scopes.push_back({});
    }

    void pushScope()
    {
        scopes.push_back({});
    }

    void popScope()
    {
        if (scopes.size() > 1)
        {
            scopes.pop_back();
        }
    }

    void define(
        const std::string& name,
        const JSValue& value)
    {
        scopes.back().variables[name] =
            value;
    }

    void assign(
        const std::string& name,
        const JSValue& value)
    {
        // Search from innermost to outermost
        for (int i =
                static_cast<int>(
                    scopes.size()) - 1;
             i >= 0; i--)
        {
            auto it =
                scopes[i].variables.find(
                    name);

            if (it !=
                scopes[i].variables.end())
            {
                it->second = value;
                return;
            }
        }

        // If not found, define in current
        scopes.back().variables[name] =
            value;
    }

    bool exists(
        const std::string& name)
    {
        for (int i =
                static_cast<int>(
                    scopes.size()) - 1;
             i >= 0; i--)
        {
            if (scopes[i].variables.count(
                    name))
            {
                return true;
            }
        }
        return false;
    }

    JSValue get(
        const std::string& name)
    {
        for (int i =
                static_cast<int>(
                    scopes.size()) - 1;
             i >= 0; i--)
        {
            auto it =
                scopes[i].variables.find(
                    name);

            if (it !=
                scopes[i].variables.end())
            {
                return it->second;
            }
        }

        throw std::runtime_error(
            "Undefined variable: " + name
        );
    }

    void defineFunction(
        const std::string& name,
        FunctionDeclaration* function)
    {
        scopes.back().functions[name] =
            function;
    }

    FunctionDeclaration* getFunction(
        const std::string& name)
    {
        for (int i =
                static_cast<int>(
                    scopes.size()) - 1;
             i >= 0; i--)
        {
            auto it =
                scopes[i].functions.find(
                    name);

            if (it !=
                scopes[i].functions.end())
            {
                return it->second;
            }
        }

        throw std::runtime_error(
            "Undefined function: " + name
        );
    }

    bool hasFunction(
        const std::string& name)
    {
        for (int i =
                static_cast<int>(
                    scopes.size()) - 1;
             i >= 0; i--)
        {
            if (scopes[i].functions.count(
                    name))
            {
                return true;
            }
        }
        return false;
    }
};