//
// Created by tripack on 17-4-13.
//

#ifndef VE490_BITSET_MANIPULATE_H
#define VE490_BITSET_MANIPULATE_H

#include <boost/dynamic_bitset.hpp>

typedef boost::dynamic_bitset<> DBitset;


// Duplicates the first n bits and fill them in the "l"
// fillWith<16>(4, 0010 0100 0111 0110B)
//  => (0110 0110 0110 0110B)
static DBitset fillWith(size_t n, DBitset f) {
    return f.size() == n ? (f) : fillWith(n << 1, f | (f << n));
}

// For a bitset of length "length", fill its least significant
// n bits using 1
static inline DBitset ones(size_t length, size_t n) {
    DBitset mask(n, 0);
    mask.set().resize(length);
    return mask;
}

static inline DBitset extract(const DBitset& bitset,
                              const std::vector<size_t> ind) {
    DBitset result(ind.size());
    for (int i = 0; i < ind.size(); ++i)
        result[i] = bitset.test(ind[i]);
    return result;
};

static inline DBitset extract(const DBitset& bitset,
                              const DBitset& mask) {
    std::vector<size_t> ind;
    size_t x = mask.find_first();
    while (x != mask.npos) {
        ind.push_back(x);
        x = mask.find_next(x);
    }
    return extract(bitset, ind);
};

static inline size_t getLSB(const DBitset& bitset) {
    assert(!bitset.empty());
    return bitset.find_first();
}

// Puts a one dimensional bitset into a 2D array
// with inputs specified by col on the column
static std::vector<DBitset>
breakdown(const DBitset& bitset,
          const DBitset& row, const DBitset& col) {
    assert(row.size() == col.size());
    assert(row == ~col);
    assert(bitset.size() == (1ul << row.size()));

    size_t total = row.size();
    size_t width = 1ul << (row.count());
    size_t height = 1ul << (col.count());

    std::vector<DBitset> result(height, DBitset(width));

    for (size_t i = 0; i < bitset.size(); i++) {
        auto currInd = DBitset(total, i);
        auto key = extract(currInd, col);
        result[key.to_ulong()] = extract(currInd, row);
    }

    return result;
}


#endif //VE490_BITSET_MANIPULATE_H
