#include "grafcet.hpp"

Grafcet::Grafcet(int numberOfStep)
{
    this->numberOfStep = numberOfStep;
    steps = (Grafstep**)malloc(numberOfStep * sizeof(Grafstep*));

    activeSteps.reserve(numberOfStep);
}

Grafcet::~Grafcet()
{
    for (int i = 0; i < numberOfStep; i++)
    {
        delete steps[i];
    }
    free(steps);
}

void Grafcet::loop()
{
    std::vector<int> toActivate, toDeactivate;
    toActivate.reserve(numberOfStep);
    toDeactivate.reserve(numberOfStep);
    for (int i = 0; i < activeSteps.size(); i++)
    {
        steps[activeSteps[i]]->loop();
        std::vector<int> transitionTo;
        steps[activeSteps[i]]->transitionTo(transitionTo, activeSteps);
        if (transitionTo.size() != 0)
        {
            toDeactivate.push_back(activeSteps[i]);
            for (int s = 0; s < transitionTo.size(); s++)
            {
                toActivate.push_back(transitionTo[s]);
            }
        }
    }
    for (int i = 0; i < toDeactivate.size(); i++)
    {
        deactivate(toDeactivate[i]);
    }
    for (int i = 0; i < toActivate.size(); i++)
    {
        activate(toActivate[i]);
    }
}

Grafstep* Grafcet::getStep(int i)
{
    return steps[i];
}

void Grafcet::setStep(int i, Grafstep* step)
{
    steps[i] = step;
}

void Grafcet::activate(int i)
{
    if (!inVector(i, activeSteps))
    {
        activeSteps.push_back(i);
        steps[i]->init();
    }
}

void Grafcet::deactivate(int i)
{
    std::vector<int>::iterator pos = std::find(activeSteps.begin(), activeSteps.end(), i);
    if (pos == activeSteps.end())
        return;
    steps[i]->end();
    activeSteps.erase(pos);
}