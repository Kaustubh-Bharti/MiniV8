#pragma once

#include "../ast/AST.h"
#include <optional>
#include "Environment.h"
#include "JSValue.h"

class Interpreter
{
private:
    Environment environment;
    std::optional<JSValue> returnValue;

public:

    std::optional<JSValue>& getReturnValue()
    {
        return returnValue;
    }

    JSValue evaluate(
        Expression* expression
    );

    void execute(
        Statement* statement
    );

    Environment& getEnvironment()
    {
        return environment;
    }

    void executeProgram(
        Program* program
    );

    JSValue callFunction(
        const std::string& name,
        const std::vector<JSValue>& arguments
    );
};



