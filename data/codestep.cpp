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
    return "Step" + num + "::Step" + num + "() : Grafstep(" + num + ") {" +
            constructorCode +
            "}\nvoid Step" + num + "::init() {" +
            initCode +
            "}\nvoid Step" + num + "::loop() {" +
            loopCode +
            "}\nvoid Step" + num + "::end() {" +
            endCode +
            "}\nbool Step" + num + "::shouldTransitionTo(int step, const std::vector<int>& activeSteps) const {" +
            transitionCode +
            "}\n\n";
}

bool CodeStep::getIsInitial() const
{
    return isInitial;
}

void CodeStep::setIsInitial(bool value)
{
    isInitial = value;
}
