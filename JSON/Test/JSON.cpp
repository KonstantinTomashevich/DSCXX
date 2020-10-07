#include <boost/test/unit_test.hpp>
#include <JSON.hpp>

BOOST_AUTO_TEST_SUITE (TestJSON)

BOOST_AUTO_TEST_CASE (ValueFromInitializerList)
{
    std::string testString = "Hello, world!";
    JSON::Value value = {testString};
    BOOST_REQUIRE (testString == value.value_);
}

BOOST_AUTO_TEST_CASE (ValueCopy)
{
    JSON::Value firstValue = {"Hello, world!"};
    JSON::Value secondValue = firstValue;
    BOOST_REQUIRE (firstValue == secondValue);
}

BOOST_AUTO_TEST_CASE (ValueMove)
{
    std::string testString = "Hello, world!";
    JSON::Value firstValue = {testString};
    JSON::Value secondValue = std::move (firstValue);

    BOOST_REQUIRE (firstValue.value_.empty ());
    BOOST_REQUIRE (secondValue.value_ == testString);
}

static void VerifyTestAttribute (const std::string &name, const std::string &value, const JSON::Attribute &attribute)
{
    BOOST_REQUIRE (attribute.name_ == name);
    BOOST_REQUIRE (std::holds_alternative <JSON::Value> (attribute.value_));
    BOOST_REQUIRE (std::get <JSON::Value> (attribute.value_).value_ == value);
}

BOOST_AUTO_TEST_CASE (AttributeFromInitializerList)
{
    std::string name = "test_name";
    std::string stringValue = "test_value";

    JSON::Attribute attribute (name, JSON::Value {stringValue});
    VerifyTestAttribute (name, stringValue, attribute);
}

BOOST_AUTO_TEST_CASE (AttributeCopy)
{
    JSON::Attribute firstAttribute ("test_name", JSON::Value {"test_value"});
    JSON::Attribute secondAttribute = firstAttribute;

    BOOST_REQUIRE (firstAttribute.name_ == secondAttribute.name_);
    BOOST_REQUIRE (firstAttribute.value_ == secondAttribute.value_);
    BOOST_REQUIRE (firstAttribute == secondAttribute);
}

BOOST_AUTO_TEST_CASE (AttributeMove)
{
    std::string name = "test_name";
    std::string stringValue = "test_value";

    JSON::Attribute firstAttribute (name, JSON::Value {stringValue});
    JSON::Attribute secondAttribute (std::move (firstAttribute));

    BOOST_REQUIRE (firstAttribute.name_.empty ());
    BOOST_REQUIRE (std::get <JSON::Value> (firstAttribute.value_).value_.empty ());
    VerifyTestAttribute (name, stringValue, secondAttribute);
}

BOOST_AUTO_TEST_CASE (EmptyArray)
{
    JSON::Array array {};
    BOOST_REQUIRE(array.values_.empty ());
}

static void VerifyTestArray (const std::array <std::string, 3> &strings, const JSON::Array &array)
{
    for (std::size_t index = 0; index < strings.size (); ++index)
    {
        BOOST_REQUIRE (std::holds_alternative <JSON::Value> (array.values_[index]));
        BOOST_REQUIRE (std::get <JSON::Value> (array.values_[index]).value_ == strings[index]);
    }
}

#define INITIALIZE_TEST_ARRAY(strings) \
{{ \
    JSON::Value {strings[0]}, \
    JSON::Value {strings[1]},\
    JSON::Value {strings[2]}\
}}

BOOST_AUTO_TEST_CASE (ArrayFromInitializerList)
{
    std::array <std::string, 3> strings {"first", "second", "third"};
    JSON::Array array INITIALIZE_TEST_ARRAY (strings);

    BOOST_REQUIRE (array.values_.size () == 3);
    VerifyTestArray (strings, array);
}

BOOST_AUTO_TEST_CASE (ArrayCopy)
{
    std::array <std::string, 3> strings {"first", "second", "third"};
    JSON::Array firstArray INITIALIZE_TEST_ARRAY (strings);

    JSON::Array secondArray = firstArray;
    for (std::size_t index = 0; index < strings.size (); ++index)
    {
        BOOST_REQUIRE (firstArray.values_[index] == secondArray.values_[index]);
    }

    BOOST_REQUIRE (firstArray == secondArray);
}

