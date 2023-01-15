#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

#include "./edge/edge.cpp"
#include "./graph/graph.cpp"
#include "./node/node.cpp"

void processOperationChoice(Graph *graph, int option, int iterations, double alpha, int block) {
    switch (option) {
        case 1:
            cout << "Algoritmo Guloso" << endl;
            graph->greedy();
            break;
        case 2:
            cout << "Algoritmo Guloso Randomizado" << endl;
            graph->randomGreedy(iterations, alpha);
            break;
        case 3:
            cout << "Algoritmo Guloso Randomizado Reativo" << endl;
            graph->reactiveGreedy(iterations, block);
            break;
    }
}

bool isSubstring(std::string mainString, std::string subString) {
    return mainString.find(subString) != std::string::npos;
}

string readFileSection(string inputFilePath, string start, string end) {
    std::ifstream file(inputFilePath);
    std::string line;
    string appendedString = "";

    bool isAppendable = false;

    while (std::getline(file, line)) {
        if (end != "" && isSubstring(line, end)) {
            isAppendable = false;
        }

        if (isAppendable) {
            appendedString += line + "\n";
        }

        if (start == "" || isSubstring(line, start)) {
            isAppendable = true;
        }
    }

    file.close();

    return appendedString;
}

double stringToDouble(const std::string &inputString) {
    std::istringstream iss(inputString);
    double result;
    iss >> result;
    return result;
}

int stringToInt(const std::string &inputString) {
    std::istringstream iss(inputString);
    int result;
    iss >> result;
    return result;
}

Node *convertStringToNodes(string nodesWeights) {
    std::istringstream iss(nodesWeights);
    std::string line;

    int counter = 1;

    Node *head = nullptr;
    Node *node = nullptr;
    Node *previousNode = nullptr;

    while (std::getline(iss, line)) {
        node = new Node(counter, stringToInt(line));

        if (head == nullptr) {
            head = node;
        }

        if (previousNode != nullptr) {
            previousNode->setNextNode(node);
        }

        previousNode = node;

        counter++;
    }

    return head;
}

Node *getNodeFromList(Node *head, int searchIndex) {
    Node *node = head;

    int i = 0;

    while (node) {
        if (i == searchIndex) {
            return node;
        }

        node = node->getNextNode();

        i++;
    }

    return nullptr;
}

void addEdgesFromStringToNodes(Node *head, string connectionMatrixStr) {
    std::istringstream iss(connectionMatrixStr);
    std::string line;
    std::string word;

    Node *node = head;

    int edgeCounter = 1;

    int i = 0;

    while (std::getline(iss, line)) {
        int j = 0;
        std::istringstream iline(line);
        while (iline >> word) {
            if (word == "1") {
                Node *sourceNode = getNodeFromList(head, i);
                Node *targetNode = getNodeFromList(head, j);

                new Edge(sourceNode, targetNode, edgeCounter);
                edgeCounter++;
            }

            j++;
        }
        i++;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        std::cerr
            << "[ERROR] Incorrect parameters\nExpecting: ./execGrupoX <input_file> <output_file> <option> <iterations> <alpha> <block>\n";
        exit(-1);
    }

    string inputFilePath = static_cast<string>(argv[1]);
    string ouputFilePath = static_cast<string>(argv[2]);
    int option = stringToInt(static_cast<string>(argv[3]));
    int iterations = stringToInt(static_cast<string>(argv[4]));
    double alpha = stringToDouble(static_cast<string>(argv[5]));
    double block = stringToInt(static_cast<string>(argv[6]));

    string numberOfNodesStr = readFileSection(inputFilePath, "NumberOfNodes:", "Positions");
    int numberOfNodes = stringToInt(numberOfNodesStr);

    string nodesWeightsStr = readFileSection(inputFilePath, "WEIGHTS", "CONNECTIONS");
    Node *head = convertStringToNodes(nodesWeightsStr);

    string connectionMatrixStr = readFileSection(inputFilePath, "CONNECTIONS", "");
    addEdgesFromStringToNodes(head, connectionMatrixStr);

    Graph *graph = new Graph(head);

    cout << "Instance: " << inputFilePath << endl;

    processOperationChoice(graph, option, iterations, alpha, block);

    return 0;
}
