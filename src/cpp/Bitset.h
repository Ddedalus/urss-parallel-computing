/**
 * @brief A class representing a subset of integers {0, 1, ..., size-1}
 * using 1 bit per element and special processor directive to speed up counting elements.
 * 
 * @file Bitset.h
 * @author Hubert Beres
 * @date 2018-06-27
 */
#ifndef __BITSET_H__
#define __BITSET_H__

#include <stdint.h>
#include <vector>

class Bitset
{
    private:
        std::vector<uint64_t> storage;
        uint32_t size;
        uint32_t chunks;

        Bitset(uint32_t size, uint64_t *content);

    public:
        /**
         * @brief Construct a new Bitset object capable of representing size elements
         * 
         * @param size - positions from 0 to size-1 will be available
         */
        Bitset(uint32_t size);

        /**
         * @brief Change a single element
         * 
         * @param pos starting at 0, unchecked
         * @param val is this element in the subset? boolean
         */
        void set(int pos, bool val=true);
        /**
         * @brief Check if element belongs to the subset 
         * 
         * @param pos starting at 0, unchecked
         */
        bool get(int pos);
        /**
         * @brief Calculate union in-place.
         * 
         * @param other subset of the same size, unchecked
         */
        void operator += (const Bitset& other);

        /**
         * @brief Calculate union set into a new Bitset object.
         * 
         * @param other other subset of the same size, unchecked
         * @return Bitset union set.
         */
        Bitset operator+(const Bitset& other);

        /**
         * @brief Order of this subset.
         */
        uint32_t count();
        /**
         * @brief Calculates complement set this - other
         * 
         * @param other subset of the same size, unchecked
         * @return Bitset complement
         */
        Bitset diff(const Bitset& other);
        /**
         * @brief Returns order of the complement, without calculating it explicitly.
         * This is faster because of __builtin__popcountll() directive can check 64
         * elements at a time.
         * 
         * @param other subset of the same size, unchecked
         * @return uint32_t number of elements which are in this but not in other.
         */
        uint32_t diff_size(const Bitset& other);
        /**
         * @brief Will set all bits to represent the same subset as other
         * 
         * @param other subset of the same size, unchecked
         */
        void copy_from(const Bitset& other);

        /**
         * @brief Checks if all elements are in this subset. Fast thanks to
         * __builtin__popcountll() and long comparison.
         */
        bool is_full();

};

#endif
