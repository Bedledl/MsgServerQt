#ifndef UNIQUE_KEY_H
#define UNIQUE_KEY_H

#include <mutex>
#include <set>

template <typename T>
class UniqueKey
{
    unsigned key;
    static unsigned iterator;
    static std::set<unsigned> reserved_keys;
    static std::mutex mutex;

    unsigned find_next_available_key(unsigned iter)
    {
        while (reserved_keys.contains(iter))
        {
            if (iter < std::numeric_limits<unsigned>::max())
            {
                ++iter;
            }
            else
            {
                iter = std::numeric_limits<unsigned>::min();
            }
        }
        return iter;
    }

public:
    UniqueKey()
    {
        std::lock_guard lock(mutex);

        key = find_next_available_key(iterator);
        ++iterator;
        reserved_keys.insert(key);
    };
    ~UniqueKey()
    {
        std::lock_guard lock(mutex);

        reserved_keys.erase(key);
    }
    UniqueKey(UniqueKey &k) = delete;
    UniqueKey &operator=(const UniqueKey &k) = delete;
    auto operator<=>(const UniqueKey &other) const = default;
    unsigned getKey() const { return key; }
};

template <typename T>
unsigned UniqueKey<T>::iterator = 0;

template <typename T>
std::set<unsigned> UniqueKey<T>::reserved_keys = {};

template <typename T>
std::mutex UniqueKey<T>::mutex = std::mutex();

#endif