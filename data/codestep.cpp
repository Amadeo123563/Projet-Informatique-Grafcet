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
    //TODO: Add constant strings
    return constructorCode + initCode + loopCode + endCode + transitionCode;
}

bool CodeStep::getIsInitial() const
{
    return isInitial;
}

void CodeStep::setIsInitial(bool value)
{
    isInitial = value;
}
