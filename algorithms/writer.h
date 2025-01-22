#ifndef WRITER_H
#define WRITER_H

#include <vector>
#include <unordered_map>
#include "data/codestep.h"
#include "data/pinmode.h"

class Writer
{
public:
    Writer(const std::vector<CodeStep>& steps, const std::vector<std::string>& globalVariables, const std::unordered_map<int, PinMode>& pinModes, std::string outputfolder);
private:
    
};

#endif // WRITER_H
