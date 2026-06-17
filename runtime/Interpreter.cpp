#define _USE_MATH_DEFINES
#include "Interpreter.h"
#include <variant>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <sstream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

// ========== Helpers ==========

std::string numberToCleanString(double number)
{
    if (number == static_cast<long long>(number)
        && std::abs(number) < 1e15)
    {
        return std::to_string(
            static_cast<long long>(number));
    }

    std::ostringstream oss;
    oss << number;
    return oss.str();
}

std::string Interpreter::jsValueToString(
    const JSValue& value)
{
    if (value.isNumber())
    {
        return numberToCleanString(
            std::get<double>(value.value));
    }

    if (value.isBool())
    {
        return std::get<bool>(value.value)
            ? "true" : "false";
    }

    if (value.isString())
    {
        return std::get<std::string>(
            value.value);
    }

    if (value.isNull())
    {
        return "null";
    }

    if (value.isUndefined())
    {
        return "undefined";
    }

    if (value.isArray())
    {
        auto& arr = std::get<
            std::vector<JSValue>>(
                value.value);

        std::string result = "";
        for (size_t i = 0; i < arr.size(); i++)
        {
            if (i > 0) result += ",";
            result += jsValueToString(arr[i]);
        }
        return result;
    }

    if (value.isObject())
    {
        auto& obj = std::get<
            std::shared_ptr<JSObject>>(
                value.value);

        std::string result = "{";
        bool first = true;
        for (auto& [k, v] : obj->properties)
        {
            if (!first) result += ", ";
            first = false;
            result += k + ": " +
                jsValueToString(v);
        }
        result += "}";
        return result;
    }

    if (value.isFunction())
    {
        return "[Function]";
    }

    return "undefined";
}

bool isTruthy(const JSValue& value)
{
    if (value.isBool())
    {
        return std::get<bool>(value.value);
    }

    if (value.isNumber())
    {
        double n = std::get<double>(
            value.value);
        return n != 0 && !std::isnan(n);
    }

    if (value.isString())
    {
        return !std::get<std::string>(
            value.value).empty();
    }

    if (value.isNull() || value.isUndefined())
    {
        return false;
    }

    // Objects, arrays, functions are truthy
    return true;
}

bool jsValuesEqual(
    const JSValue& a, const JSValue& b)
{
    // Same type comparisons
    if (a.isNumber() && b.isNumber())
    {
        return std::get<double>(a.value) ==
               std::get<double>(b.value);
    }

    if (a.isString() && b.isString())
    {
        return std::get<std::string>(a.value) ==
               std::get<std::string>(b.value);
    }

    if (a.isBool() && b.isBool())
    {
        return std::get<bool>(a.value) ==
               std::get<bool>(b.value);
    }

    if (a.isNull() && b.isNull())
        return true;

    if (a.isUndefined() && b.isUndefined())
        return true;

    // null == undefined is true in JS (loose)
    if ((a.isNull() && b.isUndefined()) ||
        (a.isUndefined() && b.isNull()))
        return true;

    return false;
}

// ========== Evaluate Expressions ==========

