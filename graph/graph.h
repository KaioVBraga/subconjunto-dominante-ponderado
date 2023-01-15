#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <list>
#include <queue>
#include <vector>

// #include "../edge/edge.h"
// #include "../Node/node.h"

using std::list;
using std::make_pair;
using std::pair;
using std::priority_queue;
using std::string;
using std::vector;

class Node;
class Edge;
class Graph;

class Graph {
   public:
    // REFACTOR
    vector<pair<int, int>> readFile();

    // LEGACY

    Graph(Node *node);
    Graph(vector<Node *> nodesVector);

    ~Graph();

    void setLimit(int limit);
    int getLimit();

    void setFirstNode(Node *node);
    Node *getFirstNode();

    int *getAllAdjacents(int id, int *cont);
    Node *getNodeIfExist(int id);
    Node *getNodePkId(int id);

    vector<Node *> getNodesVector();

    vector<Edge *> getEdgesVector();

    bool isSubgraphValidSolution(vector<Node *> nodesVector, Graph *subGraph);

    void greedy();
    void randomGreedy(int iterations, double alpha);
    void reactiveGreedy(int iterations, int block);

    Node *getNodeById(int id);

    void print();

   private:
    vector<Edge *> vectorOfEdges;
    Node *firstNode;
    int nodesTotal;
    int edgesTotal;
};

#endif