#include "associator.h"
#include <unordered_map> // Pour associer identifiants et indices

Associator::Associator(const std::vector<Node>& nodes, const std::vector<Edge>& edges)
{
    // Tables pour associer les identifiants à des indices dans les vecteurs
    std::unordered_map<int, int> stepIndices;
    std::unordered_map<int, int> actionIndices;
    std::unordered_map<int, int> transitionIndices;

    // 1. Parcourir les nœuds pour les classer et remplir les tables
    for (size_t i = 0; i < nodes.size(); ++i) {
        const Node& node = nodes[i];
        if (node.getType() == Node::Step || node.getType() == Node::InitialStep) {
            Step step;
            step.setContent(node.getText());
            step.setInitialStep(node.getType() == Node::InitialStep);
            stepIndices[node.getId()] = steps.size(); // Associer l'identifiant à l'index
            steps.push_back(step);
        }
        else if (node.getType() == Node::Action) {
            Action action;
            action.setContent(node.getText());
            actionIndices[node.getId()] = actions.size(); // Associer l'identifiant à l'index
            actions.push_back(action);
        }
        else if (node.getType() == Node::Transition) {
            Transition transition;
            transition.setContent(node.getText());
            transitionIndices[node.getId()] = transitions.size(); // Associer l'identifiant à l'index
            transitions.push_back(transition);
        }
    }

    // 2. Parcourir les arêtes pour établir les relations
    for (size_t i = 0; i < edges.size(); ++i) {
        const Edge& edge = edges[i];
        int sourceId = edge.getSource();
        int destinationId = edge.getDestination();

        // Vérifier si la source est une étape
        if (stepIndices.find(sourceId) != stepIndices.end()) {
            Step& sourceStep = steps[stepIndices[sourceId]];

            // Si la destination est une transition, l'ajouter comme transition suivante
            if (transitionIndices.find(destinationId) != transitionIndices.end()) {
                sourceStep.addTransition(transitionIndices[destinationId]);
            }

            // Si la destination est une action, l'ajouter comme action associée
            if (actionIndices.find(destinationId) != actionIndices.end()) {
                sourceStep.addAction(actionIndices[destinationId]);
            }
        } // Vérifier si la source est une transition
        else if (transitionIndices.find(sourceId) != transitionIndices.end()) {
            Transition& sourceTransition = transitions[transitionIndices[sourceId]];

            // Si la destination est une étape, l'ajouter comme étape suivante
            if (stepIndices.find(destinationId) != stepIndices.end()) {
                sourceTransition.addStep(stepIndices[destinationId]);
            }
        }
    }

const std::vector<Step>& Associator::getSteps() const
{
    return steps;
}

const std::vector<Action>& Associator::getActions() const
{
    return actions;
}

const std::vector<Transition>& Associator::getTransitions() const
{
    return transitions;
}
