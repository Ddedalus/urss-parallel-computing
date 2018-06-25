#include "Bitset.h"
#include <iostream>

Bitset::Bitset(uint32_t size) : size(size){
    chunks = (size + 63)/64;
    storage.resize(chunks, 0ul);
}

Bitset::Bitset(uint32_t size, uint64_t* content) : Bitset(size){
    for(uint32_t k = 0; k < chunks; k++){
        storage[k] = content[k];
    }
}

void Bitset::set(int pos, bool val){
    storage[pos/64] = storage[pos/64] | (val ? 1ul << (pos%64) : 0ul);
}

bool Bitset::get(int pos){
    // std::cout << "Storage: " << storage[pos/64] << " ";
    uint64_t ass = storage[pos/64] & (1ul << pos%64);
    // std::cout << "OR " << ass;
    return  ass != 0ul;
}

void Bitset::operator+=(const Bitset& other){
    for(uint32_t k = 0; k < chunks; k++){
        storage[k] = storage[k] | other.storage[k];
    }
}

Bitset Bitset::operator+(const Bitset& other){
    uint64_t* ret = new uint64_t[chunks];
    for(int k = 0; k < chunks; k++){
        ret[k] = storage[k] | other.storage[k];
    }
    return Bitset(size, ret);
}

uint32_t Bitset::count(){
    uint32_t sum = 0;
    for(int k = 0; k < chunks; k++){
        sum += __builtin_popcountll(storage[k]);
    }
    return sum;
}

Bitset Bitset::diff(const Bitset& other){
    uint64_t* ret = new uint64_t[chunks];
    for(int k = 0; k < chunks; k++){
        ret[k] = storage[k] & ~other.storage[k];
    }
    return Bitset(size, ret);
}

uint32_t Bitset::diff_size(const Bitset& other){
    uint32_t sum = 0;
    for(int k = 0; k < chunks; k++){
        sum += __builtin_popcountll(storage[k] & ~other.storage[k]);
    }
    return sum;
}

void Bitset::copy_from(const Bitset& other){
    for(int k = 0; k < chunks; k++){
        storage[k] = other.storage[k];
    }
}

bool Bitset::is_full(){
    for(uint32_t k = 0; k < chunks; k++){
        if(storage[k] != INT64_MAX)
            return false;
    }
    return true;
}
