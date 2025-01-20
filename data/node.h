#ifndef NODE_H
#define NODE_H

#include "string"

class Node
{
public:
    enum Type {
        Step,
        Action,
        Transition
    };

    Node();

    int getId() const;
    void setId(int value);

    Type getType() const;
    void setType(const Type &value);
    void setType(const std::string &value);

    std::string getText() const;
    void setText(const std::string &value);

private:
    int id;
    Type type;
    std::string text;
};

#endif // NODE_H
