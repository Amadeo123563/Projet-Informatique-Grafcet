#include "coder.h"
#include "pinmode.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
 
Coder::Coder(const std::vector<Step>& steps, const std::vector<Action>& actions, const std::vector<Transition>& transitions)
{
    codeSteps.reserve(steps.size());
 
    for (size_t i = 0; i < steps.size(); ++i) {
        const Step& step = steps[i];
 
        // Création d'un CodeStep pour chaque Step
        CodeStep codeStep;
        if (step.getInitialStep()) {
            codeStep.setIsInitial(true);
        }
 
        // Générer le code du constructeur (setTransitions)
        int* nextSteps = (int*)malloc(step.getNextTransitions().size() * sizeof(int));
        for (size_t j = 0; j < step.getNextTransitions().size(); ++j) {
            nextSteps[j] = step.getNextTransitions()[j];
        }
        codeStep.setConstructorCode("int* steps = (int*)malloc(" + std::to_string(step.getNextTransitions().size()) + " * sizeof(int));");
        codeStep.setTransitionCode("setTransitions(" + std::to_string(step.getNextTransitions().size()) + ", steps);");
 
        // Générer les codes init, loop, end pour chaque action associée
        std::string initCode = "";
        std::string loopCode = "";
        std::string endCode = "";
 
        for (int actionIndex : step.getAssociatedActions()) {
            const Action& action = actions[actionIndex];
            std::string actionContent = action.getContent();
 
            // Gestion des différentes actions possibles
            if (actionContent.find("SET ") == 0) {
                // Ex: "SET x" devient "digitalWrite(x, HIGH);" dans init()
                initCode += "digitalWrite(" + actionContent.substr(4) + ", HIGH);\n";
            }
            else if (actionContent.find(":=") != std::string::npos) {
                // Ex: "Var := 1" devient "globalVars->Var = 1;" dans init()
                size_t pos = actionContent.find(":=");
                std::string variable = actionContent.substr(0, pos - 1);
                std::string value = actionContent.substr(pos + 3);
                initCode += "globalVars->" + variable + " = " + value + ";\n";
            }
            else if (actionContent == "MEMORY_SET") {
                // Action de mémorisation à l'enclenchement
                initCode += "setMemoryState(true);\n";
            }
            else if (actionContent == "MEMORY_RESET") {
                // Action de mémorisation à la coupure
                initCode += "setMemoryState(false);\n";
            }
            else if (actionContent == "ALARM") {
                // Action de message d'alarme
                initCode += "triggerAlarm();\n";
            }
            else if (actionContent == "SAFETY_MODE") {
                // Action de mise en sécurité
                initCode += "activateSafetyMode();\n";
            }
            else if (actionContent.find("COMBO_") == 0) {
                // Ex: "COMBO_1" pourrait signifier une combinaison d'actions
                initCode += "performComboAction(" + actionContent.substr(6) + ");\n";
            }
            else if (actionContent.find("CALC_") == 0) {
                // Ex: "CALC_x = Var + 1"
                initCode += "calculate(" + actionContent.substr(5) + ");\n";
            }
        }
 
        codeStep.setInitCode(initCode);
        codeStep.setLoopCode(loopCode);  // Peut être vide pour cet exemple
        codeStep.setEndCode(endCode);    // Peut être vide pour cet exemple
 
        // Ajouter le CodeStep généré à la liste
        codeSteps.push_back(codeStep);
    }
}
 
const std::vector<CodeStep>& Coder::getCodeSteps() const
{
    return codeSteps;
}
 
// Implémentation de shouldTransitionTo
bool Coder::shouldTransitionTo(int step, const std::vector<int>& activeSteps) const
{
    // Recherche si l'étape est dans les étapes actives
    return inVector(step, activeSteps);
}
 
// Fonction utilitaire pour vérifier si une étape est active dans la liste
bool Coder::inVector(int step, const std::vector<int>& activeSteps) const
{
    for (int activeStep : activeSteps) {
        if (activeStep == step) {
            return true;
        }
    }
    return false;
}
 
const std::vector<CodeStep>& Coder::getCodeSteps() const
{
    return codeSteps;
}
 
const std::unordered_map<int, PinMode>& Coder::getPinmodes() const
{
    return pinmodes;
}
