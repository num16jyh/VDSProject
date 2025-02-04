#include "Reachability.h"
#include <stdexcept>

namespace ClassProject {

Reachability::Reachability(unsigned int stateSize, unsigned int inputSize)
    : ReachabilityInterface(stateSize, inputSize)
{
    if (stateSize == 0)
        throw std::runtime_error("Reachability constructor stateSize argument is 0.");

    for (std::size_t i = 0; i < stateSize; i++)
    {
        const BDD_ID s = createVar(std::string("s") + std::to_string(i));
        m_stateVars.push_back(s);
        m_transitionFunctions.push_back(s);
        m_initState.push_back(false);

        /* Create next state variables for the transition relation. */
        const BDD_ID s_next = createVar(std::string("s'") + std::to_string(i));
        m_nextStateVars.push_back(s_next);
    }

    computeTransitionRelation();
    computeInitStateCharFunction();

    for (std::size_t i = 0; i < inputSize; i++)
        m_inputVars.push_back(createVar(std::string("x") + std::to_string(i)));
}

const std::vector<BDD_ID> &Reachability::getStates() const
{
    return m_stateVars;
}

const std::vector<BDD_ID> &Reachability::getInputs() const
{
    return m_inputVars;
}

bool Reachability::isReachable(const std::vector<bool> &stateVector)
{
    if (stateVector.size() != m_stateVars.size())
        throw std::runtime_error(
            "isReachable() vector argument size does not match the number of input variables.");

    BDD_ID reachableSet = m_initStateCharFunc;
    BDD_ID previousReachableSet;

    while (true)
    {
        previousReachableSet = reachableSet;

        /* Compute image of current reachable set. */
        BDD_ID img = and2(reachableSet, m_transitionRelation);

        for (std::size_t i = 0; i < m_stateVars.size(); i++)
            img = existQuant(img, m_stateVars[i]);

        for (std::size_t i = 0; i < m_inputVars.size(); i++)
            img = existQuant(img, m_inputVars[i]);

        /* Because the characteristic function of the state set is a function of
         * s but the image is a function of s', we must rename s' to s in the
         * image function before adding it to the set. */

        for (std::size_t i = 0; i < m_stateVars.size(); i++)
            img = and2(img, xnor2(m_stateVars[i], m_nextStateVars[i]));

        for (std::size_t i = 0; i < m_nextStateVars.size(); i++)
            img = existQuant(img, m_nextStateVars[i]);

        reachableSet = or2(reachableSet, img);

        if (reachableSet == previousReachableSet)
            break;
    }

    /* Check if argument exists in reachable set. */
    for (std::size_t i = 0; i < m_stateVars.size(); i++)
    {
        if (stateVector[i])
            reachableSet = coFactorTrue(reachableSet, m_stateVars[i]);
        else
            reachableSet = coFactorFalse(reachableSet, m_stateVars[i]);
    }

    return reachableSet; /* implicit conversion from BDD_ID to bool */
}

int Reachability::stateDistance(const std::vector<bool> &stateVector)
{
    if (stateVector.size() != m_stateVars.size())
        throw std::runtime_error(
            "stateDistance() vector argument size does not match the number of state variables.");

    BDD_ID reachableSet = m_initStateCharFunc;
    BDD_ID previousReachableSet;
    unsigned int stepCounter = 0;

    /* Check if argument is initial state. */
    bool isInitState = true;
    for (std::size_t i = 0; i < m_initState.size(); i++)
    {
        if (m_initState[i] != stateVector[i])
        {
            isInitState = false;
            break;
        }
    }

    if (isInitState)
        return 0;

    /* Similar to reachability computation, except we stop when the argument
     * state is reached. This requires checking all newly calculated images for
     * the presence of the argument state. */
    while (true)
    {
        previousReachableSet = reachableSet;
        stepCounter++;

        /* Compute image of current reachable set. */
        BDD_ID img = and2(reachableSet, m_transitionRelation);

        for (std::size_t i = 0; i < m_stateVars.size(); i++)
            img = existQuant(img, m_stateVars[i]);

        for (std::size_t i = 0; i < m_inputVars.size(); i++)
            img = existQuant(img, m_inputVars[i]);

        /* Because the characteristic function of the state set is a function of
         * s but the image is a function of s', we must rename s' to s in the
         * image function before adding it to the set. */

        for (std::size_t i = 0; i < m_stateVars.size(); i++)
            img = and2(img, xnor2(m_stateVars[i], m_nextStateVars[i]));

        for (std::size_t i = 0; i < m_nextStateVars.size(); i++)
            img = existQuant(img, m_nextStateVars[i]);

        /* Check if argument exists in new image. */
        BDD_ID existCheckSet = img;
        for (std::size_t i = 0; i < m_stateVars.size(); i++)
        {
            if (stateVector[i])
                existCheckSet = coFactorTrue(existCheckSet, m_stateVars[i]);
            else
                existCheckSet = coFactorFalse(existCheckSet, m_stateVars[i]);
        }

        if (existCheckSet)
            return stepCounter;

        reachableSet = or2(reachableSet, img);

        if (reachableSet == previousReachableSet)
            return -1;
    }
}

void Reachability::setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions)
{
    if (transitionFunctions.size() != m_stateVars.size())
        throw std::runtime_error("setTransitionFunctions() vector argument size does not match the "
                                 "number of state variables.");

    for (std::size_t i = 0; i < transitionFunctions.size(); i++)
    {
        if (transitionFunctions[i] >= this->unique_table_vector.size())
            throw std::runtime_error("setTransitionFunctions() vector argument contains a non-existent BDD_ID.");
    }


    m_transitionFunctions = transitionFunctions;
    computeTransitionRelation();
}

void Reachability::setInitState(const std::vector<bool> &stateVector)
{
    if (stateVector.size() != m_stateVars.size())
        throw std::runtime_error(
            "setInitState() vector argument size does not match the number of state variables.");

    m_initState = stateVector;
    computeInitStateCharFunction();
}

void Reachability::computeTransitionRelation()
{
    m_transitionRelation = TRUE_ID;

    /* tau(s,x,r) = Pi_i_from_0_to_nr_of_state_bits( r[i] XNOR delta(s[i],x[i]) ) */
    for (std::size_t i = 0; i < m_transitionFunctions.size(); i++)
    {
        const BDD_ID buf = xnor2(m_nextStateVars[i], m_transitionFunctions[i]);
        m_transitionRelation = and2(m_transitionRelation, buf);
    }
}

void Reachability::computeInitStateCharFunction()
{
    m_initStateCharFunc = TRUE_ID;

    for (std::size_t i = 0; i < m_initState.size(); i++)
    {
        /* m_initState values are implicitly converted to BDD_ID. */
        const BDD_ID buf = xnor2(m_stateVars[i], m_initState[i]);
        m_initStateCharFunc = and2(m_initStateCharFunc, buf);
    }
}

BDD_ID Reachability::existQuant(BDD_ID f, BDD_ID v)
{
    return or2(coFactorTrue(f, v), coFactorFalse(f, v));
}

} // namespace ClassProject
