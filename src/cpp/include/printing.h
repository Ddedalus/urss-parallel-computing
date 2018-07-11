#ifndef __PRINTING_H__
#define __PRINTING_H__

template <class T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v)
{
    os << "[";
    for (const T &p : v)
        os << p << ", ";
    os << "]";
    return os;
}

template <class K, class V>
std::ostream &operator<<(std::ostream &os, const std::unordered_map<K, V> &m)
{
    os << "{";
    for (const V &p : m)
        os << p.first << ":" << p.second << ", ";
    os << "}";
    return os;
};

#endif