#ifndef MY_UNION_FIND_H
#define MY_UNION_FIND_H

#include "UnionFind.h"
#include <vector>

class CMyUnionFind : public CUnionFind {
private:
    std::vector<TElementID> DParent;
    std::vector<std::size_t> DGroupSize;
    std::size_t DGroupCount = 0;

public:
    std::size_t Size() const override;
    std::size_t GroupCount() const override;
    TElementID AddElement() override;
    bool Union(TElementID a, TElementID b) override;
    TGroupID Find(TElementID a) const override;
    bool Connected(TElementID a, TElementID b) override;
};
inline std::size_t CMyUnionFind::Size() const {
    return DParent.size();
}

inline std::size_t CMyUnionFind::GroupCount() const {
    return DGroupCount;
}

inline CUnionFind::TElementID CMyUnionFind::AddElement() {
    TElementID NewID = DParent.size();

    DParent.push_back(NewID);
    DGroupSize.push_back(1);
    ++DGroupCount;

    return NewID;
}
inline CUnionFind::TGroupID CMyUnionFind::Find(TElementID a) const {
    if (a >= DParent.size()) {
        return InvalidGroupID;
    }

    while (DParent[a] != a) {
        a = DParent[a];
    }

    return a;
}
inline bool CMyUnionFind::Union(TElementID a, TElementID b) {
    TGroupID RootA = Find(a);
    TGroupID RootB = Find(b);

    if (RootA == InvalidGroupID || RootB == InvalidGroupID) {
        return false;
    }

    if (RootA == RootB) {
        return false;
    }

    if (DGroupSize[RootA] < DGroupSize[RootB]) {
        DParent[RootA] = RootB;
        DGroupSize[RootB] += DGroupSize[RootA];
    } else {
        DParent[RootB] = RootA;
        DGroupSize[RootA] += DGroupSize[RootB];
    }

    --DGroupCount;
    return true;
}
inline bool CMyUnionFind::Connected(TElementID a, TElementID b) {
    TGroupID RootA = Find(a);
    TGroupID RootB = Find(b);

    if (RootA == InvalidGroupID || RootB == InvalidGroupID) {
        return false;
    }

    return RootA == RootB;
}
#endif