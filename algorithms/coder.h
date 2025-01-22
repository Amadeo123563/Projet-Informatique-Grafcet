#ifndef CODER_H
#define CODER_H
 
#include <vector>
#include <unordered_map>
#include "step.h"
#include "action.h"
#include "transition.h"
#include "codestep.h"
#include "pinmode.h"
 
class Coder
{
public:
    Coder(const std::vector<Step>& steps, const std::vector<Action>& actions, const std::vector<Transition>& transitions);
 
    const std::vector<CodeStep>& getCodeSteps() const;
 
    const std::unordered_map<int, PinMode>& getPinmodes() const;
 
    const std::vector<std::string>& getGlobalVariables() const;
 
    bool Coder::transitionClassique(const std::string& condition) const;
 
    bool Coder::transitionChronometree(unsigned long& timerStart, unsigned long delayMs) const;
 
    bool Coder::transitionForcee(int emergencyPin) const;
 
    bool Coder::transitionAnalogique(const std::string& analogCondition) const;
 
    bool Coder::transitionCombinee(const std::string& conditions) const;
 
    bool Coder::inVector(int step, const std::vector<int>& activeSteps) const;
 
    bool Coder::shouldTransitionTo(int step, const std::vector<int>& activeSteps) const;
 
private:
    std::vector<CodeStep> codeSteps;
    std::unordered_map<int, PinMode> pinmodes;
    std::vector<std::string> globalVariables;
};
 
#endif // CODER_H
