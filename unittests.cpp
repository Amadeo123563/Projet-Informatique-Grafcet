#include "unittests.h"

int UnitTests::success = 0;
int UnitTests::failures = 0;

void UnitTests::assert(bool condition, const std::string &name)
{
    if(condition) {
        std::cout << name << " PASSED\n";
        success++;
    } else {
        std::cout << name << " FAILED\n";
        failures++;
    }
}

void UnitTests::assertFiles(const std::string &path1, const std::string &path2, const std::string &name)
{
    std::ifstream file1(path1);
    std::ifstream file2(path2);
    std::string line1, line2;
    while(std::getline(file1, line1) && std::getline(file2, line2)) {
        if(line1 != line2) {
            std::cout << name << " FAILED\n";
            failures++;
            return;
        }
    }
    std::cout << name << " PASSED\n";
    success++;
}

void UnitTests::testReader() {
    std::cout << "Testing Reader..." << std::endl;

    Reader reader("unittest/test1.xgml");
    assert(reader.getNodes().size() == 4, "NodeCount");
    assert(reader.getNodes()[0].getId() == 0, "Node0ID");
    assert(reader.getNodes()[0].getType() == Node::InitialStep, "Node0Type");
    assert(reader.getNodes()[0].getText() == "A", "Node0Text");
    assert(reader.getNodes()[1].getId() == 1, "Node1ID");
    assert(reader.getNodes()[1].getType() == Node::Step, "Node1Type");
    assert(reader.getNodes()[1].getText() == "B", "Node1Text");
    assert(reader.getNodes()[2].getId() == 2, "Node2ID");
    assert(reader.getNodes()[2].getType() == Node::Transition, "Node2Type");
    assert(reader.getNodes()[2].getText() == "C", "Node2Text");
    assert(reader.getNodes()[3].getId() == 3, "Node3ID");
    assert(reader.getNodes()[3].getType() == Node::Action, "Node3Type");
    assert(reader.getNodes()[3].getText() == "D", "Node3Text");

    assert(reader.getEdges().size() == 3, "EdgeCount");
    assert(reader.getEdges()[0].getSource() == 1, "Edge0Source");
    assert(reader.getEdges()[0].getDestination() == 2, "Edge0Destination");
    assert(reader.getEdges()[1].getSource() == 2, "Edge1Source");
    assert(reader.getEdges()[1].getDestination() == 3, "Edge1Destination");
    assert(reader.getEdges()[2].getSource() == 1, "Edge2Source");
    assert(reader.getEdges()[2].getDestination() == 3, "Edge2Destination");

    std::cout << "Reader test complete." << std::endl;
}

