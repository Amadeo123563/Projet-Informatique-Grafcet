#include "step.h"

Step::Step()
{

}

void Step::addAction(int index)
{
    associatedActions.push_back(index);
}

void Step::addTransition(int index)
{
    nextTransitions.push_back(index);
}

const std::vector<int>& Step::getAssociatedActions() const
{
    return associatedActions;
}

const std::vector<int>& Step::getNextTransitions() const
{
    return nextTransitions;
}

const std::string& Step::getContent() const
{
    return content;
}

void Step::setContent(const std::string &value)
{
    content = value;
}

bool Step::getInitialStep() const
{
    return initialStep;
}

void Step::setInitialStep(bool value)
{
    initialStep = value;
}
