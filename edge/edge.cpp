#include "edge.h"

#include <iostream>

Edge::Edge(Node *head, Node *tail, int edgeCounter) {
    head->incrementDegree();

    this->head = head;
    this->tail = tail;
    this->nextEdge = nullptr;
    this->id = edgeCounter;

    Edge *headFirstEdge = head->getFirstEdge();

    if (headFirstEdge == nullptr) {
        head->setFirstEdge(this);
        return;
    }

    this->setNextEdge(headFirstEdge);
    head->setFirstEdge(this);
}

void Edge::setNextEdge(Edge *nextEdge) {
    this->nextEdge = nextEdge;
}

Edge *Edge::getNextEdge() {
    return nextEdge;
}

int Edge::getId() {
    return id;
}

Node *Edge::getTail() {
    return tail;
}

Node *Edge::getHead() {
    return head;
}

Edge *Edge::clone() {
    return new Edge(this->head, this->tail, this->id);
}

Edge::~Edge() {
    delete this->nextEdge;
}