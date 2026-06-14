#pragma once

#include "../ast/AST.h"
#include <optional>
#include "Environment.h"
#include "JSValue.h"

// Signal types for control flow
struct BreakSignal {};
struct ContinueSignal {};

class Interpreter
{
private:
    Environment environment;
    std::optional<JSValue> returnValue;
    bool breakFlag = false;
    bool continueFlag = false;

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

    // Value-based method dispatch
    JSValue callMethod(
        JSValue& object,
        const std::string& method,
        const std::vector<JSValue>& arguments
    );

    // Property access on values
    JSValue getProperty(
        JSValue& object,
        const std::string& property
    );

    // Call a JSFunction value (arrow/callback)
    JSValue callJSFunction(
        std::shared_ptr<JSFunction> func,
        const std::vector<JSValue>& arguments
    );

    // Convert any JSValue to string for printing
    std::string jsValueToString(
        const JSValue& value
    );
};
