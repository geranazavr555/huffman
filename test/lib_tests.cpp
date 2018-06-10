#include "gtest/gtest.h"

#include "encoder.h"
#include "decoder.h"

#include <string>

using huffman::Encoder;
using huffman::Decoder;
using huffman::Counter;

void complex_test(std::vector<uint8_t> const &src)
{
    Counter counter;
    Encoder encoder(counter);
    Decoder decoder;

    std::vector<uint8_t> encoded_dict;

    counter.feed(src);
    encoder.initialize();
    encoder.encode_dict(encoded_dict);
    decoder.decode_dict(encoded_dict);

    std::vector<uint8_t> encoded, decoded;

    encoder.encode_data(src, encoded);
    decoder.decode_data(encoded, decoded);

    ASSERT_EQ(src, decoded);
}

TEST(library, samples)
{
    complex_test({1, 2, 3, 3, 4, 5, 6, 6, 6, 7, 1, 2, 3, 1, 1, 1});
}

TEST(library, empty)
{
    complex_test({});
}

TEST(library, single_byte)
{
    complex_test({42});
}


TEST(library, single_byte_in_dict)
{
    std::vector<uint8_t> src;
    for (size_t i = 0; i < 1000; ++i)
        src.push_back(42);
    complex_test(src);
}

TEST(library, real_text)
{
    std::string s = "The C standard library or libc is the standard library for the C programming language,"
            "as specified in the ANSI C standard.[1] It was developed at the same time as the C library POSIX"
            "specification, which is a superset of it.[2][3] Since ANSI C was adopted by the International"
            "Organization for Standardization,[4] the C standard library is also called the ISO C library.\n"
            "The C standard library provides macros, type definitions and functions for tasks such as string"
            "handling, mathematical computations, input/output processing, memory management, and several other"
            "operating system services.";

    std::vector<uint8_t> src(s.begin(), s.end());
    complex_test(src);
}

TEST(library, big_data)
{
    const size_t N = 1024 * 1024 * 64;
    std::vector<uint8_t> src;
    for (size_t i = 0; i < N; ++i)
    {
        src.push_back(42);
    }

    Counter counter;
    Encoder encoder(counter);
    Decoder decoder;

    std::vector<uint8_t> encoded_dict;

    counter.feed(src);
    encoder.initialize();
    encoder.encode_dict(encoded_dict);
    decoder.decode_dict(encoded_dict);

    std::vector<uint8_t> encoded, decoded;

    encoder.encode_data(src, encoded);
    decoder.decode_data(encoded, decoded);

    ASSERT_EQ(src, decoded);
    ASSERT_LE(encoded.size() + encoded_dict.size(), src.size());
}