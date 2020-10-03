#pragma once
#include <string>
#include <memory>
#include <vector>
#include <variant>

namespace JSON
{
struct Value;
struct Array;
struct Object;

using Any = std::variant <Value, Array, Object>;

template <typename T, typename... Args> std::unique_ptr <Any> ConstructAny (Args... args)
{
    return std::make_unique <Any> (T {std::forward <Args> (args)...});
}

enum class Types
{
    VALUE = 0,
    ARRAY,
    OBJECT
};

struct Value
{
    bool operator== (const Value &rhs) const;
    bool operator!= (const Value &rhs) const;

    std::string value_;
};

struct Attribute
{
    Attribute (std::string name, std::unique_ptr <Any> &&value);
    Attribute (const Attribute &other);
    Attribute (Attribute &&other);

    bool operator== (const Attribute &rhs) const;
    bool operator!= (const Attribute &rhs) const;

    std::string name_;
    std::unique_ptr <Any> value_;
};

struct Array
{
    Array ();
    Array (std::initializer_list <Any *> values);
    Array (const Array &other);
    Array (Array &&other);

    bool operator== (const Array &rhs) const;
    bool operator!= (const Array &rhs) const;

    std::vector <std::unique_ptr <Any>> values_;
};

struct Object
{
    Object();
    Object(std::initializer_list<Attribute> attributes);
    Object(const Object &object);
    Object(Object &&object);

    bool operator== (const Object &rhs) const;
    bool operator!= (const Object &rhs) const;

    Attribute &GetByName (const std::string &name);
    const Attribute &GetByName (const std::string &name) const;

    std::vector <Attribute> attributes_;
};
}
