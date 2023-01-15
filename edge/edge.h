#ifndef EDGE_H
#define EDGE_H

#include <iostream>

#include "../graph/graph.h"
#include "../node/node.h"

class Graph;
class Node;

class Edge {
   public:
    Edge(Node *head, Node *tail, int edgeCounter);
    ~Edge();

    void setNextEdge(Edge *nextEdge);
    Edge *getNextEdge();

    int getId();
    Node *getTail();
    Node *getHead();
    Edge *clone();

   private:
    int id;
    Node *tail;
    Node *head;
    Edge *nextEdge;
};

#endif