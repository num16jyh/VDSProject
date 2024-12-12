//
// Created by tobias on 21.11.16.
//

#ifndef VDSPROJECT_TESTS_H
#define VDSPROJECT_TESTS_H

#include <gtest/gtest.h>
#include "../Manager.h"
#include <fstream>

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

    // Function nodes are not variables.
    const BDD_ID neg_id = manager.neg(a_id);
    EXPECT_FALSE(manager.isVariable(neg_id));
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
    const BDD_ID b_id = manager.createVar("b");
    const BDD_ID c_id = manager.createVar("c");
    const BDD_ID b_or_c_id = manager.or2(b_id, c_id);
    const BDD_ID nonexistent_id = b_or_c_id + 1;

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

    // Call with variable ID lower than the ID of the node's top variable.
    EXPECT_EQ(manager.coFactorTrue(b_or_c_id, c_id), TRUE_ID);

    // Call with variable ID higher than the ID of the node's top variable.
    EXPECT_EQ(manager.coFactorTrue(b_or_c_id, a_id), b_or_c_id);

    // coFactorTrue() is also tested for every logical function in its
    // respective test.
}

// Manager::coFactorFalse() test
TEST_F(ManagerTest, CoFactorFalse)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID b_id = manager.createVar("b");
    const BDD_ID c_id = manager.createVar("c");
    const BDD_ID b_or_c_id = manager.or2(b_id, c_id);
    const BDD_ID nonexistent_id = b_or_c_id + 1;

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

    // Call with variable ID lower than the ID of the node's top variable.
    EXPECT_EQ(manager.coFactorFalse(b_or_c_id, c_id), b_id);

    // Call with variable ID higher than the ID of the node's top variable.
    EXPECT_EQ(manager.coFactorTrue(b_or_c_id, a_id), b_or_c_id);

    // coFactorFalse() is also tested for every logical function in its
    // respective test.
}

// Manager::ite() test
TEST_F(ManagerTest, Ite)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID b_id = manager.createVar("b");
    const BDD_ID c_id = manager.createVar("c");
    const BDD_ID d_id = manager.createVar("d");
    const BDD_ID a_or_b_id = manager.or2(a_id, b_id);
    const BDD_ID a_and_b_id = manager.and2(a_id, b_id);
    const BDD_ID neg_b_id = manager.neg(b_id);

    // True condition.
    EXPECT_EQ(manager.ite(TRUE_ID, a_id, b_id), a_id);

    // False condition.
    EXPECT_EQ(manager.ite(FALSE_ID, a_id, b_id), b_id);

    // Check correct generation of a simple triple.
    const BDD_ID ite_abc_id = manager.ite(a_id, b_id, c_id);
    EXPECT_EQ(manager.topVar(ite_abc_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(ite_abc_id), b_id);
    EXPECT_EQ(manager.coFactorFalse(ite_abc_id), c_id);

    // Non-leaf node condition with equivalent then and else.
    EXPECT_EQ(manager.ite(a_id, b_id, b_id), b_id);

    // Check correct variable ordering and reduction.
    EXPECT_EQ(manager.ite(b_id, TRUE_ID, a_id), a_or_b_id);
    EXPECT_EQ(manager.ite(b_id, a_id, FALSE_ID), a_and_b_id);
    EXPECT_EQ(manager.ite(b_id, a_id, TRUE_ID), manager.or2(a_and_b_id, neg_b_id));
    EXPECT_EQ(manager.ite(b_id, FALSE_ID, a_id), manager.and2(a_id, neg_b_id));
    EXPECT_EQ(manager.topVar(manager.ite(d_id, TRUE_ID, c_id)), c_id);
    EXPECT_EQ(manager.topVar(manager.ite(d_id, FALSE_ID, c_id)), c_id);
}

// Manager::and2() test
TEST_F(ManagerTest, And2)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID b_id = manager.createVar("b");

    // Will not create new table entry.
    EXPECT_EQ(manager.and2(TRUE_ID, a_id), a_id);
    EXPECT_EQ(manager.and2(a_id, TRUE_ID), a_id);
    EXPECT_EQ(manager.and2(FALSE_ID, a_id), FALSE_ID);
    EXPECT_EQ(manager.and2(a_id, FALSE_ID), FALSE_ID);

    // Will create new table entry.
    const BDD_ID and_id = manager.and2(a_id, b_id);

    // Will not create new table entry, already created by previous call.
    EXPECT_EQ(manager.and2(b_id, a_id), and_id);

    // Ensure the right triple was created.
    EXPECT_EQ(manager.topVar(and_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(and_id), b_id);
    EXPECT_EQ(manager.coFactorFalse(and_id), FALSE_ID);
}

