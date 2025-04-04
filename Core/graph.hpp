#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <list>
#include "edge.hpp"
#include "node.hpp"

class Graph {
public:
    ~Graph();

    Graph(const Graph& other) = delete;
    Graph& operator=(const Graph& other) = delete;

    Graph(Graph&& other) = delete;
    Graph& operator=(Graph&& other) = delete;

    std::unordered_map<Node*, std::list<Edge*>> GetTable();

    static Graph* GetInstance(const std::unordered_map<Node*, std::list<Edge*>>& table);
    std::list<Edge*> FindEdgesByNode(Node* node);

    std::list<Node*> DijkstraAlgorithm(Node* starting_node, Node* final_node, Graph* graph);

private:
    inline static Graph* m_table;
    Graph(const std::unordered_map<Node*, std::list<Edge*>>& table);
    std::unordered_map<Node*, std::list<Edge*>> m_node_edge_table;
};

#endif // GRAPH_H
