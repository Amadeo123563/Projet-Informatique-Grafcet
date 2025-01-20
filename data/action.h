#ifndef ACTION_H
#define ACTION_H

#include <string>

class Action
{
public:
    Action();

    const std::string& getContent() const;
    void setContent(const std::string &value);

private:
    std::string content;
};

#endif // ACTION_H
