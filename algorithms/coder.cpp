#include "coder.h"
#include "pinmode.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
 
 
Coder::Coder(const std::vector<Step>& steps, const std::vector<Action>& actions, const std::vector<Transition>& transitions)
{
    codeSteps.reserve(steps.size());
 
    // Conteneur pour enregistrer les variables
    std::unordered_map<std::string, int> variables;
 
    for (size_t i = 0; i < steps.size(); ++i) {
        const Step& step = steps[i];
 
        // Création d'un CodeStep pour chaque Step
        CodeStep codeStep;
        if (step.getInitialStep()) {
            codeStep.setIsInitial(true);
        }
 
        // Générer le code du constructeur (setTransitions)
        std::string constructorCode;
        constructorCode += "int* steps = (int*)malloc(" + std::to_string(step.getNextTransitions().size()) + " * sizeof(int));\n";
 
        for (size_t j = 0; j < step.getNextTransitions().size(); ++j) {
            int transitionIndex = step.getNextTransitions()[j];
            constructorCode += "steps[" + std::to_string(j) + "] = " + std::to_string(transitionIndex) + ";\n";
        }
 
        constructorCode += "setTransitions(" + std::to_string(step.getNextTransitions().size()) + ", steps);\n";
 
        // Ajouter la définition des PinModes
        constructorCode += "pinMode(" + std::to_string(step.getPin()) + ", OUTPUT);\n";
 
        // Ajouter le code de construction généré à l'objet CodeStep
        codeStep.setConstructorCode(constructorCode);
 
        // Générer les codes init, loop, end pour chaque action associée
        std::string initCode = "";
        std::string loopCode = "";
        std::string endCode = "";
 
        for (int actionIndex : step.getAssociatedActions()) {
            const Action& action = actions[actionIndex];
            std::string actionContent = action.getContent();
 
            // Traduction des actions pour Arduino
            if (actionContent.find(":=") != std::string::npos) {
                // Assignation de sortie ou variable
                size_t pos = actionContent.find(":=");
                std::string variable = actionContent.substr(0, pos - 1);
                std::string value = actionContent.substr(pos + 3);
 
                if (isdigit(value[0])) {
                    initCode += variable + " = " + value + ";\n"; // Assignation directe
                    variables[variable] = std::stoi(value); // Enregistrer la variable
                }
                else {
                    initCode += "// Erreur: Assignation non prise en charge avec une variable\n";
                }
            }
            else if (actionContent.find("SET ") == 0) {
                // Mémorisation à l'enclenchement
                std::string pin = actionContent.substr(4);
                initCode += "digitalWrite(" + pin + ", HIGH);\n";
            }
            else if (actionContent.find("RESET ") == 0) {
                // Mémorisation à la coupure
                std::string pin = actionContent.substr(6);
                initCode += "digitalWrite(" + pin + ", LOW);\n";
            }
            else if (actionContent.find("Afficher ") == 0) {
                // Messages d'alarme
                std::string message = actionContent.substr(9);
                initCode += "Serial.println(\"" + message + "\");\n";
            }
            else if (actionContent.find("STOP") == 0) {
                // Mise en sécurité
                initCode += "// Mise en sécurité activée\n";
                initCode += "while (true) {}\n";
            }
            else {
                // Actions non supportées
                initCode += "// Action non supportée: " + actionContent + "\n";
            }
        }
 
        codeStep.setInitCode(initCode);
        codeStep.setLoopCode(loopCode);  // Peut être vide pour cet exemple
        codeStep.setEndCode(endCode);    // Peut être vide pour cet exemple
 
        // Ajouter le CodeStep généré à la liste
        codeSteps.push_back(codeStep);
    }
 
    // Afficher l'état des variables après l'initialisation
    std::cout << "Initial State of Variables:\n";
    for (const auto& var : variables) {
        std::cout << var.first << " = " << var.second << std::endl;
    }
}
 