// Manager::or2() test
TEST_F(ManagerTest, Or2)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID b_id = manager.createVar("b");

    // Will not create new table entry.
    EXPECT_EQ(manager.or2(TRUE_ID, a_id), TRUE_ID);
    EXPECT_EQ(manager.or2(a_id, TRUE_ID), TRUE_ID);
    EXPECT_EQ(manager.or2(FALSE_ID, a_id), a_id);
    EXPECT_EQ(manager.or2(a_id, FALSE_ID), a_id);

    // Will create new table entry.
    const BDD_ID or_id = manager.or2(a_id, b_id);

    // Will not create new table entry, already created by previous call.
    EXPECT_EQ(manager.or2(b_id, a_id), or_id);

    // Ensure the right triple was created.
    EXPECT_EQ(manager.topVar(or_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(or_id), TRUE_ID);
    EXPECT_EQ(manager.coFactorFalse(or_id), b_id);
}

// Manager::xor2() test
TEST_F(ManagerTest, Xor2)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID b_id = manager.createVar("b");

    // Will not create new table entry.
    EXPECT_EQ(manager.xor2(FALSE_ID, a_id), a_id);
    EXPECT_EQ(manager.xor2(a_id, FALSE_ID), a_id);

    // Will create new table entry due to result being neg(a_id).
    BDD_ID xor_id = manager.xor2(TRUE_ID, a_id);

    // Will not create new table entry, already created by previous call.
    EXPECT_EQ(manager.xor2(a_id, TRUE_ID), xor_id);

    // Ensure the right triple was created.
    EXPECT_EQ(xor_id, manager.neg(a_id));

    // Will create new table entries.
    xor_id = manager.xor2(a_id, b_id);

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

    // Will not create new table entry.
    EXPECT_EQ(manager.neg(TRUE_ID), FALSE_ID);
    EXPECT_EQ(manager.neg(FALSE_ID), TRUE_ID);

    // Will create new table entry.
    const BDD_ID not_id = manager.neg(a_id);

    // Will not create new table entry, already created by previous call.
    EXPECT_EQ(manager.neg(a_id), not_id);

    // Ensure the right triple was created.
    EXPECT_EQ(manager.topVar(not_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(not_id), FALSE_ID);
    EXPECT_EQ(manager.coFactorFalse(not_id), TRUE_ID);
}

// Manager::nand2() test
TEST_F(ManagerTest, Nand2)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID b_id = manager.createVar("b");

    // Will not create new table entry.
    EXPECT_EQ(manager.nand2(FALSE_ID, a_id), TRUE_ID);
    EXPECT_EQ(manager.nand2(a_id, FALSE_ID), TRUE_ID);

    // Will create new table entry due to result being neg(a_id).
    BDD_ID nand_id = manager.nand2(TRUE_ID, a_id);

    // Will not create new table entry, already created by previous call.
    EXPECT_EQ(manager.nand2(a_id, TRUE_ID), nand_id);

    // Ensure the right triple was created.
    EXPECT_EQ(nand_id, manager.neg(a_id));

    // Will create new table entries.
    nand_id = manager.nand2(a_id, b_id);

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

    // Will not create new table entry.
    EXPECT_EQ(manager.nor2(TRUE_ID, a_id), FALSE_ID);
    EXPECT_EQ(manager.nor2(a_id, TRUE_ID), FALSE_ID);

    // Will create new table entry due to result being neg(a_id).
    BDD_ID nor_id = manager.nor2(FALSE_ID, a_id);

    // Will not create new table entry, already created by previous call.
    EXPECT_EQ(manager.nor2(a_id, FALSE_ID), nor_id);

    // Ensure the right triple was created.
    EXPECT_EQ(nor_id, manager.neg(a_id));

    // Will create new table entries.
    nor_id = manager.nor2(a_id, b_id);

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

    // Will not create new table entry.
    EXPECT_EQ(manager.xnor2(TRUE_ID, a_id), a_id);
    EXPECT_EQ(manager.xnor2(a_id, TRUE_ID), a_id);

    // Will create new table entry due to result being neg(a_id).
    BDD_ID xnor_id = manager.xnor2(FALSE_ID, a_id);

    // Will not create new table entry, already created by previous call.
    EXPECT_EQ(manager.xnor2(a_id, FALSE_ID), xnor_id);

    // Ensure the right triple was created.
    EXPECT_EQ(xnor_id, manager.neg(a_id));

    // Will create new table entries.
    xnor_id = manager.xnor2(a_id, b_id);

    // Will not create new table entries, already created by previous call.
    EXPECT_EQ(manager.xnor2(b_id, a_id), xnor_id);

    // Ensure the right triples were created.
    EXPECT_EQ(manager.topVar(xnor_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(xnor_id), b_id);
    EXPECT_EQ(manager.coFactorFalse(xnor_id), manager.neg(b_id));
}

