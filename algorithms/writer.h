#ifndef WRITER_H
#define WRITER_H

#include <vector>
#include <fstream>
#include <unordered_map>
#include <regex>
#include "data/codestep.h"
#include "data/pinmode.h"

class Writer
{
public:
    Writer(const std::vector<CodeStep>& steps, const std::vector<std::string>& globalVariables, const std::unordered_map<std::string, PinMode>& pinModes, std::string outputfolder);

private:
    void copyFile(const std::string& source, const std::string& destination);

    void writeGlobalVarsHpp(const std::string& destination, const std::vector<std::string>& globalVariables);
    void writeMainCpp(const std::string& destination, const std::unordered_map<std::string, PinMode>& pinModes);
    void writeSpecificGrafcetCpp(const std::string& destination, const std::vector<CodeStep>& steps);
    void writeStepsCpp(const std::string& destination, const std::vector<CodeStep>& steps);
    void writeStepsHpp(const std::string& destination, size_t stepCount);
    void writeConstantsHpp(const std::string& destination);

    std::vector<std::string> undefinedPins;
};

#endif // WRITER_H
