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

// Manager::True() test
TEST_F(ManagerTest, True)
{
    EXPECT_EQ(manager.True(), true_id);
}

// Manager::False() test
TEST_F(ManagerTest, False)
{
    EXPECT_EQ(manager.False(), false_id);
}

// Manager::isConstant() test
TEST_F(ManagerTest, IsConstant)
{
    // True and False are leaf nodes / constants.
    EXPECT_TRUE(manager.isConstant(true_id));
    EXPECT_TRUE(manager.isConstant(false_id));

    // All other IDs are not constants; will check up to this ID.
    const BDD_ID testIDs = 100;

    for (BDD_ID i = first_var_id; i <= testIDs; i++)
        EXPECT_FALSE(manager.isConstant(i));
}

// Manager::isVariable() test
TEST_F(ManagerTest, IsVariable)
{
    // True and False are leaf nodes / constants, so not variables.
    EXPECT_FALSE(manager.isVariable(true_id));
    EXPECT_FALSE(manager.isVariable(false_id));

    // All other IDs are variables; will check up to this ID.
    const BDD_ID testIDs = 100;

    for (BDD_ID i = first_var_id; i <= testIDs; i++)
        EXPECT_TRUE(manager.isVariable(i));
}

// Manager::topVar() test
TEST_F(ManagerTest, TopVar)
{
    // Leaf variables return their own ID.
    EXPECT_EQ(manager.topVar(true_id), true_id);
    EXPECT_EQ(manager.topVar(false_id), false_id);

    // Variables added by createVar(), i.e. non-function nodes must return their
    // own ID; will add variables up to this ID.
    const BDD_ID testIDs = 100;

    for (BDD_ID i = first_var_id; i <= testIDs; i++)
        manager.createVar(::std::to_string(i));

    for (BDD_ID i = 0; i <= testIDs; i++)
        EXPECT_EQ(manager.topVar(i), i);

    //TODO: test with logical operations when these are implemented
}

// Manager::coFactorTrue() test
TEST_F(ManagerTest, CoFactorTrue)
{
    const BDD_ID a_id = manager.createVar("a");

    // Call with single argument.
    EXPECT_EQ(manager.coFactorTrue(true_id), true_id);
    EXPECT_EQ(manager.coFactorTrue(false_id), false_id);
    EXPECT_EQ(manager.coFactorTrue(a_id), true_id);

    // Call with two arguments.
    EXPECT_EQ(manager.coFactorTrue(true_id, true_id), true_id);
    EXPECT_EQ(manager.coFactorTrue(true_id, false_id), true_id);
    EXPECT_EQ(manager.coFactorTrue(true_id, a_id), true_id);
    EXPECT_EQ(manager.coFactorTrue(false_id, true_id), false_id);
    EXPECT_EQ(manager.coFactorTrue(false_id, false_id), false_id);
    EXPECT_EQ(manager.coFactorTrue(false_id, a_id), false_id);
    EXPECT_EQ(manager.coFactorTrue(a_id, true_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(a_id, false_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(a_id, a_id), true_id);

    //TODO: test function nodes when logical functions are implemented
}

// Manager::coFactorFalse() test
TEST_F(ManagerTest, CoFactorFalse)
{
    const BDD_ID a_id = manager.createVar("a");

    // Call with single argument.
    EXPECT_EQ(manager.coFactorFalse(true_id), true_id);
    EXPECT_EQ(manager.coFactorFalse(false_id), false_id);
    EXPECT_EQ(manager.coFactorFalse(a_id), false_id);

    // Call with two arguments.
    EXPECT_EQ(manager.coFactorFalse(true_id, true_id), true_id);
    EXPECT_EQ(manager.coFactorFalse(true_id, false_id), true_id);
    EXPECT_EQ(manager.coFactorFalse(true_id, a_id), true_id);
    EXPECT_EQ(manager.coFactorFalse(false_id, true_id), false_id);
    EXPECT_EQ(manager.coFactorFalse(false_id, false_id), false_id);
    EXPECT_EQ(manager.coFactorFalse(false_id, a_id), false_id);
    EXPECT_EQ(manager.coFactorFalse(a_id, true_id), a_id);
    EXPECT_EQ(manager.coFactorFalse(a_id, false_id), a_id);
    EXPECT_EQ(manager.coFactorFalse(a_id, a_id), false_id);

    //TODO: test function nodes when logical functions are implemented
}

#endif
