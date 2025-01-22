#ifndef GRAFCET_HPP
#define GRAFCET_HPP

#include "grafstep.hpp"

class Grafcet
{
public:
    Grafcet(int numberOfStep);
    ~Grafcet();

    virtual void loop();

    Grafstep* getStep(int i);
    void setStep(int i, Grafstep* step);

    void activate(int i);
    void deactivate(int i);

private:
    int numberOfStep;
    Grafstep** steps;

    std::vector<int> activeSteps;
};


#endif