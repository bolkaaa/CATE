#ifndef UTIL_HPP
#define UTIL_HPP

#include <vector>
#include <unordered_map>

using std::vector;
using std::unordered_map;

/* Given a std::unordered_map, get a std::vector of its keys. */
template <class A, class B>
static inline vector<A> get_keys(unordered_map<A, B> m)
{
    vector<A> keys;

    for (auto &elem : m)
    {
        keys.emplace_back(elem.first);
    }

    return keys;
}

#endif
