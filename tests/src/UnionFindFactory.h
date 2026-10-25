#include "UnionFind.h"
#include "MyUnionFind.h"
#include <memory>


template <typename T>
struct SUnionFindFactory;

template<>
struct SUnionFindFactory<CMyUnionFind>{
    static std::unique_ptr<CUnionFind> Create(){
        return std::make_unique<CMyUnionFind>();
    }
};
