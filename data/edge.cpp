#include "edge.h"

Edge::Edge()
{

}

int Edge::getSource() const
{
    return source;
}

void Edge::setSource(int value)
{
    source = value;
}

int Edge::getDestination() const
{
    return destination;
}

void Edge::setDestination(int value)
{
    destination = value;
}
