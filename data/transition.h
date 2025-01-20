#ifndef TRANSITION_H
#define TRANSITION_H

#include <vector>
#include <string>

class Transition
{
public:
    Transition();

    void addStep(int index);

    const std::vector<int>& getNextSteps() const;

    const std::string& getContent() const;
    void setContent(const std::string &value);

private:
    std::vector<int> nextSteps;
    std::string content;
};

#endif // TRANSITION_H
