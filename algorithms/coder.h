#ifndef CODER_H
#define CODER_H

#include <vector>
#include "data/step.h"
#include "data/action.h"
#include "data/transition.h"
#include "data/codestep.h"

class Coder
{
public:
    Coder(const std::vector<Step>& steps, const std::vector<Action>& actions, const std::vector<Transition>& transitions);

    const std::vector<CodeStep>& getCodeSteps() const;

private:
    std::vector<CodeStep> codeSteps;
};

#endif // CODER_H
