#include "Bitset.h"

Bitset::Bitset(uint32_t size) : size(size)
{
    chunks = (size + 63) / 64;
    storage.resize(chunks, 0ul);
}

Bitset::Bitset(uint32_t size, uint64_t *content) : Bitset(size)
{
    for (uint32_t k = 0; k < chunks; k++)
    {
        storage[k] = content[k];
    }
}

void Bitset::set(int pos, bool val)
{
    if (val) // one only at pos
        storage[pos / 64] = storage[pos / 64] | (1ul << (pos % 64));
    else // zero only at pos
        storage[pos / 64] = storage[pos / 64] & ~(1ul << (pos % 64));
}

bool Bitset::get(int pos) const
{
    uint64_t ass = storage[pos / 64] & (1ul << pos % 64);
    return ass != 0ul;
}

void Bitset::operator+=(const Bitset &other)
{
    for (uint32_t k = 0; k < chunks; k++)
    {
        storage[k] = storage[k] | other.storage[k];
    }
}

Bitset Bitset::operator+(const Bitset &other)
{
    uint64_t ret[chunks];
    for (int k = 0; k < chunks; k++)
    {
        ret[k] = storage[k] | other.storage[k];
    }
    return Bitset(size, ret);
}

uint32_t Bitset::count() const
{
    uint32_t sum = 0;
    for (int k = 0; k < chunks; k++)
    {
        sum += __builtin_popcountll(storage[k]);
    }
    return sum;
}

Bitset Bitset::diff(const Bitset &other)
{
    uint64_t ret[chunks];
    for (int k = 0; k < chunks; k++)
    {
        ret[k] = storage[k] & ~other.storage[k];
    }
    return Bitset(size, ret);
}

uint32_t Bitset::diff_size(const Bitset &other)
{
    uint32_t sum = 0;
    for (int k = 0; k < chunks; k++)
    {
        sum += __builtin_popcountll(storage[k] & ~other.storage[k]);
    }
    return sum;
}

void Bitset::copy_from(const Bitset &other)
{
    for (int k = 0; k < chunks; k++)
    {
        storage[k] = other.storage[k];
    }
}

bool Bitset::is_full()
{
    for (uint32_t k = 0; k < chunks - 1; k++)
    {
        if (__builtin_popcountll(storage[k]) < 64)
            return false;
    } // check the full chunks quickly
    if (__builtin_popcountll(storage[chunks - 1]) < (size % 64))
        return false;

    return true;
}

std::ostream &operator<<(std::ostream &os, const Bitset &b)
{
    os << "B[";
    if (b.count() < 50)
    {
        for (int i = 0; i < b.size; i++)
        {
            if (b.get(i))
                os << i << ", ";
        }
    }
    else
    {
        for (int i = 0; i < b.size; i++)
            os << b.get(i) ? 1 : 0;
    }
    os << "]";
    return os;
}
