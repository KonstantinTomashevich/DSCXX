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

struct Value
{
    bool operator== (const Value &rhs) const;
    bool operator!= (const Value &rhs) const;

    std::string value_;
};

struct Array
{
    Array ();
    Array (std::vector <Any> values);
    Array (const Array &other);
    Array (Array &&other);

    bool operator== (const Array &rhs) const;
    bool operator!= (const Array &rhs) const;

    std::vector <Any> values_;
};

struct Attribute;

struct Object
{
    Object ();
    Object (std::initializer_list <Attribute> attributes);
    Object (const Object &object);
    Object (Object &&object);

    bool operator== (const Object &rhs) const;
    bool operator!= (const Object &rhs) const;

    Attribute &GetByName (const std::string &name);
    const Attribute &GetByName (const std::string &name) const;

    std::vector <Attribute> attributes_;
};

struct Attribute
{
    Attribute (std::string name, Any value);
    Attribute (const Attribute &other);
    Attribute (Attribute &&other);

    bool operator== (const Attribute &rhs) const;
    bool operator!= (const Attribute &rhs) const;

    std::string name_;
    Any value_;
};
}
