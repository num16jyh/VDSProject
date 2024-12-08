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

    // topVar() is also tested for every logical function in its respective
    // test.
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

    // coFactorTrue() is also tested for every logical function in its
    // respective test.
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

    // coFactorFalse() is also tested for every logical function in its
    // respective test.
}

// Manager::ite() test
TEST_F(ManagerTest, Ite)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID b_id = manager.createVar("b");
    const BDD_ID c_id = manager.createVar("c");
    const BDD_ID next_id = c_id + 1;

    // True condition.
    EXPECT_EQ(manager.ite(TRUE_ID, a_id, b_id), a_id);

    // False condition.
    EXPECT_EQ(manager.ite(FALSE_ID, a_id, b_id), b_id);

    // Non-leaf node condition. Results in the creation of a new table entry.
    EXPECT_EQ(manager.ite(a_id, b_id, c_id), next_id);
    EXPECT_EQ(manager.topVar(next_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(next_id), b_id);
    EXPECT_EQ(manager.coFactorFalse(next_id), c_id);
}

// Manager::and2() test
TEST_F(ManagerTest, And2)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID b_id = manager.createVar("b");
    const BDD_ID next_id = b_id + 1;

    // Will not create new table entry.
    EXPECT_EQ(manager.and2(TRUE_ID, a_id), a_id);
    EXPECT_EQ(manager.and2(a_id, TRUE_ID), a_id);
    EXPECT_EQ(manager.and2(FALSE_ID, a_id), FALSE_ID);
    EXPECT_EQ(manager.and2(a_id, FALSE_ID), FALSE_ID);

    // Will create new table entry.
    EXPECT_EQ(manager.and2(a_id, b_id), next_id);

    // Will not create new table entry, already created by previous call.
    EXPECT_EQ(manager.and2(b_id, a_id), next_id);

    // Ensure the right triple was created.
    EXPECT_EQ(manager.topVar(next_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(next_id), b_id);
    EXPECT_EQ(manager.coFactorFalse(next_id), FALSE_ID);
}

// Manager::or2() test
TEST_F(ManagerTest, Or2)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID b_id = manager.createVar("b");
    const BDD_ID next_id = b_id + 1;

    // Will not create new table entry.
    EXPECT_EQ(manager.or2(TRUE_ID, a_id), TRUE_ID);
    EXPECT_EQ(manager.or2(a_id, TRUE_ID), TRUE_ID);
    EXPECT_EQ(manager.or2(FALSE_ID, a_id), a_id);
    EXPECT_EQ(manager.or2(a_id, FALSE_ID), a_id);

    // Will create new table entry.
    EXPECT_EQ(manager.or2(a_id, b_id), next_id);

    // Will not create new table entry, already created by previous call.
    EXPECT_EQ(manager.or2(b_id, a_id), next_id);

    // Ensure the right triple was created.
    EXPECT_EQ(manager.topVar(next_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(next_id), TRUE_ID);
    EXPECT_EQ(manager.coFactorFalse(next_id), b_id);
}

// Manager::xor2() test
TEST_F(ManagerTest, Xor2)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID b_id = manager.createVar("b");
    const BDD_ID next_id = b_id + 1;

    // Will not create new table entry.
    EXPECT_EQ(manager.xor2(FALSE_ID, a_id), a_id);
    EXPECT_EQ(manager.xor2(a_id, FALSE_ID), a_id);

    // Will create new table entry due to result being neg(a_id).
    EXPECT_EQ(manager.xor2(TRUE_ID, a_id), next_id);

    // Will not create new table entry, already created by previous call.
    EXPECT_EQ(manager.xor2(a_id, TRUE_ID), next_id);

    // Ensure the right triple was created.
    EXPECT_EQ(next_id, manager.neg(a_id));

    // Will create two new table entries.
    const BDD_ID xor_id = manager.xor2(a_id, b_id);
    EXPECT_TRUE(xor_id == next_id + 2);

    // Will not create new table entries, already created by previous call.
    EXPECT_EQ(manager.xor2(b_id, a_id), xor_id);

    // Ensure the right triples were created.
    EXPECT_EQ(manager.topVar(xor_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(xor_id), manager.neg(b_id));
    EXPECT_EQ(manager.coFactorFalse(xor_id), b_id);
}

