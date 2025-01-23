#ifndef UNITTESTS_H
#define UNITTESTS_H

#include <iostream>

#include "algorithms/reader.h"
#include "algorithms/associator.h"
#include "algorithms/coder.h"
#include "algorithms/writer.h"

namespace UnitTests {
    void testReader();
    void testAssociator();
    void testCoder();
    void testWriter();
}

#endif // UNITTESTS_H