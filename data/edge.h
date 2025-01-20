#ifndef EDGE_H
#define EDGE_H


class Edge
{
public:
    Edge();

    int getSource() const;
    void setSource(int value);

    int getDestination() const;
    void setDestination(int value);

private:
    int source;
    int destination;
};

#endif // READER_H
