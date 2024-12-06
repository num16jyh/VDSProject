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
    const BDD_ID FIRST_VAR_ID = 2;
    const BDD_ID TRUE_ID = 1;
    const BDD_ID FALSE_ID = 0;
};

// Manager::createVar() test
TEST_F(ManagerTest, CreateVar)
{
    // Starts with an ID of 2 since IDs 0 and 1 must already be assigned to the
    // leaf nodes False and True respectively.
    const BDD_ID a_id = manager.createVar("a");
    EXPECT_EQ(a_id, FIRST_VAR_ID);

    // Labels are not unique.
    EXPECT_EQ(manager.createVar("a"), a_id + 1);
}

// Manager::True() test
TEST_F(ManagerTest, True)
{
    EXPECT_EQ(manager.True(), TRUE_ID);
}

// Manager::False() test
TEST_F(ManagerTest, False)
{
    EXPECT_EQ(manager.False(), FALSE_ID);
}

// Manager::isConstant() test
TEST_F(ManagerTest, IsConstant)
{
    // True and False are leaf nodes / constants.
    EXPECT_TRUE(manager.isConstant(TRUE_ID));
    EXPECT_TRUE(manager.isConstant(FALSE_ID));

    // IDs returned by createVar() are not constants.
    const BDD_ID a_id = manager.createVar("a");
    EXPECT_FALSE(manager.isConstant(a_id));

    // Non-existent IDs are not constants.
    const BDD_ID nonexistent_id = a_id + 1;
    EXPECT_FALSE(manager.isConstant(nonexistent_id));
}

// Manager::isVariable() test
TEST_F(ManagerTest, IsVariable)
{
    // True and False are leaf nodes / constants, so not variables.
    EXPECT_FALSE(manager.isVariable(TRUE_ID));
    EXPECT_FALSE(manager.isVariable(FALSE_ID));

    // IDs returned by createVar() are variables.
    const BDD_ID a_id = manager.createVar("a");
    EXPECT_TRUE(manager.isVariable(a_id));

    // Non-existent IDs are not variables.
    const BDD_ID nonexistent_id = a_id + 1;
    EXPECT_FALSE(manager.isVariable(nonexistent_id));
}

// Manager::topVar() test
TEST_F(ManagerTest, TopVar)
{
    // Leaf variables return their own ID.
    EXPECT_EQ(manager.topVar(TRUE_ID), TRUE_ID);
    EXPECT_EQ(manager.topVar(FALSE_ID), FALSE_ID);

    // Variables added using createVar(), i.e. non-function nodes must return
    // their own ID.
    const BDD_ID a_id = manager.createVar("a");
    EXPECT_EQ(manager.topVar(a_id), a_id);

    // Passing as argument a non-existent ID should return the argument value.
    const BDD_ID nonexistent_id = a_id + 1;
    EXPECT_EQ(manager.topVar(nonexistent_id), nonexistent_id);

    //TODO: test with logical operations when these are implemented
}

// Manager::coFactorTrue() test
TEST_F(ManagerTest, CoFactorTrue)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID nonexistent_id = a_id + 1;

    // Call with single argument.
    EXPECT_EQ(manager.coFactorTrue(TRUE_ID), TRUE_ID);
    EXPECT_EQ(manager.coFactorTrue(FALSE_ID), FALSE_ID);
    EXPECT_EQ(manager.coFactorTrue(a_id), TRUE_ID);

    // Call with single non-existent ID argument. Return the argument.
    EXPECT_EQ(manager.coFactorTrue(nonexistent_id), nonexistent_id);

    // Call with two arguments.
    EXPECT_EQ(manager.coFactorTrue(TRUE_ID, TRUE_ID), TRUE_ID);
    EXPECT_EQ(manager.coFactorTrue(TRUE_ID, FALSE_ID), TRUE_ID);
    EXPECT_EQ(manager.coFactorTrue(TRUE_ID, a_id), TRUE_ID);
    EXPECT_EQ(manager.coFactorTrue(FALSE_ID, TRUE_ID), FALSE_ID);
    EXPECT_EQ(manager.coFactorTrue(FALSE_ID, FALSE_ID), FALSE_ID);
    EXPECT_EQ(manager.coFactorTrue(FALSE_ID, a_id), FALSE_ID);
    EXPECT_EQ(manager.coFactorTrue(a_id, TRUE_ID), a_id);
    EXPECT_EQ(manager.coFactorTrue(a_id, FALSE_ID), a_id);
    EXPECT_EQ(manager.coFactorTrue(a_id, a_id), TRUE_ID);

    // Call with two arguments where the function ID does not exist. Return the
    // function ID argument.
    EXPECT_EQ(manager.coFactorTrue(nonexistent_id, a_id), nonexistent_id);

    //TODO: test function nodes when logical functions are implemented
}

// Manager::coFactorFalse() test
TEST_F(ManagerTest, CoFactorFalse)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID nonexistent_id = a_id + 1;

    // Call with single argument.
    EXPECT_EQ(manager.coFactorFalse(TRUE_ID), TRUE_ID);
    EXPECT_EQ(manager.coFactorFalse(FALSE_ID), FALSE_ID);
    EXPECT_EQ(manager.coFactorFalse(a_id), FALSE_ID);

    // Call with single non-existent ID argument. Return the argument.
    EXPECT_EQ(manager.coFactorFalse(nonexistent_id), nonexistent_id);

    // Call with two arguments.
    EXPECT_EQ(manager.coFactorFalse(TRUE_ID, TRUE_ID), TRUE_ID);
    EXPECT_EQ(manager.coFactorFalse(TRUE_ID, FALSE_ID), TRUE_ID);
    EXPECT_EQ(manager.coFactorFalse(TRUE_ID, a_id), TRUE_ID);
    EXPECT_EQ(manager.coFactorFalse(FALSE_ID, TRUE_ID), FALSE_ID);
    EXPECT_EQ(manager.coFactorFalse(FALSE_ID, FALSE_ID), FALSE_ID);
    EXPECT_EQ(manager.coFactorFalse(FALSE_ID, a_id), FALSE_ID);
    EXPECT_EQ(manager.coFactorFalse(a_id, TRUE_ID), a_id);
    EXPECT_EQ(manager.coFactorFalse(a_id, FALSE_ID), a_id);
    EXPECT_EQ(manager.coFactorFalse(a_id, a_id), FALSE_ID);

    // Call with two arguments where the function ID does not exist. Return the
    // function ID argument.
    EXPECT_EQ(manager.coFactorFalse(nonexistent_id, a_id), nonexistent_id);

    //TODO: test function nodes when logical functions are implemented
}

#endif
