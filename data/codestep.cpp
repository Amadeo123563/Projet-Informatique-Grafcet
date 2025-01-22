#include "codestep.h"

CodeStep::CodeStep()
{

}

void CodeStep::setConstructorCode(const std::string &value)
{
    constructorCode = value;
}

void CodeStep::setInitCode(const std::string &value)
{
    initCode = value;
}

void CodeStep::setLoopCode(const std::string &value)
{
    loopCode = value;
}

void CodeStep::setEndCode(const std::string &value)
{
    endCode = value;
}

void CodeStep::setTransitionCode(const std::string &value)
{
    transitionCode = value;
}

std::string CodeStep::getCode(int stepNumber) const
{
    const std::string num = std::to_string(stepNumber);
    return "Step" + num + "::Step" + num + "() : Grafstep(" + num + ") {\n" +
            constructorCode +
            "\n}\n\nvoid Step" + num + "::init() {\n    Grafstep::init();\n" +
            initCode +
            "\n}\n\nvoid Step" + num + "::loop() {\n" +
            loopCode +
            "\n}\n\nvoid Step" + num + "::end() {\n" +
            endCode +
            "\n}\n\nbool Step" + num + "::shouldTransitionTo(int step, const std::vector<int>& activeSteps) const {\n" +
            transitionCode +
            "\n}\n\n\n";
}

bool CodeStep::getIsInitial() const
{
    return isInitial;
}

void CodeStep::setIsInitial(bool value)
{
    isInitial = value;
}
