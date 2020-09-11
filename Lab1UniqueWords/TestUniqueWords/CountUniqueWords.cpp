#include <boost/test/unit_test.hpp>
#include <UniqueWords/CountUniqueWords.hpp>
#include <fstream>

BOOST_AUTO_TEST_SUITE (TestCountUniqueWords)

BOOST_AUTO_TEST_CASE (ZeroInEmptyStream)
{
    std::istringstream input ("");
    BOOST_REQUIRE_EQUAL (CountUniqueWords (input), 0);
}

BOOST_AUTO_TEST_CASE (OneInOneWordString)
{
    std::istringstream input ("oneword");
    BOOST_REQUIRE_EQUAL (CountUniqueWords (input), 1);
}

BOOST_AUTO_TEST_CASE (OneInDuplicateString)
{
    std::istringstream input ("one one one");
    BOOST_REQUIRE_EQUAL (CountUniqueWords (input), 1);
}

BOOST_AUTO_TEST_CASE (ThreeInUniqueWordString)
{
    std::istringstream input ("one two three");
    BOOST_REQUIRE_EQUAL (CountUniqueWords (input), 3);
}

BOOST_AUTO_TEST_CASE (TwoInDifferentDuplicateString)
{
    std::istringstream input ("one one two two");
    BOOST_REQUIRE_EQUAL (CountUniqueWords (input), 2);
}

BOOST_AUTO_TEST_CASE (PrecedingSymbols)
{
    std::istringstream input ("   ### ??? !!  one");
    BOOST_REQUIRE_EQUAL (CountUniqueWords (input), 1);
}

BOOST_AUTO_TEST_CASE (TrailingSymbols)
{
    std::istringstream input ("one  ###!! ??  ");
    BOOST_REQUIRE_EQUAL (CountUniqueWords (input), 1);
}

BOOST_AUTO_TEST_CASE (MiddleSymbols)
{
    std::istringstream input ("one###!! ??  one");
    BOOST_REQUIRE_EQUAL (CountUniqueWords (input), 1);
}

BOOST_AUTO_TEST_CASE (MixedOrder)
{
    std::istringstream input ("one?two three.four two(three)one-four$two");
    BOOST_REQUIRE_EQUAL (CountUniqueWords (input), 4);
}

BOOST_AUTO_TEST_CASE (CheckStringHelper)
{
    BOOST_REQUIRE_EQUAL (CountUniqueWords ("hello? world! xyz world hello * C++ C C# Java?"), 5);
}

BOOST_AUTO_TEST_CASE (CheckFileStream)
{
#define TEMP_FILE_NAME "TestTempFile.txt"
    struct TempFileContext
    {
        std::ifstream tempInputStream;

        TempFileContext ()
        {
            std::ofstream tempOutputStream (TEMP_FILE_NAME);
            tempOutputStream << "hello? world! xyz world hello * C++ C C# Java?" << std::endl;
            tempOutputStream.close ();

            tempInputStream.open (TEMP_FILE_NAME);
        }

        ~TempFileContext ()
        {
            tempInputStream.close ();
            std::remove (TEMP_FILE_NAME);
        }
    } tempFileContext;

    BOOST_REQUIRE_EQUAL (CountUniqueWords (tempFileContext.tempInputStream), 5);
}

BOOST_AUTO_TEST_SUITE_END ()

