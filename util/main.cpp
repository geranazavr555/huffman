#include <iostream>
#include <fstream>
#include <counter.h>

#include "encoder.h"
#include "decoder.h"

using std::cout;
using std::string;
using std::ifstream;
using std::ofstream;
using std::vector;

using huffman::Encoder;
using huffman::Decoder;
using huffman::Counter;

void print_usage()
{
    std::cout << "Usage: huffman_util mode input_file output_file" << std::endl
              << "mode -- must be 'encode' or 'decode'." << std::endl;
}

int main(int argc, char **argv)
{
    double t1 = clock();

    if (argc != 4)
    {
        print_usage();
        return 0;
    }

    string mode = argv[1];

    ifstream in(argv[2], ifstream::binary);
    ofstream out(argv[3], ofstream::binary);

    const size_t BLOCK_SIZE = 1024 * 4 * 1024;
    if (mode == "encode")
    {
        Counter cnt;
        vector<uint8_t> encoded_dict;
        vector<uint8_t> block(BLOCK_SIZE), encoded_block;

        do
        {
            in.read(reinterpret_cast<char*>(block.data()), BLOCK_SIZE);
            block.resize(static_cast<size_t>(in.gcount()));

            cnt.feed(block);
        }
        while (in);

        in.close();
        //in.open(argv[2], ifstream::binary);
        ifstream nin(argv[2], ifstream::binary);

        /// at start: [8 bytes -- len of encoded dict][encoded dict]
        /// [8 bytes -- len of encoded block][encoded_block] ... ... ...
        Encoder encoder(cnt);
        encoder.initialize();
        encoder.encode_dict(encoded_dict);
        for (size_t i = 0; i < 8; i++)
            out.put(static_cast<char>((encoded_dict.size() >> (8 * i)) & 0xff));

        out.write(reinterpret_cast<char*>(encoded_dict.data()), encoded_dict.size());

        block.resize(BLOCK_SIZE);
        do
        {
            nin.read(reinterpret_cast<char*>(block.data()), BLOCK_SIZE);
            block.resize(static_cast<size_t>(nin.gcount()));
            encoder.encode_data(block, encoded_block);

            for (size_t i = 0; i < 8; i++)
                out.put(static_cast<char>((encoded_block.size() >> (8 * i)) & 0xff));
            out.write(reinterpret_cast<char*>(encoded_block.data()), encoded_block.size());
        }
        while (nin);
    }
    else if (mode == "decode")
    {
        try
        {
            Decoder decoder;

            bool eof = false;
            size_t dict_len = 0;
            for (size_t i = 0; i < 8; i++)
            {
                auto cur = in.get();
                if (cur < 0)
                {
                    eof = true;
                    break;
                }
                dict_len += static_cast<size_t>(cur) << (8 * i);
            }
            if (eof)
            {
                return -1;
            }

            vector <uint8_t> encoded_dict(dict_len);
            in.read(reinterpret_cast<char*>(encoded_dict.data()), dict_len);
            decoder.decode_dict(encoded_dict);

            vector <uint8_t> encoded_data, data;

            do
            {
                size_t block_len = 0;
                for (size_t i = 0; i < 8; i++)
                {
                    auto cur = in.get();
                    if (cur < 0)
                    {
                        eof = true;
                        break;
                    }
                    block_len += static_cast<size_t>(cur) << (8 * i);
                }
                if (eof)
                    break;

                encoded_data.resize(block_len);
                in.read(reinterpret_cast<char*>(encoded_data.data()), block_len);
                decoder.decode_data(encoded_data, data);
                out.write(reinterpret_cast<char*>(data.data()), data.size());
            }
            while (in);
        }
        catch (...) // TODO
        {
            std::cerr << "Error during decoding\n";
            return 1;
        }
    }
    else
        print_usage();

    in.close();
    out.close();

    double t2 = (clock() - t1) / CLOCKS_PER_SEC;
    std::clog << t2 << std::endl;

    return 0;
}
