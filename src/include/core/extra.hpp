#pragma once 

#include <utility>

namespace extra {

    using namespace std;

    /**
     * The next implementation is taken from here: 
     * Link: https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
     */
    inline void hash_combine(size_t seed) { }

    template <class T, class... Rest>
    inline void hash_combine(size_t& seed, const T& t, Rest... rest) {
        hash<T> hasher;
        seed ^= hasher(t) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        hash_combine(seed, rest...);
    }
}
