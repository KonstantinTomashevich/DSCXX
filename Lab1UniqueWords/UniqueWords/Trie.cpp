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
            if (searchIterator == endIterator)
            {
                return '\0';
            }
            else
            {
                char next = *searchIterator;
                ++searchIterator;
                return next;
            }
        }

        void ProcessNode (const Node *node)
        {
            if (node->symbol_ == '\0' && searchIterator == endIterator)
            {
                done = true;
            }
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
            char next = CurrentSymbol ();
            if (next != '\0')
            {
                ++searchIterator;
            }

            return next;
        }

        char CurrentSymbol ()
        {
            if (searchIterator == endIterator)
            {
                return '\0';
            }
            else
            {
                return *searchIterator;
            }
        }

        void ProcessNode (Node *node)
        {
            try
            {
                node->children_.at (CurrentSymbol ());
            }
            catch (std::out_of_range &exception)
            {
                node->children_.insert (std::make_pair (
                    *searchIterator, new Node (CurrentSymbol ())));
                changesDone = true;
            }

            if (node->symbol_ == '\0' && searchIterator == endIterator)
            {
                done = true;
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
