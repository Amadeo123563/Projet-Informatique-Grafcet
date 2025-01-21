#include "node.h"

Node::Node()
{
    id = -1;
    type = Invalid;
}

int Node::getId() const
{
    return id;
}

void Node::setId(int value)
{
    id = value;
}

Node::Type Node::getType() const
{
    return type;
}

void Node::setType(const Type &value)
{
    type = value;
}

void Node::setType(const std::string &value)
{
    if(value == "rectangle") {
        type = Action;
    }
    else if(value == "diamond") {
        type = Transition;
    }
    else if(value == "ellipse") {
        type = Step;
    }
    else if(value == "octagon") {
        type = InitialStep;
    }
    else {
        type = Invalid;
    }
}

std::string Node::getText() const
{
    return text;
}

void Node::setText(const std::string &value)
{
    text = value;
}

bool Node::isValid()
{
    return (id >= 0) && (type != Invalid);
}