JSValue Interpreter::evaluate(
    Expression* expression)
{
    if (!expression)
    {
        throw std::runtime_error(
            "Null expression");
    }

    // ----- Unary -----
    if (auto unary =
        dynamic_cast<
            UnaryExpression*>(
                expression))
    {
        if (unary->op == "typeof")
        {
            auto val = evaluate(
                unary->operand.get());

            if (val.isNumber())
                return JSValue(
                    std::string("number"));
            if (val.isBool())
                return JSValue(
                    std::string("boolean"));
            if (val.isString())
                return JSValue(
                    std::string("string"));
            if (val.isFunction())
                return JSValue(
                    std::string("function"));
            if (val.isNull())
                return JSValue(
                    std::string("object"));
            if (val.isUndefined())
                return JSValue(
                    std::string("undefined"));
            if (val.isObject())
                return JSValue(
                    std::string("object"));
            if (val.isArray())
                return JSValue(
                    std::string("object"));

            return JSValue(
                std::string("undefined"));
        }

        if (unary->op == "!")
        {
            auto val = evaluate(
                unary->operand.get());
            return JSValue(!isTruthy(val));
        }

        if (unary->op == "-")
        {
            auto val = evaluate(
                unary->operand.get());

            if (val.isNumber())
            {
                return JSValue(
                    -std::get<double>(
                        val.value));
            }

            return JSValue(
                std::numeric_limits<double>::quiet_NaN());
        }

        // ++ and --
        auto identifier =
            dynamic_cast<
                Identifier*>(
                    unary->operand.get()
                );

        if (identifier)
        {
            JSValue current =
                environment.get(
                    identifier->name
                );

            double value =
                std::get<double>(
                    current.value
                );

            if (unary->op == "++")
            {
                value++;
                JSValue result(value);
                environment.assign(
                    identifier->name,
                    result);
                return result;
            }

            if (unary->op == "--")
            {
                value--;
                JSValue result(value);
                environment.assign(
                    identifier->name,
                    result);
                return result;
            }
        }
    }

    // ----- Number -----
    if (auto number =
        dynamic_cast<
            NumberLiteral*>(
                expression))
    {
        return JSValue(
            number->value
        );
    }

    // ----- Boolean -----
    if (auto boolean =
        dynamic_cast<
            BooleanLiteral*>(
                expression))
    {
        return JSValue(
            boolean->value
        );
    }

    // ----- String -----
    if (auto stringLiteral =
        dynamic_cast<
            StringLiteral*>(
                expression))
    {
        return JSValue(
            stringLiteral->value
        );
    }

    // ----- Null -----
    if (dynamic_cast<NullLiteral*>(
            expression))
    {
        return JSValue::makeNull();
    }

    // ----- Undefined -----
    if (dynamic_cast<UndefinedLiteral*>(
            expression))
    {
        return JSValue::makeUndefined();
    }

    // ----- Identifier -----
    if (auto identifier =
        dynamic_cast<
            Identifier*>(
                expression))
    {
        return environment.get(
            identifier->name
        );
    }

    // ----- Ternary -----
    if (auto ternary =
        dynamic_cast<
            TernaryExpression*>(
                expression))
    {
        auto cond = evaluate(
            ternary->condition.get());

        if (isTruthy(cond))
        {
            return evaluate(
                ternary->consequent.get());
        }
        else
        {
            return evaluate(
                ternary->alternate.get());
        }
    }

    // ----- Binary -----
    if (auto binary =
        dynamic_cast<
            BinaryExpression*>(
                expression))
    {
        // Short-circuit for && and ||
        if (binary->op == "&&")
        {
            auto left = evaluate(
                binary->left.get());
            if (!isTruthy(left))
                return left;
            return evaluate(
                binary->right.get());
        }

        if (binary->op == "||")
        {
            auto left = evaluate(
                binary->left.get());
            if (isTruthy(left))
                return left;
            return evaluate(
                binary->right.get());
        }

        JSValue left =
            evaluate(
                binary->left.get()
            );

        JSValue right =
            evaluate(
                binary->right.get()
            );

        // Equality operators (work on all types)
        if (binary->op == "===" ||
            binary->op == "==")
        {
            return JSValue(
                jsValuesEqual(left, right));
        }

        if (binary->op == "!==" ||
            binary->op == "!=")
        {
            return JSValue(
                !jsValuesEqual(left, right));
        }

        // String concatenation
        if (binary->op == "+")
        {
            if (left.isString() ||
                right.isString())
            {
                std::string lhs =
                    jsValueToString(left);
                std::string rhs =
                    jsValueToString(right);

                return JSValue(lhs + rhs);
            }
        }

        // String comparison for relational ops
        if (left.isString() && right.isString())
        {
            auto& lstr = std::get<std::string>(
                left.value);
            auto& rstr = std::get<std::string>(
                right.value);

            if (binary->op == ">")
                return JSValue(lstr > rstr);
            if (binary->op == "<")
                return JSValue(lstr < rstr);
            if (binary->op == ">=")
                return JSValue(lstr >= rstr);
            if (binary->op == "<=")
                return JSValue(lstr <= rstr);
        }

        // Numeric operations
        double lhs = 0;
        double rhs = 0;

        if (left.isNumber())
            lhs = std::get<double>(
                left.value);
        else if (left.isBool())
            lhs = std::get<bool>(
                left.value) ? 1.0 : 0.0;

        if (right.isNumber())
            rhs = std::get<double>(
                right.value);
        else if (right.isBool())
            rhs = std::get<bool>(
                right.value) ? 1.0 : 0.0;

        if (binary->op == "+")
            return JSValue(lhs + rhs);

        if (binary->op == "-")
            return JSValue(lhs - rhs);

        if (binary->op == "*")
            return JSValue(lhs * rhs);

        if (binary->op == "**")
            return JSValue(
                std::pow(lhs, rhs));

        if (binary->op == "/")
            return JSValue(lhs / rhs);

        if (binary->op == "%")
            return JSValue(
                std::fmod(lhs, rhs));

        if (binary->op == ">")
            return JSValue(lhs > rhs);

        if (binary->op == "<")
            return JSValue(lhs < rhs);

        if (binary->op == ">=")
            return JSValue(lhs >= rhs);

        if (binary->op == "<=")
            return JSValue(lhs <= rhs);
    }

    // ----- Call Expression -----
    if (auto call =
        dynamic_cast<
            CallExpression*>(
                expression))
    {
        // Check what kind of callee
        auto memberCallee =
            dynamic_cast<MemberExpression*>(
                call->callee.get());

        auto identCallee =
            dynamic_cast<Identifier*>(
                call->callee.get());

        if (memberCallee)
        {
            // Method call: obj.method(args)
            std::string method =
                memberCallee->property;

            // Check for static builtins first
            auto staticObj =
                dynamic_cast<Identifier*>(
                    memberCallee->object.get());

            if (staticObj)
            {
                std::string fullName =
                    staticObj->name + "." +
                    method;

                // Evaluate arguments
                std::vector<JSValue> args;
                for (auto& arg :
                    call->arguments)
                {
                    args.push_back(
                        evaluate(arg.get()));
                }

                // console.log
                if (fullName == "console.log")
                {
                    for (size_t i = 0;
                         i < args.size(); i++)
                    {
                        if (i > 0)
                            std::cout << " ";
                        std::cout <<
                            jsValueToString(
                                args[i]);
                    }
                    std::cout << std::endl;
                    return JSValue::makeUndefined();
                }

                // Math functions
                if (fullName == "Math.floor")
                {
                    return JSValue(std::floor(
                        std::get<double>(
                            args[0].value)));
                }

                if (fullName == "Math.ceil")
                {
                    return JSValue(std::ceil(
                        std::get<double>(
                            args[0].value)));
                }

                if (fullName == "Math.round")
                {
                    return JSValue(std::round(
                        std::get<double>(
                            args[0].value)));
                }

                if (fullName == "Math.abs")
                {
                    return JSValue(std::abs(
                        std::get<double>(
                            args[0].value)));
                }

                if (fullName == "Math.sqrt")
                {
                    return JSValue(std::sqrt(
                        std::get<double>(
                            args[0].value)));
                }

                if (fullName == "Math.pow")
                {
                    return JSValue(std::pow(
                        std::get<double>(
                            args[0].value),
                        std::get<double>(
                            args[1].value)));
                }

                if (fullName == "Math.min")
                {
                    double result =
                        std::get<double>(
                            args[0].value);
                    for (size_t i = 1;
                         i < args.size(); i++)
                    {
                        double v =
                            std::get<double>(
                                args[i].value);
                        if (v < result)
                            result = v;
                    }
                    return JSValue(result);
                }

                if (fullName == "Math.max")
                {
                    double result =
                        std::get<double>(
                            args[0].value);
                    for (size_t i = 1;
                         i < args.size(); i++)
                    {
                        double v =
                            std::get<double>(
                                args[i].value);
                        if (v > result)
                            result = v;
                    }
                    return JSValue(result);
                }

                if (fullName == "Math.random")
                {
                    return JSValue(
                        static_cast<double>(
                            rand()) / RAND_MAX);
                }

                // Date.now()
                if (fullName == "Date.now")
                {
                    auto now = std::chrono::
                        system_clock::now();
                    auto ms = std::chrono::
                        duration_cast<
                            std::chrono::
                                milliseconds>(
                            now.time_since_epoch()
                        ).count();
                    return JSValue(
                        static_cast<double>(ms));
                }

                // Object.keys / Object.values
                if (fullName == "Object.keys" &&
                    !args.empty() &&
                    args[0].isObject())
                {
                    auto& obj = std::get<
                        std::shared_ptr<JSObject>>(
                            args[0].value);
                    std::vector<JSValue> keys;
                    for (auto& [k, v] :
                        obj->properties)
                    {
                        keys.push_back(
                            JSValue(k));
                    }
                    return JSValue(keys);
                }

                if (fullName == "Object.values" &&
                    !args.empty() &&
                    args[0].isObject())
                {
                    auto& obj = std::get<
                        std::shared_ptr<JSObject>>(
                            args[0].value);
                    std::vector<JSValue> vals;
                    for (auto& [k, v] :
                        obj->properties)
                    {
                        vals.push_back(v);
                    }
                    return JSValue(vals);
                }

                // Try as variable method call
                if (environment.exists(
                    staticObj->name))
                {
                    auto objVal =
                        environment.get(
                            staticObj->name);

                    auto result = callMethod(
                        objVal, method, args);

                    // Write back for mutating methods
                    environment.assign(
                        staticObj->name,
                        objVal);

                    return result;
                }
            }

            // Method call on expression result
            auto objVal = evaluate(
                memberCallee->object.get());

            std::vector<JSValue> args;
            for (auto& arg : call->arguments)
            {
                args.push_back(
                    evaluate(arg.get()));
            }

            return callMethod(
                objVal, method, args);
        }

        if (identCallee)
        {
            std::string name =
                identCallee->name;

            // Evaluate arguments
            std::vector<JSValue> args;
            for (auto& arg : call->arguments)
            {
                args.push_back(
                    evaluate(arg.get()));
            }

            // Check if calling a function value variable
            if (environment.exists(name))
            {
                auto val = environment.get(name);
                if (val.isFunction())
                {
                    auto fn = std::get<
                        std::shared_ptr<JSFunction>>(
                            val.value);
                    return callJSFunction(
                        fn, args);
                }
            }

            return callFunction(name, args);
        }

        // Calling a non-identifier expression
        // (could be a function expression)
        auto calleeVal = evaluate(
            call->callee.get());

        if (calleeVal.isFunction())
        {
            auto fn = std::get<
                std::shared_ptr<JSFunction>>(
                    calleeVal.value);

            std::vector<JSValue> args;
            for (auto& arg : call->arguments)
            {
                args.push_back(
                    evaluate(arg.get()));
            }

            return callJSFunction(fn, args);
        }

        throw std::runtime_error(
            "Not a function");
    }

    // ----- Assignment -----
    if (auto assignment =
        dynamic_cast<
            AssignmentExpression*>(
                expression))
    {
        JSValue value =
            evaluate(
                assignment
                    ->value.get()
            );

        environment.assign(
            assignment
                ->variableName,
            value
        );

        return value;
    }

    // ----- Member Assignment (obj.prop = val) -----
    if (auto memAssign =
        dynamic_cast<
            MemberAssignmentExpression*>(
                expression))
    {
        auto objVal = evaluate(
            memAssign->object.get());

        auto newVal = evaluate(
            memAssign->value.get());

        if (objVal.isObject())
        {
            auto& obj = std::get<
                std::shared_ptr<JSObject>>(
                    objVal.value);
            obj->set(
                memAssign->property, newVal);

            // Also update the variable
            // if the object came from a variable
            auto ident = dynamic_cast<
                Identifier*>(
                    memAssign->object.get());
            if (ident)
            {
                environment.assign(
                    ident->name, objVal);
            }
        }

        return newVal;
    }

    // ----- Index Assignment (arr[i] = val) -----
    if (auto idxAssign =
        dynamic_cast<
            IndexAssignmentExpression*>(
                expression))
    {
        auto objVal = evaluate(
            idxAssign->object.get());

        auto idxVal = evaluate(
            idxAssign->index.get());

        auto newVal = evaluate(
            idxAssign->value.get());

        if (objVal.isArray() &&
            idxVal.isNumber())
        {
            auto& arr = std::get<
                std::vector<JSValue>>(
                    objVal.value);

            int idx = static_cast<int>(
                std::get<double>(
                    idxVal.value));

            // Auto-grow array if index is
            // beyond current size (JS behavior)
            if (idx >= 0 &&
                idx >= static_cast<int>(
                    arr.size()))
            {
                arr.resize(
                    idx + 1,
                    JSValue::makeUndefined());
            }

            if (idx >= 0 &&
                idx < static_cast<int>(
                    arr.size()))
            {
                arr[idx] = newVal;
            }

            // Write back to the root variable
            // Walk up the object chain for
            // nested access like a[i][j] = val
            Expression* root =
                idxAssign->object.get();

            while (auto nested =
                dynamic_cast<IndexExpression*>(
                    root))
            {
                root = nested->object.get();
            }

            auto ident = dynamic_cast<
                Identifier*>(root);
            if (ident)
            {
                // For nested access, re-read
                // the root and update in place
                if (root !=
                    idxAssign->object.get())
                {
                    // Nested: re-evaluate and
                    // assign at each level
                    auto rootVal =
                        environment.get(
                            ident->name);

                    // Re-evaluate and set
                    // through the chain
                    std::vector<int> indices;
                    Expression* cur =
                        idxAssign->object.get();

                    while (auto ie =
                        dynamic_cast<
                            IndexExpression*>(
                                cur))
                    {
                        auto iv = evaluate(
                            ie->index.get());
                        indices.push_back(
                            static_cast<int>(
                                std::get<double>(
                                    iv.value)));
                        cur = ie->object.get();
                    }

                    // Walk from root to target
                    JSValue* target = &rootVal;
                    for (int i =
                        static_cast<int>(
                            indices.size()) - 1;
                        i >= 0; i--)
                    {
                        if (target->isArray())
                        {
                            auto& a = std::get<
                                std::vector<
                                    JSValue>>(
                                target->value);

                            int ai = indices[i];
                            if (ai >= 0 &&
                                ai >=
                                static_cast<int>(
                                    a.size()))
                            {
                                a.resize(
                                    ai + 1,
                                    JSValue::
                                    makeUndefined()
                                );
                            }
                            if (ai >= 0 &&
                                ai <
                                static_cast<int>(
                                    a.size()))
                            {
                                target = &a[ai];
                            }
                        }
                    }

                    // Now set final index
                    if (target->isArray())
                    {
                        auto& a = std::get<
                            std::vector<JSValue>>(
                            target->value);

                        int fi =
                            static_cast<int>(
                                std::get<double>(
                                    idxVal.value));
                        if (fi >= 0 &&
                            fi >=
                            static_cast<int>(
                                a.size()))
                        {
                            a.resize(
                                fi + 1,
                                JSValue::
                                makeUndefined());
                        }
                        if (fi >= 0 &&
                            fi <
                            static_cast<int>(
                                a.size()))
                        {
                            a[fi] = newVal;
                        }
                    }

                    environment.assign(
                        ident->name, rootVal);
                }
                else
                {
                    environment.assign(
                        ident->name, objVal);
                }
            }
        }

        if (objVal.isObject() &&
            idxVal.isString())
        {
            auto& obj = std::get<
                std::shared_ptr<JSObject>>(
                    objVal.value);
            obj->set(
                std::get<std::string>(
                    idxVal.value),
                newVal);

            auto ident = dynamic_cast<
                Identifier*>(
                    idxAssign->object.get());
            if (ident)
            {
                environment.assign(
                    ident->name, objVal);
            }
        }

        return newVal;
    }

    // ----- Member Expression (property access) -----
    if (auto member =
        dynamic_cast<
            MemberExpression*>(
                expression))
    {
        // Check for Math.PI, Math.E
        auto staticObj =
            dynamic_cast<Identifier*>(
                member->object.get());

        if (staticObj)
        {
            if (staticObj->name == "Math")
            {
                if (member->property == "PI")
                    return JSValue(M_PI);
                if (member->property == "E")
                    return JSValue(M_E);
            }
        }

        auto objVal = evaluate(
            member->object.get());

        return getProperty(
            objVal, member->property);
    }

    // ----- Index Expression -----
    if (auto indexExpr =
        dynamic_cast<
            IndexExpression*>(
                expression))
    {
        auto objVal = evaluate(
            indexExpr->object.get());

        auto idxVal = evaluate(
            indexExpr->index.get());

        if (objVal.isArray() &&
            idxVal.isNumber())
        {
            auto& arr = std::get<
                std::vector<JSValue>>(
                    objVal.value);
            int idx = static_cast<int>(
                std::get<double>(
                    idxVal.value));
            if (idx >= 0 &&
                idx < static_cast<int>(
                    arr.size()))
            {
                return arr[idx];
            }
            return JSValue::makeUndefined();
        }

        if (objVal.isString() &&
            idxVal.isNumber())
        {
            auto& str = std::get<
                std::string>(objVal.value);
            int idx = static_cast<int>(
                std::get<double>(
                    idxVal.value));
            if (idx >= 0 &&
                idx < static_cast<int>(
                    str.size()))
            {
                return JSValue(
                    std::string(1, str[idx]));
            }
            return JSValue::makeUndefined();
        }

        if (objVal.isObject() &&
            idxVal.isString())
        {
            auto& obj = std::get<
                std::shared_ptr<JSObject>>(
                    objVal.value);
            auto& key = std::get<
                std::string>(idxVal.value);
            auto* found = obj->find(key);
            if (found)
                return *found;
            return JSValue::makeUndefined();
        }

        return JSValue::makeUndefined();
    }

    // ----- Array Literal -----
    if (auto array =
        dynamic_cast<
            ArrayLiteral*>(
                expression))
    {
        std::vector<JSValue> values;

        for (auto& element :
            array->elements)
        {
            values.push_back(
                evaluate(
                    element.get()
                )
            );
        }

        return JSValue(values);
    }

    // ----- Object Literal -----
    if (auto objLit =
        dynamic_cast<
            ObjectLiteral*>(
                expression))
    {
        auto obj = std::make_shared<
            JSObject>();

        for (size_t i = 0;
             i < objLit->keys.size(); i++)
        {
            obj->set(
                objLit->keys[i],
                evaluate(
                    objLit->values[i].get()
                ));
        }

        return JSValue(obj);
    }

    // ----- Arrow Function Expression -----
    if (auto arrow =
        dynamic_cast<
            ArrowFunctionExpression*>(
                expression))
    {
        auto fn = std::make_shared<
            JSFunction>();

        fn->parameters = arrow->parameters;
        fn->body = arrow->body.get();
        fn->expressionBody =
            arrow->expressionBody.get();

        return JSValue(fn);
    }

    // ----- Function Expression -----
    if (auto funcExpr =
        dynamic_cast<
            FunctionExpression*>(
                expression))
    {
        auto fn = std::make_shared<
            JSFunction>();

        fn->name = funcExpr->name;
        fn->parameters =
            funcExpr->parameters;
        fn->body = funcExpr->body.get();

        return JSValue(fn);
    }

    throw std::runtime_error(
        "Unsupported expression");
}

