#ifndef READER_H
#define READER_H

#include <string>
#include <vector>
#include "data/node.h"
#include "data/edge.h"

class Reader
{
public:
    Reader(const std::string& filename);

    const std::vector<Node>& getNodes() const;

    const std::vector<Edge>& getEdges() const;

private:
    std::vector<Node> nodes;
    std::vector<Edge> edges;
};

#endif // READER_H
