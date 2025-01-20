#include "action.h"

Action::Action()
{

}

const std::string& Action::getContent() const
{
    return content;
}

void Action::setContent(const std::string &value)
{
    content = value;
}
