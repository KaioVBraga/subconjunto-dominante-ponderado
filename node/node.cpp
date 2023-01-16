#include "node.h"

#include <iostream>

Node::Node(int id, float weight) {
    this->id = id;
    this->weight = weight;

    degree = 0;
    nextNode = nullptr;
    firstEdge = nullptr;
}

Node *Node::getNextNode() {
    return nextNode;
}
void Node::setNextNode(Node *node) {
    this->nextNode = node;
}

int Node::getId() {
    return id;
}

Edge *Node::getFirstEdge() {
    return firstEdge;
}

void Node::setFirstEdge(Edge *firstEdge) {
    this->firstEdge = firstEdge;
}

void Node::incrementDegree() {
    this->degree++;
}

int Node::getDegree() {
    return this->degree;
}

int Node::getWeight() {
    return this->weight;
}

double Node::getHeuristicValue() {
    return this->getDegree() + (10 / (double)this->getWeight());
}

Node *Node::clone() {
    return new Node(this->id, this->weight);
}

Node::~Node() {
    Edge *edge = this->getFirstEdge();
    delete edge;
    delete this->nextNode;
}