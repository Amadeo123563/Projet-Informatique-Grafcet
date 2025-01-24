#include "reader.h"
#include "fstream"
#include "iostream"
#include "sstream"


Reader::Reader(const std::string& filename)
{
        parseFile(filename);

}

void Reader::parseFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier " << filename << std::endl;
        return;
    }

    std::string line;
    Node currentNode;
    Edge currentEdge;
    bool readingNode = false;
    bool readingEdge = false;

    while (std::getline(file, line)) {
        // Détection des sections
        if (line.find("<section name=\"node\">") != std::string::npos) {
            readingNode = true;
            readingEdge = false;
            currentNode = Node();
        }
        else if (line.find("<section name=\"edge\">") != std::string::npos) {
            readingNode = false;
            readingEdge = true;
            currentEdge = Edge();
        }
        else if (line.find("</section>") != std::string::npos) {
            if (readingNode) {
                nodes.push_back(currentNode);
                readingNode = false;
            }
            else if (readingEdge) {
                edges.push_back(currentEdge);
                readingEdge = false;
            }
        }

        // Lecture des attributs pour les noeuds
        if (readingNode && line.find("<attribute key=") != std::string::npos) {
            size_t keyStart = line.find("key=\"") + 5;
            size_t keyEnd = line.find("\"", keyStart);
            std::string key = line.substr(keyStart, keyEnd - keyStart);

            size_t valueStart = line.find(">", keyEnd) + 1;
            size_t valueEnd = line.find("</attribute>");
            std::string value = line.substr(valueStart, valueEnd - valueStart);

            if (key == "id") {
                currentNode.setId(std::stoi(value));
            }
            else if (key == "type") {
                currentNode.setType(value);
            }
            else if (key == "label") {
                currentNode.setText(value);
            }
        }

        // Lecture des attributs pour les arcs
        if (readingEdge && line.find("<attribute key=") != std::string::npos) {
            size_t keyStart = line.find("key=\"") + 5;
            size_t keyEnd = line.find("\"", keyStart);
            std::string key = line.substr(keyStart, keyEnd - keyStart);

            size_t valueStart = line.find(">", keyEnd) + 1;
            size_t valueEnd = line.find("</attribute>");
            std::string value = line.substr(valueStart, valueEnd - valueStart);

            if (key == "source") {
                currentEdge.setSource(std::stoi(value));
            }
            else if (key == "target") {
                currentEdge.setDestination(std::stoi(value));
            }
        }
    }

    file.close();
}

const std::vector<Node>& Reader::getNodes() const
{
    return nodes;
}

const std::vector<Edge>& Reader::getEdges() const
{
    return edges;
}
