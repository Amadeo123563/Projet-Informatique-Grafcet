#include "writer.h"

Writer::Writer(const std::vector<CodeStep>& steps, const std::vector<std::string>& globalVariables, const std::unordered_map<std::string, PinMode>& pinModes, std::string outputfolder)
{
    //[globalvars.cpp]
    copyFile("constantCode/globalvars.cpp", outputfolder + "/globalvars.cpp");

    //[globalvars.hpp]
    writeGlobalVarsHpp(outputfolder + "/globalvars.hpp", globalVariables);

    //[grafcet.cpp]
    copyFile("constantCode/grafcet.cpp", outputfolder + "/grafcet.cpp");

    //[grafcet.hpp]
    copyFile("constantCode/grafcet.hpp", outputfolder + "/grafcet.hpp");

    //[grafstep.cpp]
    copyFile("constantCode/grafstep.cpp", outputfolder + "/grafstep.cpp");

    //[grafstep.hpp]
    copyFile("constantCode/grafstep.hpp", outputfolder + "/grafstep.hpp");

    //[main.cpp]
    size_t pos = outputfolder.find_last_of("/\\");
    writeMainCpp(outputfolder + '/' + outputfolder.substr(pos, outputfolder.size() - pos) + ".ino", pinModes);

    //[specificgrafcet.cpp]
    writeSpecificGrafcetCpp(outputfolder + "/specificgrafcet.cpp", steps);

    //[specificgrafcet.hpp]
    copyFile("constantCode/specificgrafcet.hpp", outputfolder + "/specificgrafcet.hpp");

    //[steps.cpp]
    writeStepsCpp(outputfolder + "/steps.cpp", steps);

    //[steps.hpp]
    writeStepsHpp(outputfolder + "/steps.hpp", steps.size());

    //[constants.hpp]
    writeConstantsHpp(outputfolder + "/constants.hpp");
}

void Writer::copyFile(const std::string& source, const std::string& destination)
{
    std::ifstream src(source);
    std::ofstream dst(destination);
    dst << src.rdbuf();
    src.close();
    dst.close();
}

void Writer::writeGlobalVarsHpp(const std::string &destination, const std::vector<std::string>& globalVariables)
{
    std::ofstream dst(destination);
    dst <<  "#ifndef GLOBALVARS_HPP\n";
    dst <<  "#define GLOBALVARS_HPP\n";
    dst <<  "#include \"constants.hpp\"\n";
    dst <<  "#include <stdint.h>\n\n";
    dst <<  "struct GlobalVars\n";
    dst <<  "{\n";

    for(size_t v = 0; v < globalVariables.size(); v++)
    {
        dst << "    double " << globalVariables[v] << ";\n";
    }

    dst <<  "};\n\n";
    dst <<  "extern GlobalVars* globalVars;\n\n";
    dst <<  "#endif";
}

void Writer::writeMainCpp(const std::string &destination, const std::unordered_map<std::string, PinMode>& pinModes)
{
    std::ofstream dst(destination);
    dst << "#include <Arduino.h>\n";
    dst << "#include \"specificgrafcet.hpp\"\n\n";

    dst << "SpecificGrafcet* grafcet;\n\n";

    dst << "void setup() {\n";
    for(std::unordered_map<std::string, PinMode>::const_iterator it = pinModes.begin(); it != pinModes.end(); it++)
    {
        dst << "    pinMode(" << it->first << ", " << it->second.toString() << ");\n";
        const std::string& pinName = it->first;
        if (!std::regex_match(pinName, std::regex("^A?\\d+$"))) {
            undefinedPins.push_back(pinName);
        }
    }

    dst << "    grafcet = new SpecificGrafcet;\n";
    dst << "}\n\n";

    dst << "void loop() {\n";
    dst << "    grafcet->loop();\n";
    dst << "}";
}

void Writer::writeSpecificGrafcetCpp(const std::string &destination, const std::vector<CodeStep>& steps)
{
    std::ofstream dst(destination);
    dst <<  "#include \"specificgrafcet.hpp\"\n\n";

    dst <<  "SpecificGrafcet::SpecificGrafcet() : Grafcet(" + std::to_string(steps.size()) + ") {\n";

    for(size_t i = 0; i < steps.size(); i++)
    {
        const std::string iStr = std::to_string(i);
        dst << "    setStep(" + iStr + ", new Step" + iStr + ");\n";
    }

    dst << '\n';

    for(size_t i = 0; i < steps.size(); i++)
    {
        if(!steps[i].getIsInitial())
            continue;
        dst << "    activate(" + std::to_string(i) + ");\n";
    }

    dst << "}\n\nvoid SpecificGrafcet::loop() {\n    Grafcet::loop();\n}";
}

void Writer::writeStepsCpp(const std::string &destination, const std::vector<CodeStep>& steps)
{
    std::ofstream dst(destination);

    dst << "#include \"steps.hpp\"\n\n";

    for(size_t i = 0; i < steps.size(); i++)
    {
        dst << steps[i].getCode(i);
    }
}

void Writer::writeStepsHpp(const std::string &destination, size_t stepCount)
{
    std::ofstream dst(destination);

    dst << "#ifndef STEPS_HPP\n";
    dst << "#define STEPS_HPP\n\n";

    dst << "#include <Arduino.h>\n";
    dst << "#include \"grafstep.hpp\"\n";
    dst << "#include \"globalvars.hpp\"\n\n";

    for(size_t i = 0; i < stepCount; i++)
    {
        const std::string iStr = std::to_string(i);

        dst << "class Step" + iStr + " : public Grafstep {\n";
        dst << "public:\n";
        dst << "    Step" + iStr + "();\n\n";

        dst << "    virtual void init() override;\n";
        dst << "    virtual void loop() override;\n";
        dst << "    virtual void end() override;\n";
        dst << "    virtual bool shouldTransitionTo(int step, const std::vector<int>& activeSteps) const override;\n";
        dst << "};\n\n";
    }

    dst << "#endif //STEPS_HPP";
}

void Writer::writeConstantsHpp(const std::string &destination)
{
    std::ofstream dst(destination);

    for(size_t p = 0; p < undefinedPins.size(); p++)
    {
        dst << "#define " << undefinedPins[p] << " [PUT THE PIN NUMBER HERE]\n";
    }
}
