#include "counter.h"

namespace huffman
{
    Counter::Counter() : counter(SYMBOLS_CNT) {}

    Counter::Counter(std::vector<uint8_t> const &data) : Counter()
    {
        feed(data);
    }

    void Counter::feed(std::vector<uint8_t> const &data)
    {
        for (auto item : data)
            ++counter[item];
    }

    void Counter::get_current_values(std::vector<std::pair<uint8_t, size_t> > &result)
    {
        result.clear();
        for (size_t i = 0; i < SYMBOLS_CNT; ++i)
        {
            if (counter[i] > 0)
            {
                result.emplace_back(i, counter[i]);
            }
        }
    }
}