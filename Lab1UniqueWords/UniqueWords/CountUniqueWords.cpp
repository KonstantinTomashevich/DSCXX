#include "CountUniqueWords.hpp"
#include <UniqueWords/Trie.hpp>
#include <sstream>

inline void CountWord (Trie &uniqueWords, const std::string &word, std::size_t &uniqueWordsCount)
{
    if (!word.empty () && uniqueWords.Insert (word))
    {
        ++uniqueWordsCount;
    }
}

std::size_t CountUniqueWords (std::istream &input)
{
    Trie uniqueWords;
    std::size_t uniqueWordsCount = 0;
    std::string accumulatedWord;

    while (input)
    {
        char next = input.get ();
        if (isalpha (next))
        {
            accumulatedWord += next;
        }
        else
        {
            CountWord (uniqueWords, accumulatedWord, uniqueWordsCount);
            accumulatedWord.clear ();
        }
    }

    CountWord (uniqueWords, accumulatedWord, uniqueWordsCount);
    return uniqueWordsCount;
}

std::size_t CountUniqueWords (const std::string &input)
{
    std::istringstream inputStream (input);
    return CountUniqueWords (inputStream);
}
