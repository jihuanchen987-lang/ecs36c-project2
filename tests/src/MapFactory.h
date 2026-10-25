#include "Map.h"
#include "MyMap.h"
#include <memory>


template <typename T>
struct SMapFactory;

template<typename K, typename V>
struct SMapFactory<CMyMap<K,V> > {
    static std::unique_ptr<CMap<K,V>> Create(){
        return std::make_unique<CMyMap<K,V>>();
    }
};
