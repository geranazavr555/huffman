//
// Created by georgiy on 26.05.18.
//

#include "tree.h"
#include <algorithm>
#include <set>

using std::multiset;
using std::vector;
using std::pair;


namespace huffman
{
    TreeNode::TreeNode(uint8_t symbol, size_t cnt) :
            sum(cnt),
            children{nullptr, nullptr},
            symbols{symbol} {}

    TreeNode::TreeNode(uint8_t symbol) : TreeNode(symbol, 0) {}

    TreeNode::TreeNode(std::pair<uint8_t, size_t> symbol_cnt_pair) :
            TreeNode(symbol_cnt_pair.first, symbol_cnt_pair.second) {}

    TreeNode::TreeNode(TreeNode *left_child, TreeNode *right_child):
            sum(left_child->sum + right_child->sum),
            children{left_child, right_child},
            symbols(left_child->symbols)
    {
        for (const auto symbol : right_child->symbols)
            symbols.insert(symbol);
    }

    bool TreeNode::is_leaf() const
    {
        return (!children[0] && !children[1]);
    }

    bool TreeNode::go(uint8_t symbol, TreeNode *&next)
    {
        if (children[0]->symbols.find(symbol) != children[0]->symbols.end())
        {
            next = children[0];
            return false;
        }
        else
        {
            next = children[1];
            return true;
        }
    }

    void TreeNode::go(bool val, TreeNode *&next)
    {
        next = children[(val ? 1 : 0)];
    }

    uint8_t TreeNode::get_symbol()
    {
        return *(symbols.begin());
    }

    bool TreeNode::contains(uint8_t val)
    {
        return symbols.find(val) != symbols.end();
    }

    void TreeNode::serialize_subtree(BitVector &structure, std::vector<uint8_t> &ordered_symbols)
    {
        if (is_leaf())
        {
            ordered_symbols.push_back(*(symbols.begin()));
            return;
        }

        structure.push_back(false);
        children[0]->serialize_subtree(structure, ordered_symbols);
        structure.push_back(true);
        structure.push_back(false);
        children[1]->serialize_subtree(structure, ordered_symbols);
        structure.push_back(true);
    }



    TreeNode* Tree::root()
    {
        return &(vertices.back());
    }

    size_t Tree::size() const
    {
        return vertices.size();
    }

    bool Tree::empty() const
    {
        return vertices.empty();
    }

    void Tree::serialize(BitVector &structure, std::vector<uint8_t> &ordered_symbols)
    {
        if (!vertices.empty())
            root()->serialize_subtree(structure, ordered_symbols);
        // nothing to serialize
    }

    size_t Tree::deserialize_inner(BitVector const &structure, std::vector<uint8_t>::iterator &cur_symbol,
                                   size_t struct_ind)
    {
        if (structure[struct_ind])
        {
            // leaf
            vertices.emplace_back(*cur_symbol);
            ++cur_symbol;
            return struct_ind + 1;
        }
        else
        {
            struct_ind = deserialize_inner(structure, cur_symbol, struct_ind + 1);
            TreeNode* first = &(vertices.back());
            struct_ind = deserialize_inner(structure, cur_symbol, struct_ind + 1);
            TreeNode* second = &(vertices.back());
            vertices.emplace_back(first, second);
            return struct_ind + 1;
        }
    }

    void Tree::deserialize(BitVector const &structure, size_t structure_len, std::vector<uint8_t> &symbols)
    {
        if (structure_len == 0) // nothing to deserialize
            return;

        vertices.clear();
        vertices.reserve(structure_len / 2 + 1);

        auto symbols_iterator = symbols.begin();
        deserialize_inner(structure, symbols_iterator, 0);
    }

    void build(vector <pair <uint8_t, size_t> > const &src, Tree &tree)
    {
        if (src.empty())
        {
            // nothing to build
            return;
        }

        multiset<TreeNode, TreeNode::comp_by_sum> q;

        tree.vertices.reserve(src.size() * 2 - 1);

        for (auto const &item : src)
            q.emplace(item);

        while (q.size() > 1)
        {
            auto first = *(q.begin());
            auto second = *(++q.begin());

            q.erase(q.begin());
            q.erase(q.begin());

            tree.vertices.push_back(first);
            tree.vertices.push_back(second);

            q.emplace(&(tree.vertices.back()) - 1, &(tree.vertices.back()));
        }

        tree.vertices.push_back(*(q.begin()));
    }
}
