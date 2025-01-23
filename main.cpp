#include <iostream>

#include "algorithms/reader.h"
#include "algorithms/associator.h"
#include "algorithms/coder.h"
#include "algorithms/writer.h"

#define UNITTESTING true

#if UNITTESTING
#include "unittests.h"
#endif

int main(int argc, char* argv[])
{
    #if UNITTESTING
    std::cout << "Begin unittesting." << std::endl;
    UnitTests::testReader();
    UnitTests::testAssociator();
    UnitTests::testCoder();
    UnitTests::testWriter();
    std::cout << "Unit tests complete." << std::endl;
    #else
    if (argc <= 2) {
        std::cout << "Not enough arguments provided." << std::endl;
        return 1;
    }
    std::string file = argv[1];
    std::string outputFolder = argv[2];

    Reader reader(file);
    Associator associator(reader.getNodes(), reader.getEdges());
    Coder coder(associator.getSteps(), associator.getActions(), associator.getTransitions());
    Writer writer(coder.getCodeSteps(), coder.getGlobalVariables(), coder.getPinmodes(), outputFolder);
    #endif

    return 0;
}
