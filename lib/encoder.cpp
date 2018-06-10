#include "encoder.h"
#include "counter.h"

using std::vector;
using std::pair;

namespace huffman
{
    Encoder::Encoder(Counter &cnt) : counter(cnt) {}

    void Encoder::build_tree()
    {
        vector < pair <uint8_t, size_t> > tmp;
        counter.get_current_values(tmp);
        build(tmp, tree);
    }

    void Encoder::cache_dict()
    {
        if (tree.empty())
            return;
        for (uint8_t item = 0; item < 256; item++)
        {
            TreeNode *cur_node = tree.root();
            if (cur_node->contains(item))
            {
                while (!cur_node->is_leaf())
                {
                    bool tmp = cur_node->go(item, cur_node);
                    dict[item].push_back(tmp);
                }
            }
            if (item == 255)
                break;
        }
    }

    void Encoder::initialize()
    {
        build_tree();
        cache_dict();
    }

    void Encoder::encode_dict(std::vector<uint8_t> &out)
    {
        size_t structure_len;
        if (tree.empty())
            structure_len = 0;
        else
            structure_len = (tree.size() == 1 ? 1 : (tree.size() - 1) * 2);

        BitVector encoded_dict;
        /// Little endian
        /// MAX -- 16k different symbols in tree
        encoded_dict.push_full_bucket_back(static_cast<uint8_t>(structure_len % 256));
        encoded_dict.push_full_bucket_back(static_cast<uint8_t>(structure_len / 256));

        vector <uint8_t> symbols;
        tree.serialize(encoded_dict, symbols);

        if (symbols.size() == 1)
            encoded_dict.push_full_bucket_back(1);

        for (auto symbol : symbols)
            encoded_dict.push_full_bucket_back(symbol);

        out.resize(encoded_dict.buckets_count());
        std::move(encoded_dict.begin(), encoded_dict.end(), out.begin());
    }

    /// format: [2 bytes -- len of tree][tree].[dict]
    /// format: [8 bytes -- len of data][data].

    void Encoder::encode_data(std::vector<uint8_t> const &src, std::vector<uint8_t> &out)
    {
        BitVector encoded_data;

        if (!tree.empty() && tree.root()->is_leaf())
        {
            for (auto item : src)
            {
                encoded_data.push_back(false);
            }
        }
        else
        {
            for (auto item : src)
            {
                encoded_data.extend(dict[item]);
            }
        }

        size_t data_len = encoded_data.size();

        encoded_data.front_reserve(8);
        for (size_t i = 0; i < 8; i++)
        {
            encoded_data.bucket_access(i) = static_cast<uint8_t>(data_len >> (8 * i));
        }

        out.resize(encoded_data.buckets_count() + 8);
        std::move(encoded_data.begin(), encoded_data.end(), out.begin());
    }
}