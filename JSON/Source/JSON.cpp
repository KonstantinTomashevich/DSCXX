#include "JSON.hpp"
#include "OverloadedTrait.hpp"

#include <algorithm>
#include <functional>
#include <utility>

namespace JSON
{
static std::unique_ptr <Any> CopyAnyPointer (const std::unique_ptr <Any> &other)
{
    if (other == nullptr)
    {
        return nullptr;
    }
    else
    {
        return std::visit (overloaded {
            [] (const Value &value) -> std::unique_ptr <Any>
            {
                return std::make_unique <Any> (Value {value});
            },

            [] (const Array &array) -> std::unique_ptr <Any>
            {
                return std::make_unique <Any> (Array {array});
            },

            [] (const Object &object) -> std::unique_ptr <Any>
            {
                return std::make_unique <Any> (Object {object});
            },
        }, *other);
    }
}

bool Value::operator== (const Value &rhs) const
{
    return value_ == rhs.value_;
}

bool Value::operator!= (const Value &rhs) const
{
    return !(rhs == *this);
}

Attribute::Attribute (std::string name, std::unique_ptr <Any> &&value)
    : name_ (std::move (name)),
      value_ (std::move (value))
{
}

Attribute::Attribute (const Attribute &other)
    : name_ (other.name_),
      value_ (CopyAnyPointer (other.value_))
{
}

Attribute::Attribute (Attribute &&other)
    : name_ (std::move (other.name_)),
      value_ (std::move (other.value_))
{
}

bool Attribute::operator== (const Attribute &rhs) const
{
    if (name_ != rhs.name_ || (value_ == nullptr && rhs.value_ != nullptr) ||
        (value_ != nullptr && rhs.value_ == nullptr))
    {
        return false;
    }
    else if (value_ == nullptr && rhs.value_ == nullptr)
    {
        return true;
    }
    else
    {
        return *value_ == *rhs.value_;
    }
}

bool Attribute::operator!= (const Attribute &rhs) const
{
    return !(rhs == *this);
}

Array::Array ()
    : values_ ()
{
}

Array::Array (std::initializer_list <Any *> values)
    : values_ ()
{
    values_.reserve (values.size ());
    for (const auto &value : values)
    {
        values_.emplace_back (value);
    }
}

Array::Array (const Array &other)
    : values_ ()
{
    values_.reserve (other.values_.size ());
    for (const auto &otherValue : other.values_)
    {
        values_.emplace_back (CopyAnyPointer (otherValue));
    }
}

Array::Array (Array &&other)
    : values_ (std::move (other.values_))
{
}

bool Array::operator== (const Array &rhs) const
{
    if (values_.size () != rhs.values_.size ())
    {
        return false;
    }

    auto firstIterator = values_.begin ();
    auto secondIterator = rhs.values_.begin ();

    while (firstIterator != values_.end ())
    {
        if (**firstIterator != **secondIterator)
        {
            return false;
        }

        ++firstIterator;
        ++secondIterator;
    }

    return true;
}

bool Array::operator!= (const Array &rhs) const
{
    return !(rhs == *this);
}

Object::Object ()
    : attributes_ ()
{
}

Object::Object (std::initializer_list <Attribute> attributes)
    : attributes_ ()
{
    attributes_.reserve (attributes.size ());
    for (const auto &attribute : attributes)
    {
        attributes_.emplace_back (std::move (attribute));
    }
}

Object::Object (const Object &object)
    : attributes_ (object.attributes_)
{
}

Object::Object (Object &&object)
    : attributes_ (std::move (object.attributes_))
{
}

bool Object::operator== (const Object &rhs) const
{
    if (attributes_.size () != rhs.attributes_.size ())
    {
        return false;
    }

    auto firstIterator = attributes_.begin ();
    auto secondIterator = rhs.attributes_.begin ();

    while (firstIterator != attributes_.end ())
    {
        if (*firstIterator != *secondIterator)
        {
            return false;
        }

        ++firstIterator;
        ++secondIterator;
    }

    return true;
}

bool Object::operator!= (const Object &rhs) const
{
    return !(rhs == *this);
}

static std::function <bool (const Attribute &)> AttributeNameEqualsPredicate (const std::string &targetName)
{
    return [&targetName] (const Attribute &other) -> bool
    {
        return other.name_ == targetName;
    };
}

Attribute &Object::GetByName (const std::string &name)
{
    auto iterator = std::find_if (attributes_.begin (), attributes_.end (),
                                  AttributeNameEqualsPredicate (name));

    if (iterator == attributes_.end ())
    {
        throw std::out_of_range ("Object has no attribute with name \"" + name + "\"!");
    }
    else
    {
        return *iterator;
    }
}

const Attribute &Object::GetByName (const std::string &name) const
{
    auto iterator = std::find_if (attributes_.begin (), attributes_.end (),
                                  AttributeNameEqualsPredicate (name));

    if (iterator == attributes_.end ())
    {
        throw std::out_of_range ("Object has no attribute with name \"" + name + "\"!");
    }
    else
    {
        return *iterator;
    }
}
}