void UnitTests::testAssociator() {
    std::cout << "Testing Associator..." << std::endl;

    std::vector<Node> nodes;
    nodes.push_back(Node());
    nodes[0].setId(0);
    nodes[0].setType(Node::InitialStep);
    nodes.push_back(Node());
    nodes[1].setId(1);
    nodes[1].setType(Node::Transition);
    nodes[1].setText("T1");
    nodes.push_back(Node());
    nodes[2].setId(2);
    nodes[2].setType(Node::Step);
    nodes.push_back(Node());
    nodes[3].setId(3);
    nodes[3].setType(Node::Transition);
    nodes[3].setText("T2");
    nodes.push_back(Node());
    nodes[4].setId(4);
    nodes[4].setType(Node::Action);
    nodes[4].setText("A1");
    nodes.push_back(Node());
    nodes[5].setId(5);
    nodes[5].setType(Node::Action);
    nodes[5].setText("A2");
    nodes.push_back(Node());
    nodes[6].setId(6);
    nodes[6].setType(Node::Step);
    nodes.push_back(Node());
    nodes[7].setId(7);
    nodes[7].setType(Node::Action);
    nodes[7].setText("A3");
    nodes.push_back(Node());
    nodes[8].setId(8);
    nodes[8].setType(Node::Action);
    nodes[8].setText("A4");

    std::vector<Edge> edges;
    edges.push_back(Edge());
    edges[0].setSource(0);
    edges[0].setDestination(1);
    edges.push_back(Edge());
    edges[1].setSource(1);
    edges[1].setDestination(2);
    edges.push_back(Edge());
    edges[2].setSource(2);
    edges[2].setDestination(3);
    edges.push_back(Edge());
    edges[3].setSource(3);
    edges[3].setDestination(0);
    edges.push_back(Edge());
    edges[4].setSource(0);
    edges[4].setDestination(4);
    edges.push_back(Edge());
    edges[5].setSource(2);
    edges[5].setDestination(5);
    edges.push_back(Edge());
    edges[6].setSource(6);
    edges[6].setDestination(7);
    edges.push_back(Edge());
    edges[7].setSource(1);
    edges[7].setDestination(6);
    edges.push_back(Edge());
    edges[8].setSource(6);
    edges[8].setDestination(3);
    edges.push_back(Edge());
    edges[9].setSource(6);
    edges[9].setDestination(8);

    Associator associator(nodes, edges);
    assert(associator.getSteps().size() == 3, "StepCount");
    assert(associator.getSteps()[0].getInitialStep(), "Step0IsInitial");
    assert(!associator.getSteps()[1].getInitialStep(), "Step1IsInitial");
    assert(!associator.getSteps()[2].getInitialStep(), "Step2IsInitial");
    assert(associator.getSteps()[0].getNextTransitions().size() == 1, "Step0NextTransitionCount");
    assert(associator.getSteps()[0].getNextTransitions()[0] == 0, "Step0NextTransition0");
    assert(associator.getSteps()[1].getNextTransitions().size() == 1, "Step1NextTransitionCount");
    assert(associator.getSteps()[1].getNextTransitions()[0] == 1, "Step1NextTransition0");
    assert(associator.getSteps()[2].getNextTransitions().size() == 1, "Step2NextTransitionCount");
    assert(associator.getSteps()[2].getNextTransitions()[0] == 1, "Step2NextTransition0");
    assert(associator.getSteps()[0].getAssociatedActions().size() == 1, "Step0ActionsCount");
    assert(associator.getSteps()[0].getAssociatedActions()[0] == 0, "Step0Actions0");
    assert(associator.getSteps()[1].getAssociatedActions().size() == 1, "Step1ActionsCount");
    assert(associator.getSteps()[1].getAssociatedActions()[0] == 1, "Step1Actions0");
    assert(associator.getSteps()[2].getAssociatedActions().size() == 2, "Step2ActionsCount");
    assert(associator.getSteps()[2].getAssociatedActions()[0] == 2, "Step2Actions0");
    assert(associator.getSteps()[2].getAssociatedActions()[1] == 3, "Step2Actions1");
    assert(associator.getTransitions().size() == 2, "TransitionCount");
    assert(associator.getTransitions()[0].getContent() == "T1", "Transition0Content");
    assert(associator.getTransitions()[1].getContent() == "T2", "Transition1Content");
    assert(associator.getTransitions()[0].getNextSteps().size() == 2, "Transition0NextStepCount");
    assert(associator.getTransitions()[0].getNextSteps()[0] == 1, "Transition0NextStep0");
    assert(associator.getTransitions()[0].getNextSteps()[1] == 2, "Transition0NextStep1");
    assert(associator.getTransitions()[1].getNextSteps().size() == 1, "Transition1NextStepCount");
    assert(associator.getTransitions()[1].getNextSteps()[0] == 0, "Transition1NextStep0");
    assert(associator.getActions().size() == 4, "ActionCount");
    assert(associator.getActions()[0].getContent() == "A1", "Action0Content");
    assert(associator.getActions()[1].getContent() == "A2", "Action1Content");
    assert(associator.getActions()[2].getContent() == "A3", "Action2Content");
    assert(associator.getActions()[3].getContent() == "A4", "Action3Content");

    std::cout << "Associator test complete." << std::endl;
}