// ========== Execute Statements ==========

void Interpreter::execute(
    Statement* statement)
{
    if (!statement)
    {
        return;
    }

    if (breakFlag || continueFlag ||
        returnValue.has_value())
    {
        return;
    }

    // ----- Variable Declaration -----
    if (auto variable =
        dynamic_cast<
            VariableDeclaration*>(
                statement))
    {
        JSValue value =
            evaluate(
                variable->initializer.get()
            );

        environment.define(
            variable->name,
            value
        );

        return;
    }

    // ----- If Statement -----
    if (auto ifStatement =
        dynamic_cast<
            IfStatement*>(
                statement))
    {
        JSValue condition =
            evaluate(
                ifStatement
                    ->condition.get()
            );

        if (isTruthy(condition))
        {
            if (ifStatement->thenBranch)
            {
                for (auto& stmt :
                    ifStatement
                        ->thenBranch
                        ->statements)
                {
                    execute(stmt.get());
                    if (breakFlag ||
                        continueFlag ||
                        returnValue.has_value())
                        return;
                }
            }
        }
        else
        {
            if (ifStatement->elseBranch)
            {
                for (auto& stmt :
                    ifStatement
                        ->elseBranch
                        ->statements)
                {
                    execute(stmt.get());
                    if (breakFlag ||
                        continueFlag ||
                        returnValue.has_value())
                        return;
                }
            }
        }

        return;
    }

    // ----- Expression Statement -----
    if (auto expressionStatement =
        dynamic_cast<
            ExpressionStatement*>(
                statement))
    {
        evaluate(
            expressionStatement
                ->expression.get()
        );

        return;
    }

    // ----- While Statement -----
    if (auto whileStatement =
        dynamic_cast<
            WhileStatement*>(
                statement))
    {
        int safety = 0;

        while (
            isTruthy(
                evaluate(
                    whileStatement
                        ->condition.get()
                )
            )
        )
        {
            if (whileStatement->body)
            {
                for (auto& stmt :
                    whileStatement
                        ->body
                        ->statements)
                {
                    execute(stmt.get());

                    if (returnValue.has_value())
                        return;

                    if (breakFlag)
                    {
                        breakFlag = false;
                        return;
                    }

                    if (continueFlag)
                    {
                        continueFlag = false;
                        break;
                    }
                }
            }

            safety++;
            if (safety > 100000) break;
        }

        return;
    }

    // ----- Do-While Statement -----
    if (auto doWhile =
        dynamic_cast<
            DoWhileStatement*>(
                statement))
    {
        int safety = 0;

        do
        {
            if (doWhile->body)
            {
                for (auto& stmt :
                    doWhile->body
                        ->statements)
                {
                    execute(stmt.get());

                    if (returnValue.has_value())
                        return;

                    if (breakFlag)
                    {
                        breakFlag = false;
                        return;
                    }

                    if (continueFlag)
                    {
                        continueFlag = false;
                        break;
                    }
                }
            }

            safety++;
            if (safety > 100000) break;
        }
        while (
            isTruthy(
                evaluate(
                    doWhile->condition.get()
                )
            )
        );

        return;
    }

    // ----- For Statement -----
    if (auto forStatement =
        dynamic_cast<
            ForStatement*>(
                statement))
    {
        environment.pushScope();

        if (forStatement->initializer)
        {
            execute(
                forStatement
                    ->initializer.get()
            );
        }

        int safetyCounter = 0;

        while (
            !forStatement->condition
            ||
            isTruthy(
                evaluate(
                    forStatement
                        ->condition.get()
                )
            )
        )
        {
            if (forStatement->body)
            {
                for (auto& stmt :
                    forStatement
                        ->body
                        ->statements)
                {
                    execute(stmt.get());

                    if (returnValue.has_value())
                    {
                        environment.popScope();
                        return;
                    }

                    if (breakFlag)
                    {
                        breakFlag = false;
                        environment.popScope();
                        return;
                    }

                    if (continueFlag)
                    {
                        continueFlag = false;
                        break;
                    }
                }
            }

            if (forStatement->increment)
            {
                evaluate(
                    forStatement
                        ->increment.get()
                );
            }

            safetyCounter++;

            if (safetyCounter > 100000)
            {
                break;
            }
        }

        environment.popScope();
        return;
    }

    // ----- Function Declaration -----
    if (auto function =
        dynamic_cast<
            FunctionDeclaration*>(
                statement))
    {
        environment.defineFunction(
            function->name,
            function
        );

        return;
    }

    // ----- Return Statement -----
    if (auto returnStatement =
        dynamic_cast<
            ReturnStatement*>(
                statement))
    {
        returnValue =
            evaluate(
                returnStatement
                    ->value.get()
            );

        return;
    }

    // ----- Break -----
    if (dynamic_cast<BreakStatement*>(
            statement))
    {
        breakFlag = true;
        return;
    }

    // ----- Continue -----
    if (dynamic_cast<ContinueStatement*>(
            statement))
    {
        continueFlag = true;
        return;
    }
}