// Manager::getTopVarName() test
TEST_F(ManagerTest, GetTopVarName)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID b_id = manager.createVar("b");
    const BDD_ID op_id = manager.and2(a_id, b_id);

    EXPECT_EQ(manager.getTopVarName(a_id), "a");
    EXPECT_EQ(manager.getTopVarName(b_id), "b");
    EXPECT_EQ(manager.getTopVarName(op_id), "a");
    EXPECT_EQ(manager.getTopVarName(op_id + 1), "UNKNOWN");
}

// Manager::findNodes() test
TEST_F(ManagerTest, FindNodes)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID b_id = manager.createVar("b");
    const BDD_ID c_id = manager.createVar("c");
    const BDD_ID d_id = manager.createVar("d");
    const BDD_ID op_id = manager.and2(manager.nor2(a_id, b_id), manager.xor2(c_id, d_id));

    std::set<BDD_ID> nodes;
    manager.findNodes(op_id, nodes);

    // Checks presence of all reachable nodes of a function in the nodes set.
    // Returns true if all present, false otherwise.
    std::function<bool(BDD_ID)> f = [&](BDD_ID id) -> bool {
        if (!nodes.count(id))
            return false;

        if (!manager.isConstant(id))
        {
            if (!f(manager.coFactorTrue(id)))
                return false;

            if (!f(manager.coFactorFalse(id)))
                return false;
        }

        return true;
    };

    EXPECT_TRUE(f(op_id));
}

// Manager::findVars() test
TEST_F(ManagerTest, FindVars)
{
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID b_id = manager.createVar("b");
    const BDD_ID c_id = manager.createVar("c");
    const BDD_ID d_id = manager.createVar("d");
    const BDD_ID op_id = manager.and2(manager.nor2(a_id, b_id), manager.xor2(c_id, d_id));

    std::set<BDD_ID> vars;
    manager.findVars(op_id, vars);

    // Checks presence of all reachable variables of a function in the variables
    // set. Returns true if all present, false otherwise.
    std::function<bool(BDD_ID)> f = [&](BDD_ID id) -> bool {
        if (!manager.isConstant(id))
        {
            if (!vars.count(manager.topVar(id)))
                return false;

            if (id != manager.topVar(id))
            {
                if (!f(manager.coFactorTrue(id)))
                    return false;

                if (!f(manager.coFactorFalse(id)))
                    return false;
            }
        }

        return true;
    };

    EXPECT_TRUE(f(op_id));
}

// Manager::uniqueTableSize() test
TEST_F(ManagerTest, UniqueTableSize)
{
    unsigned int size = 2; // starts with the True and False nodes by default

    // createVar() should only create one node.
    for (unsigned int i = 0; i < 10; i++)
    {
        manager.createVar("");
        size++;
        EXPECT_EQ(manager.uniqueTableSize(), size);
    }
}

// Manager::visualizeBDD() test
TEST_F(ManagerTest, VisualizeBDD)
{
    const std::string filepath = "BDD.dot";
    const BDD_ID a_id = manager.createVar("a");
    const BDD_ID b_id = manager.createVar("b");
    const BDD_ID c_id = manager.createVar("c");
    const BDD_ID d_id = manager.createVar("d");

    BDD_ID f_id = manager.and2(manager.or2(a_id, b_id), manager.and2(c_id, d_id));
    //BDD_ID f_id = manager.nor2(b_id, a_id);

    manager.visualizeBDD(filepath, f_id);

    std::ifstream file(filepath);
    EXPECT_TRUE(file.is_open());
}

#endif
