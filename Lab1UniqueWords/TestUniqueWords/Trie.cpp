#include <boost/test/unit_test.hpp>
#include <UniqueWords/Trie.hpp>

BOOST_AUTO_TEST_SUITE (TestTrie)

BOOST_AUTO_TEST_CASE (AlwaysInsertEmpty)
{
    BOOST_REQUIRE (Trie ().Insert (""));
}

BOOST_AUTO_TEST_CASE (SingleInsert)
{
    Trie trie;
    BOOST_REQUIRE (Trie ().Insert ("Hello, world!"));
}

BOOST_AUTO_TEST_CASE (DontDuplicateInsert)
{
    Trie trie;
    BOOST_REQUIRE (trie.Insert ("Hello, world!"));
    BOOST_REQUIRE (!trie.Insert ("Hello, world!"));
}

BOOST_AUTO_TEST_CASE (AlwaysContainsEmpty)
{
    BOOST_REQUIRE (Trie ().Contains (""));
}

BOOST_AUTO_TEST_CASE (FreshContainsNothing)
{
    BOOST_REQUIRE (!Trie ().Contains ("Hello, world!"));
}

BOOST_AUTO_TEST_CASE (ContainsAfterInsert)
{
    Trie trie;
    BOOST_REQUIRE (trie.Insert ("Hello, world!"));
    BOOST_REQUIRE (trie.Contains ("Hello, world!"));
}

BOOST_AUTO_TEST_SUITE_END ()
