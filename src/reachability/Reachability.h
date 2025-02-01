#ifndef VDSPROJECT_REACHABILITY_H
#define VDSPROJECT_REACHABILITY_H

#include "ReachabilityInterface.h"

namespace ClassProject {

class Reachability : public ReachabilityInterface
{
public:
    explicit Reachability(int i);

    const std::vector<BDD_ID> &getStates() const override;

    const std::vector<BDD_ID> &getInputs() const override;

    bool isReachable(const std::vector<bool> &stateVector) override;

    int stateDistance(const std::vector<bool> &stateVector) override;

    void setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions) override;

    void setInitState(const std::vector<bool> &stateVector) override;
};

} // namespace ClassProject
#endif
