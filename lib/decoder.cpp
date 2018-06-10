//
// Created by georgiy on 26.05.18.
//

#include "decoder.h"
#include "tree.h"

using std::vector;

namespace huffman
{

    /// structure_len = 2 * edges_count
    /// leaves_count = edges_count / 2 + 1;
    /// => leaves_count = structure_len / 4 + 1

    void Decoder::decode_dict(std::vector<uint8_t> const &src)
    {
        size_t structure_len = src[0] + (src[1] << 8);
        size_t symbols_count = (structure_len > 0 ? structure_len / 4 + 1 : 0);

        BitVector structure_bits(src.begin() + 2, src.begin() + 2 + structure_len / 8 + (structure_len % 8 ? 1 : 0));


        vector <uint8_t> symbols(symbols_count);
        std::move(src.begin() + 2 + structure_len / 8 + (structure_len % 8 ? 1 : 0),
                  src.begin() + 2 + structure_len / 8 + (structure_len % 8 ? 1 : 0) + symbols_count,
                  symbols.begin());


        //read structure

        tree.deserialize(structure_bits, structure_len, symbols);
    }


    void Decoder::decode_data(std::vector <uint8_t> const &src, std::vector<uint8_t> &out)
    {
        size_t data_len = 0;
        for (size_t i = 0; i < 8; i++)
        {
            data_len +=
                    static_cast<size_t>(src[i]) << (8 * i);
        }

        BitVector data_bits(src.begin() + 8, src.end());
        //data_bits.resize(data_len);

        out.clear();
        if (!tree.empty())
        {
            if (tree.root()->is_leaf())
            {
                for (size_t i = 0; i < data_len; ++i)
                    out.push_back(tree.root()->get_symbol());
            }
            else
            {
                TreeNode *cur_node = tree.root();
                for (size_t i = 0; i <= data_len; ++i)
                {
                    if (cur_node->is_leaf())
                    {
                        out.push_back(cur_node->get_symbol());
                        cur_node = tree.root();
                    }
                    if (i < data_len)
                        cur_node->go(data_bits[i], cur_node);
                }
            }
        }
    }
}