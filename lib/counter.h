#ifndef HUFFMAN_COUNTER_H
#define HUFFMAN_COUNTER_H

#include <cstdint>
#include <vector>
#include <cstddef>

namespace huffman
{
    class Counter
    {
    private:
        static const size_t SYMBOLS_CNT = 256;

        std::vector <size_t> counter;
    public:
        Counter();
        Counter(std::vector <uint8_t> const &data);

        void feed(std::vector <uint8_t> const &data);
        void get_current_values(std::vector < std::pair <uint8_t, size_t> > &result);
    };
}

#endif //HUFFMAN_COUNTER_H
