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
    const BDD_ID first_var_id = 2;
    const BDD_ID true_id = 1;
    const BDD_ID false_id = 0;
};

// Manager::createVar() test
TEST_F(ManagerTest, CreateVar)
{
    // Will try adding variables up to this ID.
    const BDD_ID testIDs = 100;

    // Start with an ID of 2 since IDs 0 and 1 must already be assigned to the
    // leaf nodes False and True respectively.
    for (BDD_ID i = first_var_id; i <= testIDs; i++)
        EXPECT_EQ(manager.createVar(::std::to_string(i)), i);

    // Attempting to add a label that already exists should return its already
    // assigned ID.
    EXPECT_EQ(manager.createVar("False"), false_id);
    EXPECT_EQ(manager.createVar("True"), true_id);

    for (BDD_ID i = first_var_id; i <= testIDs; i++)
        EXPECT_EQ(manager.createVar(::std::to_string(i)), i);
}

#endif
