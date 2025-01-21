#ifndef STEP_H
#define STEP_H

#include <vector>
#include <string>

class Step
{
public:
    Step();

    void addAction(int index);
    void addTransition(int index);

    const std::vector<int>& getAssociatedActions() const;
    const std::vector<int>& getNextTransitions() const;

    const std::string& getContent() const;
    void setContent(const std::string &value);

    bool getInitialStep() const;
    void setInitialStep(bool value);

private:
    std::vector<int> associatedActions;
    std::vector<int> nextTransitions;
    std::string content;
    bool initialStep;
};

#endif // STEP_H
