#ifndef VDSPROJECT_REACHABILITY_TESTS_H
#define VDSPROJECT_REACHABILITY_TESTS_H

#include <gtest/gtest.h>
#include "Reachability.h"

using namespace ClassProject;

struct ReachabilityTest : testing::Test
{
    std::unique_ptr<ClassProject::ReachabilityInterface> fsm2 =
        std::make_unique<ClassProject::Reachability>(2);

    std::vector<BDD_ID> stateVars2 = fsm2->getStates();
    std::vector<BDD_ID> transitionFunctions;
};

TEST_F(ReachabilityTest, HowTo_Example)
{ /* NOLINT */

    BDD_ID s0 = stateVars2.at(0);
    BDD_ID s1 = stateVars2.at(1);

    transitionFunctions.push_back(fsm2->neg(s0)); // s0' = not(s0)
    transitionFunctions.push_back(fsm2->neg(s1)); // s1' = not(s1)
    fsm2->setTransitionFunctions(transitionFunctions);

    fsm2->setInitState({false, false});

    ASSERT_TRUE(fsm2->isReachable({false, false}));
    ASSERT_FALSE(fsm2->isReachable({false, true}));
    ASSERT_FALSE(fsm2->isReachable({true, false}));
    ASSERT_TRUE(fsm2->isReachable({true, true}));
}

//Test if the constructor throws a runtime error for invalid input size (state size = 0)
TEST_F(ReachabilityTest, ReachabilityConstructorError)
{
    EXPECT_THROW(
        { ClassProject::Reachability Reachability(0); },
        std::runtime_error); // State size 0 should throw an error
}

// Test if setInitState throws exceptions for invalid state vectors and works correctly for valid ones
TEST_F(ReachabilityTest, SetInitState)
{
    // Test for state vectors with incorrect sizes
    EXPECT_THROW(
        { fsm2->setInitState({true, false, false}); }, std::runtime_error); // Too many states
    EXPECT_THROW({ fsm2->setInitState({false}); }, std::runtime_error); // Too few states

    // Test for valid state vectors
    EXPECT_NO_THROW({ fsm2->setInitState({false, false}); });
    EXPECT_NO_THROW({ fsm2->setInitState({false, true}); });
}

// Test the isReachable function without setting any transition functions (default identity transition)
TEST_F(ReachabilityTest, isReachable)
{
    // Test for initial state {false, false}
    fsm2->setInitState({false, false}); // State A
    EXPECT_TRUE(fsm2->isReachable({false, false})); // Initial state is reachable
    EXPECT_FALSE(fsm2->isReachable({false, true})); // Not reachable
    EXPECT_FALSE(fsm2->isReachable({true, false})); // Not reachable
    EXPECT_FALSE(fsm2->isReachable({true, true})); // Not reachable

    // Test for initial state {true, false}
    fsm2->setInitState({true, false}); // State B
    EXPECT_FALSE(fsm2->isReachable({false, false})); // Not reachable
    EXPECT_TRUE(fsm2->isReachable({true, false})); // Initial state is reachable
    EXPECT_FALSE(fsm2->isReachable({false, true})); // Not reachable
    EXPECT_FALSE(fsm2->isReachable({true, true})); // Not reachable

    // Test for initial state {false, true}
    fsm2->setInitState({false, true}); // State C
    EXPECT_FALSE(fsm2->isReachable({true, false})); // Not reachable
    EXPECT_FALSE(fsm2->isReachable({false, false})); // Not reachable
    EXPECT_TRUE(fsm2->isReachable({false, true})); // Initial state is reachable
    EXPECT_FALSE(fsm2->isReachable({true, true})); // Not reachable

    // Test for initial state {true, true}
    fsm2->setInitState({true, true}); // State D
    EXPECT_FALSE(fsm2->isReachable({true, false})); // Not reachable
    EXPECT_FALSE(fsm2->isReachable({false, false})); // Not reachable
    EXPECT_FALSE(fsm2->isReachable({false, true})); // Not reachable
    EXPECT_TRUE(fsm2->isReachable({true, true})); // Initial state is reachable
}

