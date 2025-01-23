#include "coder.h"

Coder::Coder(const std::vector<Step>& steps, const std::vector<Action>& actions, const std::vector<Transition>& transitions)
{
    codeSteps.reserve(steps.size());

    for(size_t s = 0; s < steps.size(); s++)
    {
        CodeStep code;

        code.setIsInitial(steps[s].getInitialStep());
        writeConstructor(code, s, steps, transitions);
        writeMainCode(code, steps[s], actions);
        writeShouldTransition(code, s, steps, transitions);

        codeSteps.push_back(code);
    }
}

const std::vector<CodeStep>& Coder::getCodeSteps() const
{
    return codeSteps;
}

const std::unordered_map<std::string, PinMode>& Coder::getPinmodes() const
{
    return pinmodes;
}

const std::vector<std::string>& Coder::getGlobalVariables() const
{
    return globalVariables;
}

void Coder::writeConstructor(CodeStep &code, int stepIndex, const std::vector<Step> &steps, const std::vector<Transition> &transitions)
{
    int stepCount = 0;
    std::vector<int> nextSteps;
    nextSteps.reserve(steps.size());
    for(size_t t = 0; t < steps[stepIndex].getNextTransitions().size(); t++)
    {
        stepCount += transitions[steps[stepIndex].getNextTransitions()[t]].getNextSteps().size();
        for(size_t s = 0; s < transitions[steps[stepIndex].getNextTransitions()[t]].getNextSteps().size(); s++)
        {
            nextSteps.push_back(transitions[steps[stepIndex].getNextTransitions()[t]].getNextSteps()[s]);
        }
    }
    const std::string countStr = std::to_string(stepCount);
    std::string constructor = "    int* steps = (int*)malloc(" + countStr + " * sizeof(int));\n";
    for(size_t s = 0; s < nextSteps.size(); s++)
    {
        constructor += "    steps[" + std::to_string(s) + "] = " + std::to_string(nextSteps[s]) + ";\n";
    }
    constructor += "    setTransitions(" + countStr + ", steps);\n";
    code.setConstructorCode(constructor);
}

void Coder::writeMainCode(CodeStep &code, const Step &step, const std::vector<Action> &actions)
{
    std::string initCode = "";
    std::string loopCode = "";
    std::string endCode = "";

    for(size_t a = 0; a < step.getAssociatedActions().size(); a++)
    {
        const std::string& action = actions[step.getAssociatedActions()[a]].getContent();

        if (action.find("SET") == 0) {
            const std::string actionSubStr = action.substr(4);
            initCode += "    digitalWrite(" + actionSubStr + ", HIGH);\n";
            pinmodes[actionSubStr].setMode(PinMode::Output);
            continue;
        }

        if (action.find("RESET") == 0) {
            const std::string actionSubStr = action.substr(6);
            initCode += "    digitalWrite(" + actionSubStr + ", LOW);\n";
            pinmodes[actionSubStr].setMode(PinMode::Output);
            continue;
        }

        const size_t equalPos = action.find(":=");
        if (equalPos != std::string::npos) {
            initCode += "    " + addGlobalVarPreffixes(action.substr(0, equalPos - 1)) + " = " + addGlobalVarPreffixes(action.substr(equalPos + 3)) + ";\n";
            continue;
        }

        std::cout << "Unknown action: " << action << std::endl;;
    }

    code.setInitCode(initCode);
    code.setLoopCode(loopCode);
    code.setEndCode(endCode);
}

