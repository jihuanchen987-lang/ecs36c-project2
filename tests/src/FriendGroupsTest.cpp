
#include <gtest/gtest.h>
#include "FriendGroups.h"
#include <iostream>


class FriendGroupsTest : public ::testing::Test {
    protected:
        std::unique_ptr<CFriendGroups> DFriendGroups = std::make_unique<CFriendGroups>();
};


TEST_F(FriendGroupsTest, SimpleFriendship){
    this->DFriendGroups->AddFriendship({"A","B"});
    EXPECT_TRUE(this->DFriendGroups->SameFriendGroup({"A","B"}));
}

TEST_F(FriendGroupsTest, SingleGroupTest){
    CFriendGroups Groups;
    
    EXPECT_FALSE(Groups.SameFriendGroup(CFriendGroups::TConnection{"A", "B"}));
    Groups.AddFriendship(CFriendGroups::TConnection{"A", "B"});
    EXPECT_TRUE(Groups.SameFriendGroup(CFriendGroups::TConnection{"A", "B"}));
    Groups.AddFriendship(CFriendGroups::TConnection{"C", "D"});
    EXPECT_FALSE(Groups.SameFriendGroup(CFriendGroups::TConnection{"A", "C"}));
    auto FriendshipsNeeded = Groups.FriendshipsForSingleGroup();
    
    EXPECT_EQ(FriendshipsNeeded.size(), 1);
    if(FriendshipsNeeded.size()){
        auto MinFriend = std::min(std::get<0>(FriendshipsNeeded[0]), std::get<1>(FriendshipsNeeded[0]));
        auto MaxFriend = std::max(std::get<0>(FriendshipsNeeded[0]), std::get<1>(FriendshipsNeeded[0]));
        EXPECT_TRUE(MinFriend == "A" || MinFriend == "B");
        EXPECT_TRUE(MaxFriend == "C" || MaxFriend == "D");
    }
}
TEST_F(FriendGroupsTest, EmptyQueriesDoNotCreatePeople) {
    EXPECT_TRUE(
        this->DFriendGroups->FriendshipsForSingleGroup().empty()
    );

    EXPECT_FALSE(
        this->DFriendGroups->SameFriendGroup({"Unknown", "Other"})
    );

    EXPECT_FALSE(
        this->DFriendGroups->SameFriendGroup({"Unknown", "Unknown"})
    );

    EXPECT_TRUE(
        this->DFriendGroups->FriendshipsForSingleGroup().empty()
    );
}
TEST_F(FriendGroupsTest, ConnectFourGroupsWithLimitedUsage) {
    CFriendGroups Groups;

    std::vector<std::string> Names{
        "A", "B", "C", "D", "E", "F", "G", "H"
    };

    Groups.AddFriendship({"A", "B"});
    Groups.AddFriendship({"C", "D"});
    Groups.AddFriendship({"E", "F"});
    Groups.AddFriendship({"G", "H"});

    auto Solution = Groups.FriendshipsForSingleGroup();
    ASSERT_EQ(Solution.size(), 3u);

    // Generating suggestions must not merge existing groups.
    EXPECT_FALSE(Groups.SameFriendGroup({"A", "C"}));

    for (const auto &Name : Names) {
        int Usage = 0;

        for (const auto &Connection : Solution) {
            Usage += std::get<0>(Connection) == Name;
            Usage += std::get<1>(Connection) == Name;
        }

        EXPECT_LE(Usage, 2);
    }

    for (const auto &Connection : Solution) {
        Groups.AddFriendship(Connection);
    }

    for (const auto &Name : Names) {
        EXPECT_TRUE(Groups.SameFriendGroup({"A", Name}));
    }

    EXPECT_TRUE(Groups.FriendshipsForSingleGroup().empty());
}
TEST_F(FriendGroupsTest, KnownPersonAndUnknownPerson) {
    this->DFriendGroups->AddFriendship({"A", "B"});

    EXPECT_FALSE(
        this->DFriendGroups->SameFriendGroup({"A", "Unknown"})
    );

    EXPECT_TRUE(
        this->DFriendGroups->SameFriendGroup({"A", "B"})
    );

    EXPECT_TRUE(
        this->DFriendGroups->FriendshipsForSingleGroup().empty()
    );
}