#include "transition.h"

Transition::Transition()
{

}

void Transition::addStep(int index)
{
    nextSteps.push_back(index);
}

const std::vector<int> &Transition::getNextSteps() const
{
    return nextSteps;
}

const std::string &Transition::getContent() const
{
    return content;
}

void Transition::setContent(const std::string &value)
{
    content = value;
}
