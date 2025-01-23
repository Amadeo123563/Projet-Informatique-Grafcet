#ifndef ASSOCIATOR_H
#define ASSOCIATOR_H

#include <vector>
#include "../data/node.h"
#include "../data/edge.h"
#include "../data/step.h"
#include "../data/action.h"
#include "../data/transition.h"

class Associator
{
public:
    Associator(const std::vector<Node>& nodes, const std::vector<Edge>& edges);

    const std::vector<Step>& getSteps() const;
    const std::vector<Action>& getActions() const;
    const std::vector<Transition>& getTransitions() const;

private:
    std::vector<Step> steps;
    std::vector<Action> actions;
    std::vector<Transition> transitions;
};

#endif // ASSOCIATOR_H
