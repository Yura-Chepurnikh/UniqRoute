#include "graph.hpp"
#include <stdexcept>
#include <QDebug>

Graph::~Graph() {
    delete m_table;
}

Graph::Graph(const std::unordered_map<Node*, std::list<Edge*>>& table) {
    m_node_edge_table = table;
}

Graph* Graph::GetInstance(const std::unordered_map<Node*, std::list<Edge*>>& table) {
    if (!m_table) {
        m_table = new Graph(table);
    }
    return m_table;
}

std::unordered_map<Node*, std::list<Edge*>> Graph::GetTable() {
    return m_node_edge_table;
}

std::list<Edge*> Graph::FindEdgesByNode(Node* node) {
    auto table = m_table->GetTable();
    auto it = table.find(node);
    if (it != table.end()) {
        return it->second;
    }
    throw std::invalid_argument("Key is not found");
}

std::list<Node*> Graph::DijkstraAlgorithm(Node *starting_node, Node *final_node, Graph* graph) {
    std::list<Node*> result {starting_node};
    return result;
}
