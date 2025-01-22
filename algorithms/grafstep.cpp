#include "grafstep.hpp"

Grafstep::Grafstep(int num)
{
    this->num = num;
}

Grafstep::~Grafstep()
{
    free(nextSteps);
}

void Grafstep::setTransitions(int numberOfNextStep, int* nextSteps)
{
    this->numberOfNextStep = numberOfNextStep;
    this->nextSteps = nextSteps;
}

void Grafstep::transitionTo(std::vector<int>& transitions, const std::vector<int>& activeSteps) const
{
    transitions.reserve(numberOfNextStep);
    for (int i = 0; i < numberOfNextStep; i++)
    {
        if (shouldTransitionTo(nextSteps[i], activeSteps))
        {
            transitions.push_back(nextSteps[i]);
        }
    }
}

void Grafstep::init()
{

}

void Grafstep::loop()
{

}

void Grafstep::end()
{

}

bool Grafstep::shouldTransitionTo(int step, const std::vector<int>& activeSteps) const
{
    return true;
}