BOOST_AUTO_TEST_CASE (ArrayMove)
{
    std::array <std::string, 3> strings {"first", "second", "third"};
    JSON::Array firstArray INITIALIZE_TEST_ARRAY (strings);

    JSON::Array secondArray (std::move (firstArray));
    BOOST_REQUIRE (firstArray.values_.empty ());
    VerifyTestArray (strings, secondArray);
}

BOOST_AUTO_TEST_CASE (EmptyObject)
{
    JSON::Object object {};
    BOOST_REQUIRE (object.attributes_.empty ());
}

static void VerifyTestObject (const std::string &firstAttributeName, const std::string &firstAttributeValue,
                              const std::string &secondAttributeName,
                              const std::array <std::string, 3> &secondAttributeValue,
                              const JSON::Object &object)
{
    BOOST_REQUIRE (object.attributes_.size () == 2);
    VerifyTestAttribute (firstAttributeName, firstAttributeValue, object.attributes_[0]);

    BOOST_REQUIRE (object.attributes_[1].name_ == secondAttributeName);
    BOOST_REQUIRE (std::holds_alternative <JSON::Array> (object.attributes_[1].value_));
    VerifyTestArray (secondAttributeValue, std::get <JSON::Array> (object.attributes_[1].value_));
}

#define INITIALIZE_TEST_OBJECT(firstAttributeName, firstAttributeValue, secondAttributeName, secondAttributeValue) \
{ \
    {firstAttributeName, JSON::Value {firstAttributeValue}}, \
    { \
        secondAttributeName, \
        JSON::Array INITIALIZE_TEST_ARRAY (secondAttributeValue) \
    } \
}

#define TEST_OBJECT_STANDART_VALUES \
std::string firstAttributeName = "simple_value"; \
std::string firstAttributeValue = "test_value"; \
 \
std::string secondAttributeName = "array"; \
std::array <std::string, 3> secondAttributeValue {"first", "second", "third"}

BOOST_AUTO_TEST_CASE (ObjectFromInitializerList)
{
    TEST_OBJECT_STANDART_VALUES;
    JSON::Object object INITIALIZE_TEST_OBJECT (firstAttributeName, firstAttributeValue,
                                                secondAttributeName, secondAttributeValue);
    VerifyTestObject (firstAttributeName, firstAttributeValue, secondAttributeName, secondAttributeValue, object);
}

BOOST_AUTO_TEST_CASE (ObjectCopy)
{
    TEST_OBJECT_STANDART_VALUES;
    JSON::Object firstObject INITIALIZE_TEST_OBJECT (firstAttributeName, firstAttributeValue,
                                                     secondAttributeName, secondAttributeValue);

    JSON::Object secondObject = firstObject;
    BOOST_REQUIRE (firstObject.attributes_ == secondObject.attributes_);
    BOOST_REQUIRE (firstObject == secondObject);
}

BOOST_AUTO_TEST_CASE (ObjectMove)
{
    TEST_OBJECT_STANDART_VALUES;
    JSON::Object firstObject INITIALIZE_TEST_OBJECT (firstAttributeName, firstAttributeValue,
                                                     secondAttributeName, secondAttributeValue);

    JSON::Object secondObject (std::move (firstObject));
    BOOST_REQUIRE (firstObject.attributes_.empty ());
    VerifyTestObject (firstAttributeName, firstAttributeValue,
                      secondAttributeName, secondAttributeValue, secondObject);
}

BOOST_AUTO_TEST_CASE (ObjectGetByName)
{
    TEST_OBJECT_STANDART_VALUES;
    JSON::Object object INITIALIZE_TEST_OBJECT (firstAttributeName, firstAttributeValue,
                                                secondAttributeName, secondAttributeValue);

    BOOST_REQUIRE (&object.GetByName (secondAttributeName) == &object.attributes_[1]);
    BOOST_REQUIRE (&object.GetByName (firstAttributeName) == &object.attributes_[0]);

    const JSON::Object &constObject = object;
    BOOST_REQUIRE (&constObject.GetByName (secondAttributeName) == &constObject.attributes_[1]);
    BOOST_REQUIRE (&constObject.GetByName (firstAttributeName) == &constObject.attributes_[0]);
}

BOOST_AUTO_TEST_SUITE_END ()
