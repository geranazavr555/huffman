//
// Created by georgiy on 02.06.18.
//

#ifndef HUFFMAN_BIT_VECTOR_H
#define HUFFMAN_BIT_VECTOR_H

#include <cstdint>
#include <vector>
#include <cstddef>

class BitVector
{
private:
    static const size_t BITS_PER_BUCKET = 8;

    size_t used_bits_in_last_bucket;
    std::vector <uint8_t> data;

public:
    BitVector();
    BitVector(std::vector <uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last);

    size_t size() const;
    size_t buckets_count();

    void clear();
    void push_back(bool val);
    void push_full_bucket_back(uint8_t values);
    void front_reserve(size_t cnt);
    uint8_t& bucket_access(size_t ind);
    void extend_full_bucket_back(uint8_t values);
    void extend(BitVector &v);
    void resize(size_t sz);

    std::vector<uint8_t>::iterator begin();
    std::vector<uint8_t>::iterator end();

    bool operator[](size_t ind) const;
};

#endif //HUFFMAN_BIT_VECTOR_H
