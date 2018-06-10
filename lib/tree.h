//
// Created by georgiy on 26.05.18.
//

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "bit_vector.h"

#include <unordered_set>
#include <vector>
#include <functional>

namespace huffman
{
    class TreeNode
    {
    private:
        size_t sum;
        TreeNode* children[2];
        std::unordered_set<uint8_t> symbols;

    public:
        explicit TreeNode(std::pair <uint8_t, size_t> symbol_cnt_pair);
        explicit TreeNode(uint8_t symbol);
        explicit TreeNode(uint8_t symbol, size_t cnt);
        explicit TreeNode(TreeNode* left_child, TreeNode* right_child);

        bool is_leaf() const;
        bool go(uint8_t symbol, TreeNode*& next);
        void go(bool val, TreeNode*& next);
        uint8_t get_symbol();
        bool contains(uint8_t val);

        void serialize_subtree(BitVector &structure, std::vector<uint8_t> &ordered_symbols);

        struct comp_by_sum : public std::binary_function<TreeNode, TreeNode, bool>
        {
            bool operator()(const TreeNode& x, const TreeNode& y) const
            {
                return x.sum < y.sum;
            }
        };
    };

    class Tree
    {
    private:
        std::vector <TreeNode> vertices;

        size_t deserialize_inner(BitVector const &structure, std::vector<uint8_t>::iterator &cur_symbol, size_t struct_ind);

    public:
        TreeNode* root();
        size_t size() const;
        bool empty() const;

        void serialize(BitVector &structure, std::vector<uint8_t> &ordered_symbols);

        void deserialize(BitVector const &structure, size_t structure_len, std::vector<uint8_t> &symbols);

        friend void build(std::vector <std::pair <uint8_t, size_t> > const &src, Tree &tree);
    };
}

#endif //HUFFMAN_TREE_H
