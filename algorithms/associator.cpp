#include "associator.h"

Associator::Associator(const std::vector<Node>& nodes, const std::vector<Edge>& edges)
{
    //TODO
}

const std::vector<Step>& Associator::getSteps() const
{
    return steps;
}

const std::vector<Action>& Associator::getActions() const
{
    return actions;
}

const std::vector<Transition>& Associator::getTransitions() const
{
    return transitions;
}
