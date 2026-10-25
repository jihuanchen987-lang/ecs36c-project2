#ifndef MY_MAP_H
#define MY_MAP_H

#include "Map.h"
#include <cstddef>
#include <functional>
#include <list>
#include <vector>

template <class K, class V>
class CMyMap : public CMap<K, V> {
private:
    struct SEntry {
        K Key;
        V Value;
    };

    std::vector<std::list<SEntry>> DBuckets;
    std::size_t DSize = 0;

    std::size_t BucketIndex(const K &key) const {
        return std::hash<K>{}(key) % DBuckets.size();
    }

    void Rehash();

public:
    CMyMap() : DBuckets(16) {}

    std::size_t Size() const override {
        return DSize;
    }

    bool Add(const K &key, V val) override;
    bool Remove(const K &key) override;
    bool Find(const K &key, V &val) const override;
    void ForEach(
        typename CMap<K, V>::TEntityFunction fun
    ) override;
};
template <class K, class V>
bool CMyMap<K, V>::Add(const K &key, V val) {
    std::size_t Index = BucketIndex(key);

    for (const auto &Entry : DBuckets[Index]) {
        if (Entry.Key == key) {
            return false;
        }
    }

    if (DSize + 1 > DBuckets.size()) {
        Rehash();
        Index = BucketIndex(key);
    }

    DBuckets[Index].push_back({key, val});
    ++DSize;

    return true;
}
template <class K, class V>
bool CMyMap<K, V>::Remove(const K &key) {
    auto &Bucket = DBuckets[BucketIndex(key)];

    for (auto It = Bucket.begin(); It != Bucket.end(); ++It) {
        if (It->Key == key) {
            Bucket.erase(It);
            --DSize;
            return true;
        }
    }

    return false;
}
 template <class K, class V>
void CMyMap<K, V>::ForEach(
    typename CMap<K, V>::TEntityFunction fun
) {
    for (const auto &Bucket : DBuckets) {
        for (const auto &Entry : Bucket) {
            fun(Entry.Key, Entry.Value);
        }
    }
}
template <class K, class V>
bool CMyMap<K, V>::Find(const K &key, V &val) const {
    const auto &Bucket = DBuckets[BucketIndex(key)];

    for (const auto &Entry : Bucket) {
        if (Entry.Key == key) {
            val = Entry.Value;
            return true;
        }
    }

    return false;
}

template <class K, class V>
void CMyMap<K, V>::Rehash() {
    std::vector<std::list<SEntry>> NewBuckets(
        DBuckets.size() * 2
    );

    for (auto &Bucket : DBuckets) {
        auto It = Bucket.begin();

        while (It != Bucket.end()) {
            auto Current = It++;
            std::size_t Index =
                std::hash<K>{}(Current->Key) % NewBuckets.size();

            NewBuckets[Index].splice(
                NewBuckets[Index].end(), Bucket, Current
            );
        }
    }

    DBuckets.swap(NewBuckets);
}

#endif