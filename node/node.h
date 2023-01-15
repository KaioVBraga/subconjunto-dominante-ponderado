#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>

// #include "../edge/edge.h"
// #include "../graph/graph.h"

using std::vector;

class Edge;
class Graph;

class Node {
   public:
    Node(int id, float weight);
    ~Node();

    int getId();

    Node *getNextNode();
    void setNextNode(Node *node);

    Edge *getFirstEdge();
    void setFirstEdge(Edge *firstEdge);

    void incrementDegree();

    int getDegree();

    int getWeight();

    double getHeuristicValue();

    Node *clone();

   private:
    int id;
    int weight;
    int degree;
    Node *nextNode;
    Edge *firstEdge;
};

#endif