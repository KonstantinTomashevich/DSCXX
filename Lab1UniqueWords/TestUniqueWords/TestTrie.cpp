#include <boost/test/unit_test.hpp>
#include <boost/format.hpp>
#include <UniqueWords/Trie.hpp>

BOOST_AUTO_TEST_SUITE (MixedTransaction)

BOOST_AUTO_TEST_CASE(TrieAlwaysInsertEmpty)
{
    BOOST_REQUIRE (Trie ().Insert (""));
}

BOOST_AUTO_TEST_CASE(TrieSingleInsert)
{
    Trie trie;
    BOOST_REQUIRE (Trie ().Insert ("Hello, world!"));
}

BOOST_AUTO_TEST_CASE(TrieDontDuplicateInsert)
{
    Trie trie;
    BOOST_REQUIRE (trie.Insert ("Hello, world!"));
    BOOST_REQUIRE (!trie.Insert ("Hello, world!"));
}

BOOST_AUTO_TEST_CASE(TrieAlwaysContainsEmpty)
{
    BOOST_REQUIRE (Trie ().Contains (""));
}

BOOST_AUTO_TEST_CASE(TrieFreshContainsNothing)
{
    BOOST_REQUIRE (!Trie ().Contains ("Hello, world!"));
}

BOOST_AUTO_TEST_CASE(TrieContainsAfterInsert)
{
    Trie trie;
    BOOST_REQUIRE (trie.Insert ("Hello, world!"));
    BOOST_REQUIRE (trie.Contains ("Hello, world!"));
}

BOOST_AUTO_TEST_SUITE_END ()
