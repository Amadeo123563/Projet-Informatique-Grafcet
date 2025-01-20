#ifndef CODESTEP_H
#define CODESTEP_H

#include <string>

class CodeStep
{
public:
    CodeStep();

    void setConstructorCode(const std::string &value);
    void setInitCode(const std::string &value);
    void setLoopCode(const std::string &value);
    void setEndCode(const std::string &value);
    void setTransitionCode(const std::string &value);

    std::string getCode() const;

private:
    std::string constructorCode;
    std::string initCode;
    std::string loopCode;
    std::string endCode;
    std::string transitionCode;
};

#endif // CODESTEP_H