// Manager::neg() test
TEST_F(ManagerTest, Neg)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID next_id = a_id + 1;

    // Will not create new table entry.
    EXPECT_EQ(manager.neg(TRUE_ID), FALSE_ID);
    EXPECT_EQ(manager.neg(FALSE_ID), TRUE_ID);

    // Will create new table entry.
    EXPECT_EQ(manager.neg(a_id), next_id);

    // Will not create new table entry, already created by previous call.
    EXPECT_EQ(manager.neg(a_id), next_id);

    // Ensure the right triple was created.
    EXPECT_EQ(manager.topVar(next_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(next_id), FALSE_ID);
    EXPECT_EQ(manager.coFactorFalse(next_id), TRUE_ID);
}

// Manager::nand2() test
TEST_F(ManagerTest, Nand2)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID b_id = manager.createVar("b");
    const BDD_ID next_id = b_id + 1;

    // Will not create new table entry.
    EXPECT_EQ(manager.nand2(FALSE_ID, a_id), TRUE_ID);
    EXPECT_EQ(manager.nand2(a_id, FALSE_ID), TRUE_ID);

    // Will create new table entry due to result being neg(a_id).
    EXPECT_EQ(manager.nand2(TRUE_ID, a_id), next_id);

    // Will not create new table entry, already created by previous call.
    EXPECT_EQ(manager.nand2(a_id, TRUE_ID), next_id);

    // Ensure the right triple was created.
    EXPECT_EQ(next_id, manager.neg(a_id));

    // Will create new table entries.
    const BDD_ID nand_id = manager.nand2(a_id, b_id);

    // Will not create new table entries, already created by previous call.
    EXPECT_EQ(manager.nand2(b_id, a_id), nand_id);

    // Ensure the right triples were created.
    EXPECT_EQ(manager.topVar(nand_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(nand_id), manager.neg(b_id));
    EXPECT_EQ(manager.coFactorFalse(nand_id), TRUE_ID);
}

// Manager::nor2() test
TEST_F(ManagerTest, Nor2)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID b_id = manager.createVar("b");
    const BDD_ID next_id = b_id + 1;

    // Will not create new table entry.
    EXPECT_EQ(manager.nor2(TRUE_ID, a_id), FALSE_ID);
    EXPECT_EQ(manager.nor2(a_id, TRUE_ID), FALSE_ID);

    // Will create new table entry due to result being neg(a_id).
    EXPECT_EQ(manager.nor2(FALSE_ID, a_id), next_id);

    // Will not create new table entry, already created by previous call.
    EXPECT_EQ(manager.nor2(a_id, FALSE_ID), next_id);

    // Ensure the right triple was created.
    EXPECT_EQ(next_id, manager.neg(a_id));

    // Will create new table entries.
    const BDD_ID nor_id = manager.nor2(a_id, b_id);

    // Will not create new table entries, already created by previous call.
    EXPECT_EQ(manager.nor2(b_id, a_id), nor_id);

    // Ensure the right triples were created.
    EXPECT_EQ(manager.topVar(nor_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(nor_id), FALSE_ID);
    EXPECT_EQ(manager.coFactorFalse(nor_id), manager.neg(b_id));
}

// Manager::xnor2() test
TEST_F(ManagerTest, Xnor2)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID b_id = manager.createVar("b");
    const BDD_ID next_id = b_id + 1;

    // Will not create new table entry.
    EXPECT_EQ(manager.xnor2(TRUE_ID, a_id), a_id);
    EXPECT_EQ(manager.xnor2(a_id, TRUE_ID), a_id);

    // Will create new table entry due to result being neg(a_id).
    EXPECT_EQ(manager.xnor2(FALSE_ID, a_id), next_id);

    // Will not create new table entry, already created by previous call.
    EXPECT_EQ(manager.xnor2(a_id, FALSE_ID), next_id);

    // Ensure the right triple was created.
    EXPECT_EQ(next_id, manager.neg(a_id));

    // Will create new table entries.
    const BDD_ID xnor_id = manager.xnor2(a_id, b_id);

    // Will not create new table entries, already created by previous call.
    EXPECT_EQ(manager.xnor2(b_id, a_id), xnor_id);

    // Ensure the right triples were created.
    EXPECT_EQ(manager.topVar(xnor_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(xnor_id), b_id);
    EXPECT_EQ(manager.coFactorFalse(xnor_id), manager.neg(b_id));
}

#endif
