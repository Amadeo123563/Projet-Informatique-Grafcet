#ifndef GRAFSTEP_HPP
#define GRAFSTEP_HPP

#include <malloc.h>
#include <vector>
#include <algorithm>

#define inVector(S, V) (std::find(V.begin(), V.end(), S) != V.end())

class Grafstep
{
public:
    Grafstep(int num);
    ~Grafstep();

    void setTransitions(int numberOfNextStep, int* nextSteps);
    void transitionTo(std::vector<int>& transitions, const std::vector<int>& activeSteps) const;

    virtual void init();
    virtual void loop();
    virtual void end();
    virtual bool shouldTransitionTo(int step, const std::vector<int>& activeSteps) const;

private:
    int num;
    int numberOfNextStep;
    int* nextSteps;
};


#endif