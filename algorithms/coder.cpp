#include "coder.h"

Coder::Coder(const std::vector<Step>& steps, const std::vector<Action>& actions, const std::vector<Transition>& transitions)
{
    codeSteps.reserve(steps.size());

    //TODO
}

const std::vector<CodeStep>& Coder::getCodeSteps() const
{
    return codeSteps;
}
