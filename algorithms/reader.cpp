#include "reader.h"

Reader::Reader(const std::string& filename)
{
    //TODO: Read file
}

const std::vector<Node>& Reader::getNodes() const
{
    return nodes;
}

const std::vector<Edge>& Reader::getEdges() const
{
    return edges;
}
