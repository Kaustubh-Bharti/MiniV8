#pragma once

#include <string>
#include <variant>
#include <vector>
#include <map>
#include <memory>

// Forward declarations
class BlockStatement;
class Expression;
class Environment;

// Sentinel types for null and undefined
struct JSNull {};
struct JSUndefined {};

// Object: a string-keyed property map
struct JSObject;

// Function value: stores params + body for callbacks, arrow functions, etc.
struct JSFunction;

class JSValue
{
public:
    using ValueType =
        std::variant<
            double,
            bool,
            std::string,
            std::vector<JSValue>,
            JSNull,
            JSUndefined,
            std::shared_ptr<JSObject>,
            std::shared_ptr<JSFunction>
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

    explicit JSValue(
        const std::vector<JSValue>& array)
        : value(array)
    {
    }

    explicit JSValue(JSNull)
        : value(JSNull{})
    {
    }

    explicit JSValue(JSUndefined)
        : value(JSUndefined{})
    {
    }

    explicit JSValue(
        std::shared_ptr<JSObject> obj)
        : value(std::move(obj))
    {
    }

    explicit JSValue(
        std::shared_ptr<JSFunction> fn)
        : value(std::move(fn))
    {
    }

    // Convenience factories
    static JSValue makeNull()
    {
        return JSValue(JSNull{});
    }

    static JSValue makeUndefined()
    {
        return JSValue(JSUndefined{});
    }

    bool isNull() const
    {
        return std::holds_alternative<JSNull>(
            value);
    }

    bool isUndefined() const
    {
        return std::holds_alternative<JSUndefined>(
            value);
    }

    bool isNumber() const
    {
        return std::holds_alternative<double>(
            value);
    }

    bool isBool() const
    {
        return std::holds_alternative<bool>(
            value);
    }

    bool isString() const
    {
        return std::holds_alternative<std::string>(
            value);
    }

    bool isArray() const
    {
        return std::holds_alternative<
            std::vector<JSValue>>(value);
    }

    bool isObject() const
    {
        return std::holds_alternative<
            std::shared_ptr<JSObject>>(value);
    }

    bool isFunction() const
    {
        return std::holds_alternative<
            std::shared_ptr<JSFunction>>(value);
    }
};

struct JSObject
{
    // Vector of pairs preserves insertion order
    std::vector<std::pair<std::string, JSValue>>
        properties;

    JSValue* find(const std::string& key)
    {
        for (auto& [k, v] : properties)
        {
            if (k == key) return &v;
        }
        return nullptr;
    }

    const JSValue* find(
        const std::string& key) const
    {
        for (auto& [k, v] : properties)
        {
            if (k == key) return &v;
        }
        return nullptr;
    }

    void set(
        const std::string& key,
        const JSValue& value)
    {
        for (auto& [k, v] : properties)
        {
            if (k == key)
            {
                v = value;
                return;
            }
        }
        properties.push_back({key, value});
    }
};

struct JSFunction
{
    std::string name;
    std::vector<std::string> parameters;
    BlockStatement* body = nullptr;
    Expression* expressionBody = nullptr;
};