void Coder::writeShouldTransition(CodeStep &code, int stepIndex, const std::vector<Step> &steps, const std::vector<Transition> &transitions)
{
    std::string transitionCode = "    switch(step) {\n";
    for(size_t t = 0; t < steps[stepIndex].getNextTransitions().size(); t++)
    {
        for(size_t s = 0; s < transitions[steps[stepIndex].getNextTransitions()[t]].getNextSteps().size(); s++)
        {
            transitionCode += "    case " + std::to_string(transitions[steps[stepIndex].getNextTransitions()[t]].getNextSteps()[s]) + ":\n";
        }
        std::string condition = "(";
        std::vector<int> previousSteps = getPreviousSteps(steps[stepIndex].getNextTransitions()[t], steps);
        if(previousSteps.size() > 1)
        {
            for(size_t p = 0; p < previousSteps.size(); p++)
            {
                if(previousSteps[p] != stepIndex)
                    condition += "inVector(" + std::to_string(previousSteps[p]) + ", activeSteps) && ";
            }
            condition = condition.substr(0, condition.size() - 4) + ") && (";
        }
        condition += generateSpecificTransitionCode(transitions[steps[stepIndex].getNextTransitions()[t]].getContent());
        transitionCode += "        return " + condition + ");\n";
    }
    transitionCode += "    }\n    return true;";
    code.setTransitionCode(transitionCode);
}

std::string Coder::addGlobalVarPreffixes(const std::string &input)
{
    std::string output = "";
    std::string foundVariableBuffer;
    bool mayAddPrefix = true;
    bool readingVariable = false;
    for(size_t c = 0; c < input.size(); c++)
    {
        if(mayAddPrefix) {
            if(IS_LETTER(input[c]))
            {
                output += "globalVars->";
                mayAddPrefix = false;
                foundVariableBuffer = input[c];
                readingVariable = true;
            }
            else if(IS_NUMBER(input[c]))
            {
                mayAddPrefix = false;
            }
        } else {
            if(!IS_LETTER(input[c]) && !IS_NUMBER(input[c]))
            {
                mayAddPrefix = true;
                addVariable(foundVariableBuffer);
            }
            else if(readingVariable)
            {
                foundVariableBuffer += input[c];
            }
        }
        output += input[c];
    }
    addVariable(foundVariableBuffer);
    return output;
}

void Coder::addVariable(const std::string &name)
{
    if(name.size() < 1)
        return;
    for(size_t v = 0; v < globalVariables.size(); v++)
    {
        if(globalVariables[v] == name)
            return;
    }
    globalVariables.push_back(name);
}

std::vector<int> Coder::getPreviousSteps(int transitionIndex, const std::vector<Step> &steps)
{
    std::vector<int> previousSteps;
    previousSteps.reserve(steps.size());

    for(size_t s = 0; s < steps.size(); s++)
    {
        if(std::find(steps[s].getNextTransitions().begin(), steps[s].getNextTransitions().end(), transitionIndex) != steps[s].getNextTransitions().end())
        {
            previousSteps.push_back(s);
        }
    }

    return previousSteps;
}

std::string Coder::generateSpecificTransitionCode(const std::string &code)
{
    if(code == "")
        return "true";
    std::vector<TransitionConditionPart> conditionList;
    conditionList.push_back({"", TransitionConditionPart::Null, false});
    size_t c = 0;
    while(c < code.size())
    {
        if(c+2 < code.size() && code[c] == 'A' && code[c+1] == 'N' && code[c+2] == 'D')
        {
            conditionList[conditionList.size() - 1].separator = TransitionConditionPart::And;
            conditionList.push_back({"", TransitionConditionPart::Null, false});
            c += 3;
        }
        else if(c+1 < code.size() && code[c] == 'O' && code[c+1] == 'R')
        {
            conditionList[conditionList.size() - 1].separator = TransitionConditionPart::Or;
            conditionList.push_back({"", TransitionConditionPart::Null, false});
            c += 2;
        }
        else if(c+2 < code.size() && code[c] == 'N' && code[c+1] == 'O' && code[c+2] == 'T')
        {
            conditionList[conditionList.size() - 1].inverted = !conditionList[conditionList.size() - 1].inverted;
            c += 3;
        }
        else
        {
            conditionList[conditionList.size() - 1].part += code[c];
            c++;
        }
    }
    std::string condition = "";
    for(size_t p = 0; p < conditionList.size(); p++)
    {
        if(conditionList[p].inverted)
            condition += "!(";

        condition += generateSpecificCondition(conditionList[p].part);

        if(conditionList[p].inverted)
            condition += ')';

        switch (conditionList[p].separator) {
        case TransitionConditionPart::And:
            condition += " && ";
            break;
        case TransitionConditionPart::Or:
            condition += " || ";
            break;
        }
    }
    return condition;
}

