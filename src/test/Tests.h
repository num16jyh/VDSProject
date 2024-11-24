//
// Created by tobias on 21.11.16.
//

#ifndef VDSPROJECT_TESTS_H
#define VDSPROJECT_TESTS_H

#include <gtest/gtest.h>
#include "../Manager.h"

using ClassProject::BDD_ID;

class ManagerTest : public ::testing::Test
{
public:
    ClassProject::Manager manager;
};

// Manager::createVar() test
TEST_F(ManagerTest, CreateVar)
{
    // Will try adding variables up to this ID.
    const BDD_ID testIDs = 100;

    // Start with an ID of 2 since IDs 0 and 1 must already be assigned to the
    // leaf nodes False and True respectively.
    for (BDD_ID i = 2; i <= testIDs; i++)
        EXPECT_EQ(manager.createVar(::std::to_string(i)), i);

    // Attempting to add a label that already exists should return its already
    // assigned ID.
    EXPECT_EQ(manager.createVar("False"), 0);
    EXPECT_EQ(manager.createVar("True"), 1);

    for (BDD_ID i = 2; i <= testIDs; i++)
        EXPECT_EQ(manager.createVar(::std::to_string(i)), i);
}

#endif
