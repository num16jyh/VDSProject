#include "Reachability.h"

namespace ClassProject {

Reachability::Reachability(int i)
: ReachabilityInterface(0,0)
{}

const std::vector<BDD_ID> &Reachability::getStates() const
{}

const std::vector<BDD_ID> &Reachability::getInputs() const
{}

bool Reachability::isReachable(const std::vector<bool> &stateVector)
{}

int Reachability::stateDistance(const std::vector<bool> &stateVector)
{}

void Reachability::setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions)
{}

void Reachability::setInitState(const std::vector<bool> &stateVector)
{}

} // namespace ClassProject
