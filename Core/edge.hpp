#ifndef EDGE_HPP
#define EDGE_HPP

#include <utility>
#include "node.hpp"

/* In this simple example we use cost or weight of
 * graph as a distance between two nodes.
*/

struct Edge {
    double cost;
    std::pair<Node*, Node*> nodes;

    Edge(double cost, Node* from, Node* to);
};

#endif // EDGE_HPP
