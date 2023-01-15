#include "graph.h"

#include <math.h>
#include <time.h>

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

#include "../edge/edge.h"
#include "../node/node.h"
#include "../timer/timer.h"

using std::cout;
using std::endl;
using std::fstream;

using std::cin;
using std::greater;
using std::list;
using std::make_pair;
using std::pair;
using std::priority_queue;
using std::queue;
using std::string;
using std::vector;

constexpr int FLOAT_MIN = 0;
constexpr int FLOAT_MAX = 1;

Graph::Graph(Node *firstNode) {
    Node *node = firstNode;

    this->nodesTotal = 0;
    this->edgesTotal = 0;

    while (node) {
        Edge *edge = node->getFirstEdge();

        while (edge) {
            edge = edge->getNextEdge();
            this->edgesTotal++;
        }

        node = node->getNextNode();
        this->nodesTotal++;
    }

    this->firstNode = firstNode;
}

Graph::Graph(vector<Node *> nodesVector) {
    this->nodesTotal = 0;
    this->edgesTotal = 0;

    Node *node = nullptr;
    Node *previousNode = nullptr;

    for (int i = 0; i < nodesVector.size(); i++) {
        node = nodesVector[i]->clone();

        if (i == 0) {
            this->firstNode = node;
        }

        if (previousNode != nullptr) {
            previousNode->setNextNode(node);
        }

        this->nodesTotal++;
        previousNode = node;
    }

    vector<Node *> subNodesVector = getNodesVector();

    for (int i = 0; i < subNodesVector.size(); i++) {
        Edge *edge = nodesVector[i]->getFirstEdge();

        while (edge) {
            Node *foundTargetNode = this->getNodeById(edge->getTail()->getId());

            if (foundTargetNode != nullptr) {
                new Edge(subNodesVector[i], foundTargetNode, edge->getId());
                this->edgesTotal++;
            }

            edge = edge->getNextEdge();
        }
    }
}

Node *Graph::getNodeById(int id) {
    Node *node = this->firstNode;

    while (node) {
        if (node->getId() == id) {
            return node;
        }

        node = node->getNextNode();
    }

    return nullptr;
}

void Graph::setFirstNode(Node *node) {
    firstNode = node;
}

Node *Graph::getFirstNode() {
    return firstNode;
}

vector<Node *> Graph::getNodesVector() {
    vector<Node *> nodesVector;

    Node *node = this->firstNode;

    while (node) {
        nodesVector.push_back(node);
        node = node->getNextNode();
    }

    return nodesVector;
}

vector<Edge *> Graph::getEdgesVector() {
    vector<Edge *> edgesVector;

    Node *node = this->firstNode;
    Edge *edge = nullptr;

    while (node) {
        edge = node->getFirstEdge();

        while (edge) {
            edgesVector.push_back(edge);
            edge = edge->getNextEdge();
        }

        node = node->getNextNode();
    }

    return edgesVector;
}

void quicksort(std::vector<Node *> &nodesVector, int start, int end) {
    if (start >= end) {
        return;
    }
    // int pivot = (int)(start + end) / 2;
    int pivot = start;
    int i = start + 1;
    int j = end;
    while (i <= j) {
        if (nodesVector[i]->getHeuristicValue() > nodesVector[pivot]->getHeuristicValue() &&
            nodesVector[j]->getHeuristicValue() <= nodesVector[pivot]->getHeuristicValue()) {
            Node *tmp = nodesVector[i];
            nodesVector[i] = nodesVector[j];
            nodesVector[j] = tmp;
            i++;
            j--;
        } else {
            if (nodesVector[i]->getHeuristicValue() <= nodesVector[pivot]->getHeuristicValue()) {
                i++;
            }
            if (nodesVector[j]->getHeuristicValue() > nodesVector[pivot]->getHeuristicValue()) {
                j--;
            }
        }
    }
    Node *tmp = nodesVector[pivot];
    nodesVector[pivot] = nodesVector[j];
    nodesVector[j] = tmp;
    quicksort(nodesVector, start, j - 1);
    quicksort(nodesVector, j + 1, end);
}

