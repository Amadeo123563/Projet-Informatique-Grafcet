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

//The constructors should be written in the following format:
//StepX::StepX() : Grafstep(X) {
//    int* steps = (int*)malloc(Y * sizeof(int));
//    steps[0] = Z;
//      ...
//    setTransitions(Y, steps);
//}
//Where:
//    X is the step number,
//    Y is the number of transitions,
//    and Z is the next step number
//There should be as many next steps as there are transitions
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

//This function write the init(), loop() and end() functions
//Theses functions include specific code depending on the associated actions
//Currently, loop() and end() are always empty
void Coder::writeMainCode(CodeStep &code, const Step &step, const std::vector<Action> &actions)
{
    std::string initCode = "";
    std::string loopCode = "";
    std::string endCode = "";

    for(size_t a = 0; a < step.getAssociatedActions().size(); a++)
    {
        const std::string& action = actions[step.getAssociatedActions()[a]].getContent();

        //Send 1 to a Pin
        if (action.find("SET") == 0) {
            const std::string actionSubStr = action.substr(4);
            initCode += "    digitalWrite(" + actionSubStr + ", HIGH);\n";
            pinmodes[actionSubStr].setMode(PinMode::Output);
            continue;
        }

        //Send 0 to a Pin
        if (action.find("RESET") == 0) {
            const std::string actionSubStr = action.substr(6);
            initCode += "    digitalWrite(" + actionSubStr + ", LOW);\n";
            pinmodes[actionSubStr].setMode(PinMode::Output);
            continue;
        }

        //Assign a value to a variable. The right side of the assignment can be a variable, a number or any valid C++ set of operations between variables and numbers
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

//The shouldTransitionTo function should be written in the following format:
//bool StepX::shouldTransitionTo(int step, const std::vector<int>& activeSteps) const {
//    switch(step) {
//    case Y:
//        return (S) && (C);
//    }
//    return true;
//}
//Where:
//    X is the step number,
//    Y is the next step number,
//    S checks that all steps prior to the transition are active (S is not written if there is only one previous step),
//    C is the specific condition for the transition
void Coder::writeShouldTransition(CodeStep &code, int stepIndex, const std::vector<Step> &steps, const std::vector<Transition> &transitions)
{
    std::string transitionCode = "    switch(step) {\n";
    for(size_t t = 0; t < steps[stepIndex].getNextTransitions().size(); t++)
    {
        //One transition can lead to multiple steps, so we put one case for each steps
        //Reminder that the shouldTransitionTo() function is called once for every possible next step even if a transition is taken at some point before the last one get checked
        for(size_t s = 0; s < transitions[steps[stepIndex].getNextTransitions()[t]].getNextSteps().size(); s++)
        {
            transitionCode += "    case " + std::to_string(transitions[steps[stepIndex].getNextTransitions()[t]].getNextSteps()[s]) + ":\n";
        }
        std::string condition = "(";
        std::vector<int> previousSteps = getPreviousSteps(steps[stepIndex].getNextTransitions()[t], steps);
        //If there are more than one previous step, we need to check that all of them are active
        if(previousSteps.size() > 1)
        {
            for(size_t p = 0; p < previousSteps.size(); p++)
            {
                if(previousSteps[p] != stepIndex) //Don't check the current step as it must be active
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

//This function adds the "globalVars->" preffix to all variables in the input string
//It also adds the variable to the list of global variables if it is not already in it
std::string Coder::addGlobalVarPreffixes(const std::string &input)
{
    std::string output = "";
    std::string foundVariableBuffer;
    bool mayAddPrefix = true; //true when the next character can be the start of a variable
    bool readingVariable = false;
    for(size_t c = 0; c < input.size(); c++)
    {
        if(mayAddPrefix) {
            if(IS_LETTER(input[c])) //If the next character is a letter, it is the start of a variable
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
            if(!IS_LETTER(input[c]) && !IS_NUMBER(input[c])) //We reached the end of a variable or a number
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
    for(size_t v = 0; v < globalVariables.size(); v++) //Check if the variable is already in the list
    {
        if(globalVariables[v] == name)
            return;
    }
    globalVariables.push_back(name);
}

//This function returns the list of steps that are previous to a transition
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
    //Separate the string condition to a list of TransitionConditionPart
    while(c < code.size())
    {
        //Start by checking if we have a AND separator, OR separator or NOT
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
        else //Add the current character to the current part
        {
            conditionList[conditionList.size() - 1].part += code[c];
            c++;
        }
    }
    //The condition can now be translated to C++
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

//This function translates a condition part to C++, the part does not contain any separator (AND, OR) or NOT
std::string Coder::generateSpecificCondition(const std::string &part)
{
    //Check if we received 1 from a pin
    const size_t pressedPos = part.find("PRESSED?");
    if(pressedPos != std::string::npos)
    {
        const std::string pin = trimWhitespaces(part.substr(0, pressedPos));
        pinmodes[pin].setMode(PinMode::Input_Pulldown);
        return "digitalRead(" + pin + ") == HIGH";
    }

    //Check if we received 0 from a pin
    const size_t releasedPos = part.find("RELEASED?");
    if(releasedPos != std::string::npos)
    {
        const std::string pin = trimWhitespaces(part.substr(0, releasedPos));
        pinmodes[pin].setMode(PinMode::Input_Pulldown);
        return "digitalRead(" + pin + ") == LOW";
    }

    //Check if the state has been active for a certain amount of time
    const size_t waitPos = part.find("WAIT ");
    if(waitPos != std::string::npos)
    {
        const std::string time = trimWhitespaces(part.substr(waitPos+5, part.size() - waitPos-5));
        return "timeSinceActivation() >= " + strToDuration(time);
    }

    //Check if a variable is superior, inferior, equal, not equal to a number
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

//The outputed string is in milliseconds but can include a multiplication sign, it should be directly sent in the Arduino C++ code output
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