void Interpreter::executeProgram(
    Program* program)
{
    if (!program)
    {
        return;
    }

    for (auto& statement :
         program->statements)
    {
        execute(
            statement.get()
        );
    }
}

// ========== Property Access ==========

JSValue Interpreter::getProperty(
    JSValue& object,
    const std::string& property)
{
    if (property == "length")
    {
        if (object.isArray())
        {
            return JSValue(
                static_cast<double>(
                    std::get<
                        std::vector<JSValue>>(
                            object.value
                    ).size()
                )
            );
        }

        if (object.isString())
        {
            return JSValue(
                static_cast<double>(
                    std::get<std::string>(
                        object.value
                    ).size()
                )
            );
        }
    }

    if (object.isObject())
    {
        auto& obj = std::get<
            std::shared_ptr<JSObject>>(
                object.value);

        auto* found = obj->find(property);

        if (found)
        {
            return *found;
        }
    }

    return JSValue::makeUndefined();
}

// ========== Method Dispatch ==========

JSValue Interpreter::callMethod(
    JSValue& object,
    const std::string& method,
    const std::vector<JSValue>& arguments)
{
    // ===== Array Methods =====
    if (object.isArray())
    {
        auto& arr = std::get<
            std::vector<JSValue>>(
                object.value);

        if (method == "length")
        {
            return JSValue(
                static_cast<double>(
                    arr.size()));
        }

        if (method == "push")
        {
            for (auto& arg : arguments)
            {
                arr.push_back(arg);
            }
            return JSValue(
                static_cast<double>(
                    arr.size()));
        }

        if (method == "pop")
        {
            if (arr.empty())
                return JSValue::makeUndefined();
            auto val = arr.back();
            arr.pop_back();
            return val;
        }

        if (method == "shift")
        {
            if (arr.empty())
                return JSValue::makeUndefined();
            auto val = arr.front();
            arr.erase(arr.begin());
            return val;
        }

        if (method == "unshift")
        {
            for (int i =
                static_cast<int>(
                    arguments.size()) - 1;
                 i >= 0; i--)
            {
                arr.insert(
                    arr.begin(),
                    arguments[i]);
            }
            return JSValue(
                static_cast<double>(
                    arr.size()));
        }

        if (method == "reverse")
        {
            auto copy = arr;
            std::reverse(
                copy.begin(), copy.end());
            return JSValue(copy);
        }

        if (method == "join")
        {
            std::string separator = ",";

            if (!arguments.empty() &&
                arguments[0].isString())
            {
                separator = std::get<
                    std::string>(
                        arguments[0].value);
            }

            std::string result;

            for (size_t i = 0;
                 i < arr.size(); i++)
            {
                if (i > 0) result += separator;
                result += jsValueToString(
                    arr[i]);
            }

            return JSValue(result);
        }

        if (method == "indexOf")
        {
            if (arguments.empty())
                return JSValue(-1.0);

            for (size_t i = 0;
                 i < arr.size(); i++)
            {
                if (jsValuesEqual(
                    arr[i], arguments[0]))
                {
                    return JSValue(
                        static_cast<double>(i));
                }
            }
            return JSValue(-1.0);
        }

        if (method == "includes")
        {
            if (arguments.empty())
                return JSValue(false);

            for (auto& elem : arr)
            {
                if (jsValuesEqual(
                    elem, arguments[0]))
                {
                    return JSValue(true);
                }
            }
            return JSValue(false);
        }

        if (method == "slice")
        {
            int start = 0;
            int end = static_cast<int>(
                arr.size());

            if (!arguments.empty() &&
                arguments[0].isNumber())
            {
                start = static_cast<int>(
                    std::get<double>(
                        arguments[0].value));
            }

            if (arguments.size() > 1 &&
                arguments[1].isNumber())
            {
                end = static_cast<int>(
                    std::get<double>(
                        arguments[1].value));
            }

            if (start < 0)
                start += static_cast<int>(
                    arr.size());
            if (end < 0)
                end += static_cast<int>(
                    arr.size());

            std::vector<JSValue> result;

            for (int i = start;
                 i < end &&
                 i < static_cast<int>(
                     arr.size());
                 i++)
            {
                if (i >= 0)
                    result.push_back(arr[i]);
            }

            return JSValue(result);
        }

        if (method == "concat")
        {
            auto result = arr;

            for (auto& arg : arguments)
            {
                if (arg.isArray())
                {
                    auto& other = std::get<
                        std::vector<JSValue>>(
                            arg.value);
                    result.insert(
                        result.end(),
                        other.begin(),
                        other.end());
                }
                else
                {
                    result.push_back(arg);
                }
            }

            return JSValue(result);
        }

        if (method == "map")
        {
            if (arguments.empty() ||
                !arguments[0].isFunction())
            {
                throw std::runtime_error(
                    "map requires a function");
            }

            auto fn = std::get<
                std::shared_ptr<JSFunction>>(
                    arguments[0].value);

            std::vector<JSValue> result;

            for (size_t i = 0;
                 i < arr.size(); i++)
            {
                std::vector<JSValue> cbArgs;
                cbArgs.push_back(arr[i]);
                cbArgs.push_back(
                    JSValue(
                        static_cast<double>(i)));

                result.push_back(
                    callJSFunction(fn, cbArgs));
            }

            return JSValue(result);
        }

        if (method == "filter")
        {
            if (arguments.empty() ||
                !arguments[0].isFunction())
            {
                throw std::runtime_error(
                    "filter requires a function");
            }

            auto fn = std::get<
                std::shared_ptr<JSFunction>>(
                    arguments[0].value);

            std::vector<JSValue> result;

            for (size_t i = 0;
                 i < arr.size(); i++)
            {
                std::vector<JSValue> cbArgs;
                cbArgs.push_back(arr[i]);
                cbArgs.push_back(
                    JSValue(
                        static_cast<double>(i)));

                auto ret = callJSFunction(
                    fn, cbArgs);

                if (isTruthy(ret))
                {
                    result.push_back(arr[i]);
                }
            }

            return JSValue(result);
        }

        if (method == "reduce")
        {
            if (arguments.empty() ||
                !arguments[0].isFunction())
            {
                throw std::runtime_error(
                    "reduce requires a function");
            }

            auto fn = std::get<
                std::shared_ptr<JSFunction>>(
                    arguments[0].value);

            JSValue accumulator;
            size_t startIdx = 0;

            if (arguments.size() > 1)
            {
                accumulator = arguments[1];
            }
            else if (!arr.empty())
            {
                accumulator = arr[0];
                startIdx = 1;
            }

            for (size_t i = startIdx;
                 i < arr.size(); i++)
            {
                std::vector<JSValue> cbArgs;
                cbArgs.push_back(accumulator);
                cbArgs.push_back(arr[i]);
                cbArgs.push_back(
                    JSValue(
                        static_cast<double>(i)));

                accumulator =
                    callJSFunction(fn, cbArgs);
            }

            return accumulator;
        }

        if (method == "forEach")
        {
            if (arguments.empty() ||
                !arguments[0].isFunction())
            {
                throw std::runtime_error(
                    "forEach requires a function");
            }

            auto fn = std::get<
                std::shared_ptr<JSFunction>>(
                    arguments[0].value);

            for (size_t i = 0;
                 i < arr.size(); i++)
            {
                std::vector<JSValue> cbArgs;
                cbArgs.push_back(arr[i]);
                cbArgs.push_back(
                    JSValue(
                        static_cast<double>(i)));

                callJSFunction(fn, cbArgs);
            }

            return JSValue::makeUndefined();
        }

        if (method == "find")
        {
            if (arguments.empty() ||
                !arguments[0].isFunction())
            {
                throw std::runtime_error(
                    "find requires a function");
            }

            auto fn = std::get<
                std::shared_ptr<JSFunction>>(
                    arguments[0].value);

            for (size_t i = 0;
                 i < arr.size(); i++)
            {
                std::vector<JSValue> cbArgs;
                cbArgs.push_back(arr[i]);
                cbArgs.push_back(
                    JSValue(
                        static_cast<double>(i)));

                auto ret = callJSFunction(
                    fn, cbArgs);

                if (isTruthy(ret))
                {
                    return arr[i];
                }
            }

            return JSValue::makeUndefined();
        }
    }

    // ===== String Methods =====
    if (object.isString())
    {
        auto& str = std::get<std::string>(
            object.value);

        if (method == "split")
        {
            std::string separator = "";

            if (!arguments.empty() &&
                arguments[0].isString())
            {
                separator = std::get<
                    std::string>(
                        arguments[0].value);
            }

            std::vector<JSValue> result;

            if (separator.empty())
            {
                // Split into chars
                for (char c : str)
                {
                    result.push_back(
                        JSValue(
                            std::string(1, c)));
                }
            }
            else
            {
                size_t pos = 0;
                size_t found;

                while ((found =
                    str.find(separator, pos))
                    != std::string::npos)
                {
                    result.push_back(
                        JSValue(str.substr(
                            pos,
                            found - pos)));
                    pos = found +
                        separator.length();
                }

                result.push_back(
                    JSValue(str.substr(pos)));
            }

            return JSValue(result);
        }

        if (method == "toUpperCase")
        {
            std::string result = str;
            std::transform(
                result.begin(),
                result.end(),
                result.begin(),
                ::toupper);
            return JSValue(result);
        }

        if (method == "toLowerCase")
        {
            std::string result = str;
            std::transform(
                result.begin(),
                result.end(),
                result.begin(),
                ::tolower);
            return JSValue(result);
        }

        if (method == "charAt")
        {
            if (!arguments.empty() &&
                arguments[0].isNumber())
            {
                int idx = static_cast<int>(
                    std::get<double>(
                        arguments[0].value));
                if (idx >= 0 &&
                    idx < static_cast<int>(
                        str.size()))
                {
                    return JSValue(
                        std::string(
                            1, str[idx]));
                }
            }
            return JSValue(std::string(""));
        }

        if (method == "indexOf")
        {
            if (!arguments.empty() &&
                arguments[0].isString())
            {
                auto& search = std::get<
                    std::string>(
                        arguments[0].value);
                auto pos = str.find(search);
                if (pos != std::string::npos)
                {
                    return JSValue(
                        static_cast<double>(
                            pos));
                }
            }
            return JSValue(-1.0);
        }

        if (method == "includes")
        {
            if (!arguments.empty() &&
                arguments[0].isString())
            {
                auto& search = std::get<
                    std::string>(
                        arguments[0].value);
                return JSValue(
                    str.find(search) !=
                    std::string::npos);
            }
            return JSValue(false);
        }

        if (method == "trim")
        {
            std::string result = str;
            size_t start =
                result.find_first_not_of(
                    " \t\n\r");
            size_t end =
                result.find_last_not_of(
                    " \t\n\r");

            if (start == std::string::npos)
                return JSValue(
                    std::string(""));

            return JSValue(
                result.substr(
                    start,
                    end - start + 1));
        }

        if (method == "substring" ||
            method == "slice")
        {
            int start = 0;
            int end = static_cast<int>(
                str.size());

            if (!arguments.empty() &&
                arguments[0].isNumber())
            {
                start = static_cast<int>(
                    std::get<double>(
                        arguments[0].value));
            }

            if (arguments.size() > 1 &&
                arguments[1].isNumber())
            {
                end = static_cast<int>(
                    std::get<double>(
                        arguments[1].value));
            }

            if (start < 0)
                start += static_cast<int>(
                    str.size());
            if (end < 0)
                end += static_cast<int>(
                    str.size());

            if (start < 0) start = 0;
            if (end > static_cast<int>(
                str.size()))
                end = static_cast<int>(
                    str.size());

            if (start >= end)
                return JSValue(
                    std::string(""));

            return JSValue(
                str.substr(
                    start, end - start));
        }

        if (method == "length")
        {
            return JSValue(
                static_cast<double>(
                    str.size()));
        }
    }

    throw std::runtime_error(
        "Cannot call method '" + method +
        "' on this value");
}