void sortNodesVectorByHeuristic(std::vector<Node *> &nodesVector) {
    quicksort(nodesVector, 0, nodesVector.size() - 1);
}

bool Graph::isSubgraphValidSolution(vector<Node *> nodesVector, Graph *subGraph) {
    vector<Edge *> subEdgesVector = subGraph->getEdgesVector();

    bool found = true;
    Edge *edge = nullptr;
    for (int i = 0; i < nodesVector.size(); i++) {
        found = false;

        for (int j = 0; j < subEdgesVector.size(); j++) {
            int subHeadId = subEdgesVector[j]->getHead()->getId();
            int subTailNodeId = subEdgesVector[j]->getTail()->getId();

            if (nodesVector[i]->getId() == subHeadId || nodesVector[i]->getId() == subTailNodeId) {
                found = true;
                break;
            }

            edge = nodesVector[i]->getFirstEdge();
            while (edge) {
                if (edge->getTail()->getId() == subHeadId || edge->getTail()->getId() == subTailNodeId) {
                    found = true;
                    break;
                }
                edge = edge->getNextEdge();
            }

            if (found == true) {
                break;
            }
        }

        if (found == false) {
            break;
        }
    }

    return found;
}

double randomValue() {
    std::random_device rd;   // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

void Graph::greedy() {
    Timer *timer = new Timer("Time: ");

    vector<Node *> nodesVector = this->getNodesVector();
    vector<Node *> nodesSolution;

    sortNodesVectorByHeuristic(nodesVector);

    vector<Node *> nodesCandidates(nodesVector);

    while (nodesVector.size() > 0) {
        nodesSolution.push_back(nodesCandidates[0]);
        nodesCandidates.erase(std::remove(nodesCandidates.begin(), nodesCandidates.end(), nodesCandidates[0]), nodesCandidates.end());

        Graph *subGraph = new Graph(nodesSolution);

        bool found = isSubgraphValidSolution(nodesVector, subGraph);

        if (found) {
            break;
        }
    }

    int totalWeightsCost = 0;

    for (int i = 0; i < nodesSolution.size(); i++) {
        totalWeightsCost += nodesSolution[i]->getWeight();
    }

    cout << "Cost: " << totalWeightsCost << endl;

    delete timer;
}

int getRandomIndex(double alpha, int size) {
    return floor(ceil(alpha * size) * randomValue());
}

void Graph::randomGreedy(int iterations, double alpha) {
    Timer *timer = new Timer("Time: ");

    Graph *bestGraph = nullptr;
    int bestCost = INFINITY;

    while (iterations--) {
        vector<Node *> nodesVector = this->getNodesVector();
        vector<Node *> nodesSolution;

        sortNodesVectorByHeuristic(nodesVector);

        vector<Node *> nodesCandidates(nodesVector);

        Graph *subGraph = nullptr;

        while (nodesCandidates.size() > 0) {
            int randomIndex = getRandomIndex(alpha, nodesCandidates.size());
            nodesSolution.push_back(nodesCandidates[randomIndex]);
            nodesCandidates.erase(std::remove(nodesCandidates.begin(), nodesCandidates.end(), nodesCandidates[randomIndex]), nodesCandidates.end());

            subGraph = new Graph(nodesSolution);

            bool found = isSubgraphValidSolution(nodesVector, subGraph);

            if (found) {
                break;
            }
        }

        int totalWeightsCost = 0;

        for (int i = 0; i < nodesSolution.size(); i++) {
            totalWeightsCost += nodesSolution[i]->getWeight();
        }

        if (totalWeightsCost < bestCost) {
            bestCost = totalWeightsCost;
            bestGraph = subGraph;
        }
    }

    cout << "Cost: " << bestCost << endl;

    delete timer;
}

int chooseAlphaIndex(vector<double> alphas, vector<double> probabilities) {
    double value = randomValue();
    double acum = 0;

    for (int i = 0; i < probabilities.size(); i++) {
        acum += probabilities[i];

        if (value < acum) {
            return i;
        }
    }

    return probabilities.size() - 1;
}

void updateAverages(vector<double> averages, vector<int> usages, int totalWeightsCost, int alphaIndex) {
    usages[alphaIndex]++;

    if (averages[alphaIndex] == INFINITY) {
        averages[alphaIndex] = totalWeightsCost;
        return;
    }

    averages[alphaIndex] = ((double)(averages[alphaIndex] * (usages[alphaIndex] - 1) + totalWeightsCost)) / usages[alphaIndex];
}

void updateProbabilities(vector<double> averages, vector<double> probabilities, int bestCost) {
    vector<double> qualities;
    double qualitiesSum = 0;
    for (int i = 0; i < averages.size(); i++) {
        double quality = averages[i] != INFINITY ? pow(((double)bestCost) / averages[i], 100) : 0;
        qualitiesSum += quality;
        qualities.push_back(quality);
    }

    if (qualitiesSum == 0) {
        return;
    }

    for (int i = 0; i < qualities.size(); i++) {
        probabilities[i] = qualities[i] / qualitiesSum;
    }
}

void Graph::reactiveGreedy(int iterations, int block) {
    Timer *timer = new Timer("Time: ");

    // vector<double> alphas = {0.15, 0.30, 0.50};
    vector<double> alphas = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1};

    vector<double> probabilities;
    vector<double> averages;
    vector<int> usages;
    for (int i = 0; i < alphas.size(); i++) {
        probabilities.push_back(1.0 / alphas.size());
        averages.push_back(INFINITY);
        usages.push_back(0);
    }

    Graph *bestGraph = nullptr;
    int bestCost = INFINITY;

    for (int iter = 0; iter < iterations; iter++) {
        if ((iter + 1) % block == 0) {
            // ATUALIZA PROBABILIDADES
            updateProbabilities(averages, probabilities, bestCost);
        }

        vector<Node *> nodesVector = this->getNodesVector();
        vector<Node *> nodesSolution;

        sortNodesVectorByHeuristic(nodesVector);

        vector<Node *> nodesCandidates(nodesVector);

        Graph *subGraph = nullptr;

        int alphaIndex = chooseAlphaIndex(alphas, probabilities);

        while (nodesCandidates.size() > 0) {
            int randomIndex = getRandomIndex(alphas[alphaIndex], nodesCandidates.size());
            nodesSolution.push_back(nodesCandidates[randomIndex]);
            nodesCandidates.erase(std::remove(nodesCandidates.begin(), nodesCandidates.end(), nodesCandidates[randomIndex]), nodesCandidates.end());

            subGraph = new Graph(nodesSolution);

            bool found = isSubgraphValidSolution(nodesVector, subGraph);

            if (found) {
                break;
            }
        }

        int totalWeightsCost = 0;

        for (int i = 0; i < nodesSolution.size(); i++) {
            totalWeightsCost += nodesSolution[i]->getWeight();
        }

        // atualizar medias
        updateAverages(averages, usages, totalWeightsCost, alphaIndex);

        if (totalWeightsCost < bestCost) {
            bestCost = totalWeightsCost;
            bestGraph = subGraph;
        }
    }

    cout << "Cost: " << bestCost << endl;

    delete timer;
}

void Graph::print() {
    Node *node = this->firstNode;

    while (node) {
        cout << "\nNode" << endl;
        cout << " ID: " << node->getId() << endl;
        cout << " WEIGHT: " << node->getWeight() << endl;
        cout << " DEGREE: " << node->getDegree() << endl;
        cout << " HEURISTIC: " << node->getHeuristicValue() << endl;

        Edge *edge = node->getFirstEdge();

        cout << "\n Edges" << endl;

        while (edge) {
            cout << "\n   ID: " << edge->getId() << endl;
            cout << "   HEAD ID: " << edge->getHead()->getId() << endl;
            cout << "   TARGET ID: " << edge->getTail()->getId() << endl;

            edge = edge->getNextEdge();
            this->edgesTotal++;
        }

        node = node->getNextNode();
        this->nodesTotal++;
    }
}

Graph::~Graph() {
    delete this->firstNode;
}