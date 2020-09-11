#include "Trie.hpp"

Trie::Trie ()
    : root_ (0)
{
}

bool Trie::Contains (const std::string &string) const
{
    if (string.empty ())
    {
        return true;
    }

    struct TraverseFunctor
    {
        typedef bool Result;

        TraverseFunctor (const std::string &string)
        {
            searchIterator = string.begin ();
            endIterator = string.end ();
        }

        char NextSymbol ()
        {
            char next = *searchIterator;
            ++searchIterator;

            if (searchIterator == endIterator)
            {
                done = true;
            }

            return next;
        }

        void ProcessNode (const Node *node)
        {
        }

        bool IsSatisfied ()
        {
            return done;
        }

        Result EndTraverse ()
        {
            return done;
        }

        std::string::const_iterator searchIterator;
        std::string::const_iterator endIterator;
        bool done = false;
    };

    TraverseFunctor functor (string);
    return ConstTraverse (functor);
}

bool Trie::Insert (const std::string &string)
{
    if (string.empty ())
    {
        return true;
    }

    struct TraverseFunctor
    {
        typedef bool Result;

        TraverseFunctor (const std::string &string)
        {
            searchIterator = string.begin ();
            endIterator = string.end ();
        }

        char NextSymbol ()
        {
            char next = *searchIterator;
            ++searchIterator;

            if (searchIterator == endIterator)
            {
                done = true;
            }

            return next;
        }

        void ProcessNode (Node *node)
        {
            try
            {
                node->children_.at (*searchIterator);
            }
            catch (std::out_of_range &exception)
            {
                node->children_.insert (std::make_pair (
                    *searchIterator, new Node (*searchIterator)));
                changesDone = true;
            }
        }

        bool IsSatisfied ()
        {
            return done;
        }

        Result EndTraverse ()
        {
            return changesDone;
        }

        bool done = false;
        bool changesDone = false;
        std::string::const_iterator searchIterator;
        std::string::const_iterator endIterator;
    };

    TraverseFunctor functor (string);
    return Traverse (functor);
}

Trie::Node::Node (char symbol)
    : symbol_ (symbol)
{
}

Trie::Node::~Node ()
{
    for (auto &keyValuePair : children_)
    {
        delete keyValuePair.second;
    }
}
