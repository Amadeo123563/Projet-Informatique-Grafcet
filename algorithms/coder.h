#ifndef CODER_H
#define CODER_H

#include <vector>
#include <unordered_map>
#include <algorithm>
#include "data/step.h"
#include "data/action.h"
#include "data/transition.h"
#include "data/codestep.h"
#include "data/pinmode.h"

#include <iostream>

#define IS_LETTER(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#define IS_NUMBER(c) (c >= '0' && c <= '9')
#define WHITESPACE " \n\r\t\f\v"

typedef struct transitionConditionPart  {
    enum ConditionSeparator {
        Null,
        And,
        Or
    };

    std::string part;
    ConditionSeparator separator;
    bool inverted;
} TransitionConditionPart;

class Coder
{
public:
    Coder(const std::vector<Step>& steps, const std::vector<Action>& actions, const std::vector<Transition>& transitions);

    const std::vector<CodeStep>& getCodeSteps() const;

    const std::unordered_map<std::string, PinMode>& getPinmodes() const;

    const std::vector<std::string>& getGlobalVariables() const;

private:
    std::vector<CodeStep> codeSteps;
    std::unordered_map<std::string, PinMode> pinmodes;
    std::vector<std::string> globalVariables;

    void writeConstructor(CodeStep& code, int stepIndex, const std::vector<Step>& steps, const std::vector<Transition>& transitions);
    void writeMainCode(CodeStep& code, const Step& step, const std::vector<Action>& actions);
    void writeShouldTransition(CodeStep& code, int stepIndex, const std::vector<Step>& steps, const std::vector<Transition>& transitions);

    std::string addGlobalVarPreffixes(const std::string& input);
    void addVariable(const std::string& name);

    std::vector<int> getPreviousSteps(int transitionIndex, const std::vector<Step>& steps);
    std::string generateSpecificTransitionCode(const std::string& code);
    std::string generateSpecificCondition(const std::string& part);

    std::string trimWhitespaces(const std::string& str);
    std::string strToDuration(const std::string& str);
};

#endif // CODER_H