std::string Coder::generateSpecificCondition(const std::string &part)
{
    const size_t pressedPos = part.find("PRESSED?");
    if(pressedPos != std::string::npos)
    {
        const std::string pin = trimWhitespaces(part.substr(0, pressedPos));
        pinmodes[pin].setMode(PinMode::Input_Pulldown);
        return "digitalRead(" + pin + ") == HIGH";
    }

    const size_t releasedPos = part.find("RELEASED?");
    if(releasedPos != std::string::npos)
    {
        const std::string pin = trimWhitespaces(part.substr(0, releasedPos));
        pinmodes[pin].setMode(PinMode::Input_Pulldown);
        return "digitalRead(" + pin + ") == LOW";
    }

    const size_t waitPos = part.find("WAIT ");
    if(waitPos != std::string::npos)
    {
        const std::string time = trimWhitespaces(part.substr(waitPos+5, part.size() - waitPos-5));
        return "timeSinceActivation() >= " + strToDuration(time);
    }

    const size_t inferiorEqPos = part.find("&lt;=");
    if (inferiorEqPos != std::string::npos) {
        return addGlobalVarPreffixes(part.substr(0, inferiorEqPos - 1)) + " <= " + addGlobalVarPreffixes(part.substr(inferiorEqPos + 6));
    }
    const size_t superiorEqPos = part.find("&gt;=");
    if (superiorEqPos != std::string::npos) {
        return addGlobalVarPreffixes(part.substr(0, superiorEqPos - 1)) + " >= " + addGlobalVarPreffixes(part.substr(superiorEqPos + 6));
    }
    const size_t equalsPos = part.find("=");
    if (equalsPos != std::string::npos) {
        return addGlobalVarPreffixes(part.substr(0, equalsPos - 1)) + " == " + addGlobalVarPreffixes(part.substr(equalsPos + 2));
    }
    const size_t notEqualsPos = part.find("!=");
    if (notEqualsPos != std::string::npos) {
        return addGlobalVarPreffixes(part.substr(0, notEqualsPos - 1)) + " != " + addGlobalVarPreffixes(part.substr(notEqualsPos + 3));
    }
    const size_t inferiorPos = part.find("&lt;");
    if (inferiorPos != std::string::npos) {
        return addGlobalVarPreffixes(part.substr(0, inferiorPos - 1)) + " < " + addGlobalVarPreffixes(part.substr(inferiorPos + 5));
    }
    const size_t superiorPos = part.find("&gt;");
    if (superiorPos != std::string::npos) {
        return addGlobalVarPreffixes(part.substr(0, superiorPos - 1)) + " > " + addGlobalVarPreffixes(part.substr(superiorPos + 5));
    }

    std::cout << "Unknown transition part: " << part << std::endl;

    return "true";
}

std::string Coder::trimWhitespaces(const std::string &str)
{
    size_t start = str.find_first_not_of(WHITESPACE);
    std::string trimmed = (start == std::string::npos) ? "" : str.substr(start);

    size_t end = trimmed.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : trimmed.substr(0, end + 1);
}

std::string Coder::strToDuration(const std::string &str)
{
    if(str[str.size() - 2] == 'm' && str[str.size() - 1] == 's')
        return str.substr(0, str.size() - 2);
    if(str[str.size() - 1] == 's')
        return str.substr(0, str.size() - 1) + "*1000";
    if(str[str.size() - 3] == 'm' && str[str.size() - 2] == 'i' && str[str.size() - 1] == 'n')
        return str.substr(0, str.size() - 3) + "*60000";
    if(str[str.size() - 1] == 'h')
        return str.substr(0, str.size() - 1) + "*3600000";
    return str + "*1000";
}
