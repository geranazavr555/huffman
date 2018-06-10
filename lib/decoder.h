#ifndef HUFFMAN_DECODER_H
#define HUFFMAN_DECODER_H

#include "tree.h"

#include <vector>
#include <cstdint>

namespace huffman
{
    class Decoder
    {
    private:
        Tree tree;

    public:
        void decode_dict(std::vector<uint8_t> const &src);
        void decode_data(std::vector <uint8_t> const &src, std::vector<uint8_t> &out);
    };
}

#endif //HUFFMAN_DECODER_H
