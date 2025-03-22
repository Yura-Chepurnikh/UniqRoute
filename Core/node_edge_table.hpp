#ifndef NODE_EDGE_TABLE_HPP
#define NODE_EDGE_TABLE_HPP

#include <unordered_map>
#include <list>
#include "edge.hpp"
#include "node.hpp"

/* For navigation in our graph we use the third-party class
 * NodeEdgeTable where we hold our Nodes and their edges.
 * NodeEdgeTable this is wrap to hash table where keys are
 * Nodes and value is an edge. This help us easily find edge by
 * Node (FindEdgeByNode). For void a losses we make this class
 * SINGLETON!
 */

class NodeEdgeTable {
public:
    ~NodeEdgeTable();

    NodeEdgeTable(const NodeEdgeTable& other) = delete;
    NodeEdgeTable& operator=(const NodeEdgeTable& other) = delete;

    NodeEdgeTable(NodeEdgeTable&& other) = delete;
    NodeEdgeTable& operator=(NodeEdgeTable&& other) = delete;

    std::unordered_map<Node*, std::list<Edge*>> GetTable();

    static NodeEdgeTable* GetInstance(const std::unordered_map<Node*, std::list<Edge*>>& table);
    std::list<Edge*> FindEdgeByNode(Node* node);

private:
    static NodeEdgeTable* m_table;
    NodeEdgeTable(const std::unordered_map<Node*, std::list<Edge*>>& table);
    std::unordered_map<Node*, std::list<Edge*>> m_node_edge_table;
};

NodeEdgeTable* NodeEdgeTable::m_table = nullptr;

#endif // NODE_EDGE_TABLE_HPP
