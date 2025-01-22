#include <iostream>

#include "algorithms/reader.h"
#include "algorithms/associator.h"
#include "algorithms/coder.h"
#include "algorithms/writer.h"

int main(int argc, char* argv[])
{
    if (argc <= 2) {
        std::cout << "Not enough arguments provided." << std::endl;
        return 1;
    }
    std::string file = argv[1];
    std::string outputFolder = argv[2];

    Reader reader(file);
    Associator associator(reader.getNodes(), reader.getEdges());
    Coder coder(associator.getSteps(), associator.getActions(), associator.getTransitions());
    Writer writer(coder.getCodeSteps(), coder.getPinmodes(), outputFolder);

    return 0;
}
