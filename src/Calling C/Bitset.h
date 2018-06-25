#ifndef __BITSET_H__
#define __BITSET_H__

#include <stdint.h>
#include <vector>

class Bitset
{
    private:
        std::vector<uint32_t> storage;
        uint32_t size;
        uint32_t chunks;

        Bitset(uint32_t size, uint64_t *content);

    public:
        Bitset(uint32_t size);

        void set(int pos, bool val);

        bool get(int pos);

        void operator += (const Bitset& other);

        Bitset operator+(const Bitset& other);

        uint32_t count();

        Bitset diff(const Bitset& other);

        uint32_t diff_size(const Bitset& other);

        void copy_from(const Bitset& other);

        bool is_full();

};

#endif
