#include "FriendGroups.h"
#include "MyMap.h"
#include "MyUnionFind.h"

struct CFriendGroups::SImplementation {
    CMyMap<TName, CUnionFind::TElementID> DNameToID;
    CMyUnionFind DGroups;
    std::vector<TName> DNames;

    CUnionFind::TElementID GetOrAddPerson(const TName &name) {
        CUnionFind::TElementID ID;

        if (DNameToID.Find(name, ID)) {
            return ID;
        }

        ID = DGroups.AddElement();
        DNameToID.Add(name, ID);
        DNames.push_back(name);

        return ID;
    }
};

CFriendGroups::CFriendGroups(){
    DImplementation = std::make_unique<SImplementation>();
}

CFriendGroups::~CFriendGroups(){
    
}

bool CFriendGroups::SameFriendGroup(
    const TConnection &connection
) noexcept {
    CUnionFind::TElementID IDA;
    CUnionFind::TElementID IDB;

    if (!DImplementation->DNameToID.Find(
            std::get<0>(connection), IDA)) {
        return false;
    }

    if (!DImplementation->DNameToID.Find(
            std::get<1>(connection), IDB)) {
        return false;
    }

    return DImplementation->DGroups.Connected(IDA, IDB);
}

void CFriendGroups::AddFriendship(
    const TConnection &friendship
) noexcept {
    auto IDA = DImplementation->GetOrAddPerson(
        std::get<0>(friendship)
    );

    auto IDB = DImplementation->GetOrAddPerson(
        std::get<1>(friendship)
    );

    DImplementation->DGroups.Union(IDA, IDB);
}

std::vector<CFriendGroups::TConnection>
CFriendGroups::FriendshipsForSingleGroup() noexcept {
    std::vector<TConnection> Result;
    auto Count = DImplementation->DGroups.Size();

    std::vector<bool> Seen(Count, false);
    std::vector<CUnionFind::TElementID> Representatives;

    for (CUnionFind::TElementID ID = 0; ID < Count; ++ID) {
        auto Root = DImplementation->DGroups.Find(ID);

        if (!Seen[Root]) {
            Seen[Root] = true;
            Representatives.push_back(ID);
        }
    }

    for (std::size_t I = 1; I < Representatives.size(); ++I) {
        Result.emplace_back(
            DImplementation->DNames[Representatives[I - 1]],
            DImplementation->DNames[Representatives[I]]
        );
    }

    return Result;
}