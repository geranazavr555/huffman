//
// Created by georgiy on 02.06.18.
//

#include "bit_vector.h"

BitVector::BitVector() :
        used_bits_in_last_bucket(0),
        data()
{}

// TODO : case = 0;
BitVector::BitVector(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last) :
        used_bits_in_last_bucket(BITS_PER_BUCKET),
        data(first, last) {}

void BitVector::push_back(bool val)
{
    uint8_t numv = (val ? 1 : 0);
    if (used_bits_in_last_bucket == BITS_PER_BUCKET || used_bits_in_last_bucket == 0)
    {
        data.emplace_back(numv);
        used_bits_in_last_bucket = 1;
    }
    else
    {
        data.back() |= (1 << used_bits_in_last_bucket) * numv;
        ++used_bits_in_last_bucket;
    }
}

void BitVector::clear()
{
    used_bits_in_last_bucket = 0;
    data.clear();
}

size_t BitVector::size() const
{
    if (data.empty())
        return 0;
    if (used_bits_in_last_bucket == 0)
        return (data.size() - 1) * BITS_PER_BUCKET;
    else
        return (data.size() - 1) * BITS_PER_BUCKET + used_bits_in_last_bucket;
}

size_t BitVector::buckets_count()
{
    return data.size();
}

void BitVector::push_full_bucket_back(uint8_t values)
{
    data.push_back(values);
    used_bits_in_last_bucket = BITS_PER_BUCKET;
}

void BitVector::extend_full_bucket_back(uint8_t values)
{
    if (used_bits_in_last_bucket == 0 || used_bits_in_last_bucket == BITS_PER_BUCKET)
        data.push_back(values);
    else
    {
        size_t rest = BITS_PER_BUCKET - used_bits_in_last_bucket;
        data.back() |= (values & ((1 << rest) - 1)) << used_bits_in_last_bucket;
        data.push_back(values >> rest);
    }
}

void BitVector::front_reserve(size_t cnt)
{
    data.resize(data.size() + cnt);
    for (size_t i = data.size() - 1; i >= cnt; --i)
    {
        data[i] = data[i - cnt];
    }

    for (size_t i = 0; i < cnt; ++i)
        data[i] = 0;
}

uint8_t& BitVector::bucket_access(size_t ind)
{
    return data[ind];
}

void BitVector::extend(BitVector &v)
{
    //data.reserve(data.size() + v.data.size() + 1);
    //for (size_t i = 0; i < v.size(); i++)
    //    push_back(v[i]);
    for (size_t i = 0; i < v.data.size() - 1; i++)
    {
        extend_full_bucket_back(v.bucket_access(i));
    }
    for (size_t i = 0; i < v.used_bits_in_last_bucket; i++)
        push_back(v[(v.buckets_count() - 1) * 8 + i]);
}

void BitVector::resize(size_t sz)
{
    data.resize(sz / 8);
    used_bits_in_last_bucket = sz % 8;
}

std::vector<uint8_t>::iterator BitVector::begin()
{
    return data.begin();
}

std::vector<uint8_t>::iterator BitVector::end()
{
    return data.end();
}

bool BitVector::operator[](size_t ind) const
{
    return static_cast<bool>(data[ind / BITS_PER_BUCKET] & (1 << (ind % BITS_PER_BUCKET)));
}