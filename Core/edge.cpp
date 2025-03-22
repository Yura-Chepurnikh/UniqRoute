#include "edge.hpp"

Edge::Edge(double cost, Node* from, Node* to) : cost(cost)
{
    std::pair<Node*, Node*> new_pair(from, to);
    nodes = new_pair;
}
