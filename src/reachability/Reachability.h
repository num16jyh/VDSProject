#ifndef VDSPROJECT_REACHABILITY_H
#define VDSPROJECT_REACHABILITY_H

#include "ReachabilityInterface.h"

namespace ClassProject {

class Reachability : public ReachabilityInterface
{
public:
    explicit Reachability(unsigned int stateSize, unsigned int inputSize = 0);

    const std::vector<BDD_ID> &getStates() const override;

    const std::vector<BDD_ID> &getInputs() const override;

    bool isReachable(const std::vector<bool> &) override;

    int stateDistance(const std::vector<bool> &) override;

    void setTransitionFunctions(const std::vector<BDD_ID> &) override;

    void setInitState(const std::vector<bool> &) override;

private:
    std::vector<BDD_ID> m_stateVars;
    std::vector<BDD_ID> m_nextStateVars;
    std::vector<BDD_ID> m_inputVars;
    std::vector<BDD_ID> m_transitionFunctions;
    std::vector<bool> m_initState;
    BDD_ID m_transitionRelation;
    BDD_ID m_initStateCharFunc;

    void computeTransitionRelation();
    void computeInitStateCharFunction();

    BDD_ID existQuant(BDD_ID, BDD_ID);
};

} // namespace ClassProject
#endif