// Test if getStates returns the correct state bits
TEST_F(ReachabilityTest, getStates)
{
    std::vector<BDD_ID> expectedStates = {2, 4}; // Assuming the state bits are assigned IDs 2 and 4
    EXPECT_EQ(expectedStates, fsm2->getStates());
}

// Test if getInputs returns the correct input bits
TEST_F(ReachabilityTest, GetInputsTest)
{
    // Assuming the input bits are assigned IDs 3 and 4 (since state bits are 2 and 4)
    std::vector<BDD_ID> expectedInputs = {3, 4};
    EXPECT_EQ(expectedInputs, fsm2->getInputs());
}

// Test if isReachable throws exceptions for invalid state vectors
TEST_F(ReachabilityTest, isReachableError)
{
    // Test for state vectors with incorrect sizes
    EXPECT_THROW(
        { fsm2->isReachable({true, false, false}); }, std::runtime_error); // Too many states
    EXPECT_NO_THROW({ fsm2->isReachable({true, false}); }); // Valid state vector
}

TEST_F(ReachabilityTest, StateDistanceTest)
{
    // s0' = !s1 and s1' = s0
    BDD_ID s0 = stateVars2.at(0);
    BDD_ID s1 = stateVars2.at(1);
    std::vector<BDD_ID> transitionFunctions;
    transitionFunctions.push_back(fsm2->neg(s1)); // s0' = !s1
    transitionFunctions.push_back(s0); // s1' = s0
    fsm2->setTransitionFunctions(transitionFunctions);

    // Set initial state: {false, false}
    fsm2->setInitState({false, false});

    // Verify state distances according to the documented cycle:
    EXPECT_EQ(fsm2->stateDistance({false, false}), 0); // initial state, distance 0
    EXPECT_EQ(fsm2->stateDistance({true, false}), 1); // reached in 1 transition
    EXPECT_EQ(fsm2->stateDistance({true, true}), 2); // reached in 2 transitions
    EXPECT_EQ(fsm2->stateDistance({false, true}), 3); // reached in 3 transitions
}

TEST_F(ReachabilityTest, StateDistanceError)
{
    // Test for invalid state vector sizes
    EXPECT_THROW(
        { fsm2->stateDistance({true, false, false}); }, std::runtime_error); // Too many states
    EXPECT_THROW({ fsm2->stateDistance({false}); }, std::runtime_error); // Too few states
}

TEST_F(ReachabilityTest, SetTransitionFunctionsTest)
{
    BDD_ID s0 = stateVars2.at(0);
    BDD_ID s1 = stateVars2.at(1);

    // Valid transition functions: s0' = not(s0), s1' = not(s1)
    transitionFunctions.push_back(fsm2->neg(s0)); // s0' = not(s0)
    transitionFunctions.push_back(fsm2->neg(s1)); // s1' = not(s1)
    EXPECT_NO_THROW(fsm2->setTransitionFunctions(transitionFunctions));

    // Test for invalid number of transition functions
    std::vector<BDD_ID> invalidTransitionFunctions;
    invalidTransitionFunctions.push_back(fsm2->neg(s0)); // Only one transition function
    EXPECT_THROW(fsm2->setTransitionFunctions(invalidTransitionFunctions),
                 std::runtime_error); // Too few functions

    invalidTransitionFunctions.push_back(fsm2->neg(s1));
    invalidTransitionFunctions.push_back(s0); // Extra function
    EXPECT_THROW(fsm2->setTransitionFunctions(invalidTransitionFunctions),
                 std::runtime_error); // Too many functions

    // Test for unknown BDD_ID in transition functions
    invalidTransitionFunctions.clear();
    invalidTransitionFunctions.push_back(1000); // Invalid BDD_ID
    invalidTransitionFunctions.push_back(s1);
    EXPECT_THROW(fsm2->setTransitionFunctions(invalidTransitionFunctions),
                 std::runtime_error); // Unknown ID
}
#endif