bool Coder::transitionClassique(const std::string& condition) const {
    // Ex: "Si X1 = TRUE"
    if (condition.find("Si ") == 0) {
        size_t pos = condition.find("=");
        if (pos != std::string::npos) {
            std::string variable = condition.substr(3, pos - 3);
            std::string value = condition.substr(pos + 2);
            return (digitalRead(variable.c_str()) == (value == "TRUE"));
        }
    }
    return false;
}
 
bool Coder::transitionChronometree(unsigned long& timerStart, unsigned long delayMs) const {
    // Ex: "T >= 5s" (delayMs = 5000)
    if (millis() - timerStart >= delayMs) {
        timerStart = millis();  // Réinitialisation du timer
        return true;
    }
    return false;
}
 
bool Coder::transitionForcee(int emergencyPin) const {
    // Ex: "Bouton d'arrêt d'urgence" (emergencyPin est le pin du bouton)
    return (digitalRead(emergencyPin) == HIGH);
}
 
bool Coder::transitionAnalogique(const std::string& analogCondition) const {
    // Ex: "Si Température >= 25°C"
    if (analogCondition.find("Si ") == 0) {
        size_t pos = analogCondition.find(">=");
        if (pos != std::string::npos) {
            std::string variable = analogCondition.substr(3, pos - 3);
            int threshold = std::stoi(analogCondition.substr(pos + 3));
            int analogValue = analogRead(variable.c_str());
            return (analogValue >= threshold);
        }
    }
    return false;
}
 
bool Coder::transitionCombinee(const std::string& conditions) const {
    // Remplacer "ET" par "&&", "OU" par "||" pour une syntaxe compatible Arduino
    std::string parsedConditions = conditions;
    size_t pos;
 
    while ((pos = parsedConditions.find("ET")) != std::string::npos) {
        parsedConditions.replace(pos, 2, "&&");
    }
    while ((pos = parsedConditions.find("OU")) != std::string::npos) {
        parsedConditions.replace(pos, 2, "||");
    }
 
    // Traduire les conditions "Si X1 = 1" en appels à digitalRead
    size_t start = 0;
    while ((pos = parsedConditions.find("Si ", start)) != std::string::npos) {
        size_t end = parsedConditions.find(")", pos);
        if (end == std::string::npos) {
            end = parsedConditions.size();
        }
        std::string condition = parsedConditions.substr(pos + 3, end - pos - 3);
 
        // Extrait les parties de la condition : X1 = 1
        size_t eqPos = condition.find("=");
        if (eqPos != std::string::npos) {
            std::string variable = condition.substr(0, eqPos - 1);
            std::string value = condition.substr(eqPos + 2);
 
            // Adapter la condition au GRAFCET
            std::string replacement = "(digitalRead(" + variable + ") == " + (value == "1" ? "HIGH" : "LOW") + ")";
            parsedConditions.replace(pos, end - pos + 1, replacement);
        }
        start = pos + 1;
    }
 
    // Exemple d'interprétation simplifiée (le GRAFCET n'évalue pas les chaînes dynamiquement)
    bool result = evaluateParsedConditions(parsedConditions);
 
    std::cout << "Parsed conditions for GRAFCET: " << parsedConditions << std::endl;
 
    return result;
}
 
const std::vector<CodeStep>& Coder::getCodeSteps() const
{
    return codeSteps;
}
 
// Impl�mentation de shouldTransitionTo
bool Coder::shouldTransitionTo(int step, const std::vector<int>& activeSteps) const
{
    // Recherche si l'�tape est dans les �tapes actives
    return inVector(step, activeSteps);
}
 
// Fonction utilitaire pour v�rifier si une �tape est active dans la liste
bool Coder::inVector(int step, const std::vector<int>& activeSteps) const
{
    for (int activeStep : activeSteps) {
        if (activeStep == step) {
            return true;
        }
    }
    return false;
}
 
const std::unordered_map<int, PinMode>& Coder::getPinmodes() const
{
    return pinmodes;
}
