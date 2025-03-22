#include "node_edge_table.hpp"
#include <stdexcept>

NodeEdgeTable::~NodeEdgeTable() {
    delete m_table;
}

NodeEdgeTable::NodeEdgeTable(const std::unordered_map<Node*, std::list<Edge*>>& table) {
    m_node_edge_table = table;
}

NodeEdgeTable* NodeEdgeTable::GetInstance(const std::unordered_map<Node*, std::list<Edge*>>& table) {
    if (!m_table) {
        m_table = new NodeEdgeTable(table);
    }
    return m_table;
}

std::unordered_map<Node*, std::list<Edge*>> NodeEdgeTable::GetTable() {
    return m_node_edge_table;
}

std::list<Edge*> NodeEdgeTable::FindEdgeByNode(Node* node) {
    auto table = m_table->GetTable();
    auto it = table.find(node);
    if (it != table.end()) {
        return it->second;
    }
    throw std::invalid_argument("Key is not found");
}
