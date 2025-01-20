#include <iostream>

#include "algorithms/reader.h"
#include "algorithms/associator.h"
#include "algorithms/coder.h"
#include "algorithms/writer.h"

int main(int argc, char* argv[])
{
    if (argc <= 1) {
        std::cout << "No file provided." << std::endl;
        return 1;
    }
    std::string file = argv[1];

    Reader reader(file);
    Associator associator(reader.getNodes(), reader.getEdges());
    Coder coder(associator.getSteps(), associator.getActions(), associator.getTransitions());
    Writer writer(coder.getCodeSteps());

    return 0;
}