void UnitTests::testCoder() {
    std::cout << "Testing Coder..." << std::endl;

    std::vector<Step> steps;
    steps.push_back(Step());
    steps[0].setInitialStep(true);
    steps[0].addTransition(0);
    steps[0].addAction(0);
    steps[0].addAction(1);
    steps.push_back(Step());
    steps[1].setInitialStep(true);
    steps[1].addTransition(0);
    steps.push_back(Step());
    steps[2].setInitialStep(false);

    std::vector<Transition> transitions;
    transitions.push_back(Transition());
    transitions[0].setContent("P1 PRESSED? AND WAIT 5s OR X &gt; 10");
    transitions[0].addStep(2);

    std::vector<Action> actions;
    actions.push_back(Action());
    actions[0].setContent("Y := 0");
    actions.push_back(Action());
    actions[1].setContent("SET P2");

    Coder coder(steps, actions, transitions);
    assert(coder.getGlobalVariables().size() == 2, "GlobalVariablesCount");
    assert(coder.getGlobalVariables()[0] == "Y", "GlobalVariables0");
    assert(coder.getGlobalVariables()[1] == "X", "GlobalVariables1");
    assert(coder.getPinmodes().size() == 2, "PinmodesCount");
    assert(coder.getPinmodes().find("P1") != coder.getPinmodes().end(), "PinmodesP1Exists");
    assert(coder.getPinmodes().at("P1").getMode() == PinMode::Input_Pulldown, "PinmodesP1Mode");
    assert(coder.getPinmodes().find("P2") != coder.getPinmodes().end(), "PinmodesP2Exists");
    assert(coder.getPinmodes().at("P2").getMode() == PinMode::Output, "PinmodesP2Mode");
    assert(coder.getCodeSteps().size() == 3, "CodeStepsCount");
    assert(coder.getCodeSteps()[0].getCode(0) == "Step0::Step0() : Grafstep(0) {\n    int* steps = (int*)malloc(1 * sizeof(int));\n    steps[0] = 2;\n    setTransitions(1, steps);\n\n}\n\nvoid Step0::init() {\n    Grafstep::init();\n    globalVars->Y = 0;\n    digitalWrite(P2, HIGH);\n\n}\n\nvoid Step0::loop() {\n\n}\n\nvoid Step0::end() {\n\n}\n\nbool Step0::shouldTransitionTo(int step, const std::vector<int>& activeSteps) const {\n    switch(step) {\n    case 2:\n        return (inVector(1, activeSteps)) && (digitalRead(P1) == HIGH && timeSinceActivation() >= 5*1000 ||  globalVars->X > 10);\n    }\n    return true;\n}\n\n\n", "CodeSteps0Code");
    assert(coder.getCodeSteps()[1].getCode(1) == "Step1::Step1() : Grafstep(1) {\n    int* steps = (int*)malloc(1 * sizeof(int));\n    steps[0] = 2;\n    setTransitions(1, steps);\n\n}\n\nvoid Step1::init() {\n    Grafstep::init();\n\n}\n\nvoid Step1::loop() {\n\n}\n\nvoid Step1::end() {\n\n}\n\nbool Step1::shouldTransitionTo(int step, const std::vector<int>& activeSteps) const {\n    switch(step) {\n    case 2:\n        return (inVector(0, activeSteps)) && (digitalRead(P1) == HIGH && timeSinceActivation() >= 5*1000 ||  globalVars->X > 10);\n    }\n    return true;\n}\n\n\n", "CodeSteps1Code");
    assert(coder.getCodeSteps()[2].getCode(2) == "Step2::Step2() : Grafstep(2) {\n    int* steps = (int*)malloc(0 * sizeof(int));\n    setTransitions(0, steps);\n\n}\n\nvoid Step2::init() {\n    Grafstep::init();\n\n}\n\nvoid Step2::loop() {\n\n}\n\nvoid Step2::end() {\n\n}\n\nbool Step2::shouldTransitionTo(int step, const std::vector<int>& activeSteps) const {\n    switch(step) {\n    }\n    return true;\n}\n\n\n", "CodeSteps2Code");

    std::cout << "Coder test complete." << std::endl;
}

void UnitTests::testWriter() {
    std::cout << "Testing Writer..." << std::endl;

    std::vector<CodeStep> steps;
    steps.push_back(CodeStep());
    steps[0].setIsInitial(true);
    steps[0].setConstructorCode("//Constructor code");
    steps[0].setInitCode("//Init code");
    steps[0].setLoopCode("//Loop code");
    steps[0].setEndCode("//End code");
    steps[0].setTransitionCode("//Transition code");

    std::vector<std::string> globalVariables;
    globalVariables.push_back("Var1");
    globalVariables.push_back("Var2");

    std::unordered_map<std::string, PinMode> pinModes;
    pinModes["Pin1"] = PinMode(PinMode::Input);
    pinModes["Pin2"] = PinMode(PinMode::Input_Pullup);
    pinModes["Pin3"] = PinMode(PinMode::Input_Pulldown);
    pinModes["Pin4"] = PinMode(PinMode::Output);

    Writer writer(steps, globalVariables, pinModes, "unittest/output/generated");

    assertFiles("unittest/output/needed/constants.hpp", "unittest/output/generated/constants.hpp", "FileConstantsHpp");
    assertFiles("unittest/output/needed/generated.ino", "unittest/output/generated/generated.ino", "FileGenerateIno");
    assertFiles("unittest/output/needed/globalvars.cpp", "unittest/output/generated/globalvars.cpp", "FileGlobalVarsCpp");
    assertFiles("unittest/output/needed/globalvars.hpp", "unittest/output/generated/globalvars.hpp", "FileGlobalVarsHpp");
    assertFiles("unittest/output/needed/grafcet.cpp", "unittest/output/generated/grafcet.cpp", "FileGrafcetCpp");
    assertFiles("unittest/output/needed/grafcet.hpp", "unittest/output/generated/grafcet.hpp", "FileGrafcetHpp");
    assertFiles("unittest/output/needed/grafstep.cpp", "unittest/output/generated/grafstep.cpp", "FileGrafstepCpp");
    assertFiles("unittest/output/needed/grafstep.hpp", "unittest/output/generated/grafstep.hpp", "FileGrafstepHpp");
    assertFiles("unittest/output/needed/specificgrafcet.cpp", "unittest/output/generated/specificgrafcet.cpp", "FileSpecificGrafcetCpp");
    assertFiles("unittest/output/needed/specificgrafcet.hpp", "unittest/output/generated/specificgrafcet.hpp", "FileSpecificGrafcetHpp");
    assertFiles("unittest/output/needed/steps.cpp", "unittest/output/generated/steps.cpp", "FileStepsCpp");
    assertFiles("unittest/output/needed/steps.hpp", "unittest/output/generated/steps.hpp", "FileStepsHpp");

    std::cout << "Writer test complete." << std::endl;
}