// ========== Call Named Function ==========

JSValue Interpreter::callFunction(
    const std::string& name,
    const std::vector<JSValue>& arguments)
{
    auto function =
        environment.getFunction(
            name
        );

    environment.pushScope();

    returnValue.reset();

    for (
        size_t i = 0;
        i < function->parameters.size()
        &&
        i < arguments.size();
        i++
    )
    {
        environment.define(
            function->parameters[i],
            arguments[i]
        );
    }

    if (function->body)
    {
        for (auto& statement :
             function->body->statements)
        {
            execute(
                statement.get()
            );

            if (returnValue.has_value())
            {
                auto result = *returnValue;
                returnValue.reset();
                environment.popScope();
                return result;
            }
        }
    }

    environment.popScope();
    return JSValue::makeUndefined();
}

// ========== Call JS Function Value ==========

JSValue Interpreter::callJSFunction(
    std::shared_ptr<JSFunction> func,
    const std::vector<JSValue>& arguments)
{
    environment.pushScope();

    // Bind parameters
    for (size_t i = 0;
         i < func->parameters.size(); i++)
    {
        if (i < arguments.size())
        {
            environment.define(
                func->parameters[i],
                arguments[i]);
        }
        else
        {
            environment.define(
                func->parameters[i],
                JSValue::makeUndefined());
        }
    }

    // Expression body (arrow function)
    if (func->expressionBody)
    {
        auto result = evaluate(
            func->expressionBody);
        environment.popScope();
        return result;
    }

    // Block body
    if (func->body)
    {
        returnValue.reset();

        for (auto& stmt :
             func->body->statements)
        {
            execute(stmt.get());

            if (returnValue.has_value())
            {
                auto result = *returnValue;
                returnValue.reset();
                environment.popScope();
                return result;
            }
        }
    }

    environment.popScope();
    return JSValue::makeUndefined();
}
