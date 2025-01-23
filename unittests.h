#ifndef UNITTESTS_H
#define UNITTESTS_H

#include <iostream>

#include "algorithms/reader.h"
#include "algorithms/associator.h"
#include "algorithms/coder.h"
#include "algorithms/writer.h"
#include "data/action.h"
#include "data/codestep.h"
#include "data/edge.h"
#include "data/node.h"
#include "data/pinmode.h"
#include "data/step.h"
#include "data/transition.h"

namespace UnitTests {
    void assert(bool condition, const std::string& name);
    void assertFiles(const std::string& path1, const std::string& path2, const std::string& name);

    void testReader();
    void testAssociator();
    void testCoder();
    void testWriter();

    extern int success;
    extern int failures;
}

#endif // UNITTESTS_H
