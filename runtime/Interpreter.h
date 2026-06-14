#pragma once

#include "../ast/AST.h"

#include "Environment.h"
#include "JSValue.h"

class Interpreter
{
private:
    Environment environment;

public:

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
};



