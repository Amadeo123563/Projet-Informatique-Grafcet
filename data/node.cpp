#include "node.h"

Node::Node()
{

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
    if(value == "diamond") {
        type = Transition;
    }
    if(value == "ellipse") {
        type = Step;
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
