#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

#include "bit_vector.h"
#include "tree.h"
#include "counter.h"

#include <vector>
#include <cstdint>

namespace huffman
{
    class Encoder
    {
    private:
        Counter& counter;
        Tree tree;
        BitVector dict[256];

        void build_tree();
        void cache_dict();
    public:
        Encoder(Counter &cnt);
        void initialize();

        void encode_dict(std::vector<uint8_t> &out);
        void encode_data(std::vector<uint8_t> const &src, std::vector<uint8_t> &out);
        //void encode(std::vector <uint8_t> const &src, std::vector <uint8_t> &out);
    };
}

#endif //HUFFMAN_ENCODER_H
