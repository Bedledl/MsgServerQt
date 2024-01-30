#ifndef UNIQUE_KEY_H
#define UNIQUE_KEY_H

#include <mutex>
#include <set>

template <typename K>
class UniqueKeyGenerator
{
    static inline K iterator = std::numeric_limits<K>::min();
    static inline std::set<K> reserved_keys = {};
    static inline std::mutex mutex = std::mutex();
    static inline UniqueKeyGenerator<K> *instance = nullptr;

    UniqueKeyGenerator() = default;
    ~UniqueKeyGenerator() = default;
    UniqueKeyGenerator(const UniqueKeyGenerator &) = delete;
    UniqueKeyGenerator &operator=(const UniqueKeyGenerator &) = delete;

public:
    K requestKey()
    {
        std::lock_guard lock(mutex);

        while (reserved_keys.contains(iterator))
        {
            if (iterator < std::numeric_limits<K>::max())
            {
                ++iterator;
            }
            else
            {
                iterator = std::numeric_limits<K>::min();
            }
        }
        auto new_key = iterator;
        reserved_keys.emplace(std::move(new_key));
        ++iterator;
        return new_key;
    };
    void releaseKey(const K &key)
    {
        std::lock_guard lock(mutex);

        reserved_keys.erase(reserved_keys.find(key));
    }
    static UniqueKeyGenerator *get_instance()
    {
        std::lock_guard lock(mutex);

        if (!instance)
        {
            instance = new UniqueKeyGenerator();
        }

        return instance;
    }
};

#endif