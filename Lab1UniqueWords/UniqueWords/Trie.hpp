#pragma once
#include <unordered_map>

class Trie final
{
public:
    Trie ();
    ~Trie () = default;

    bool Contains (const std::string &string) const;
    bool Insert (const std::string &string);

private:
    struct Node
    {
        char symbol_;
        std::unordered_map <char, Node *> children_;

        explicit Node (char symbol);
        virtual ~Node ();
    };

    template <typename Ruler> typename Ruler::Result Traverse (Ruler &ruler);
    template <typename Ruler> typename Ruler::Result ConstTraverse (Ruler &ruler) const;

    Node root_;
};

template <typename Ruler>
typename Ruler::Result Trie::Traverse (Ruler &ruler)
{
#define TRIE_TRAVERSE_CODE(CONST_MARKER) \
    CONST_MARKER Node *current = &root_; \
     \
    do \
    { \
        ruler.ProcessNode (current); \
        try \
        { \
            current = current->children_.at(ruler.NextSymbol ()); \
        } \
        catch (std::out_of_range &exception) \
        { \
            current = nullptr; \
        } \
    } \
    while (current != nullptr && !ruler.IsSatisfied ()); \
    \
    return ruler.EndTraverse ();
    TRIE_TRAVERSE_CODE()
}

template <typename Ruler>
typename Ruler::Result Trie::ConstTraverse (Ruler &ruler) const
{
    TRIE_TRAVERSE_CODE (const)
}
