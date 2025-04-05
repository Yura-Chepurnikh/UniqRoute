#include <graph.hpp>
#include <utility>
#include <list>
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
    std::unordered_map<Node*, int> shortest;
    std::unordered_map<Node*, Node*> previous_node;

    std::list<Node*> unvisited_nodes;

    auto table = graph->GetTable();

    for (const auto& [node, edges] : table) {
        unvisited_nodes.push_back(node);
        shortest[node] = INT_MAX;
    }
    shortest[starting_node] = 0;

    Node* current_node = starting_node;

    while (!unvisited_nodes.empty())
    {
        int shortest_distance = INT_MAX;
        for (const auto& node : unvisited_nodes) {
            if (shortest[node] < shortest_distance) {
                shortest_distance = shortest[node];
                current_node = node;
            }
        }

        unvisited_nodes.remove(current_node);

        if (current_node == final_node) break;

        auto edges = graph->FindEdgesByNode(current_node);
        for (auto edge : edges)
        {
            Node* neighbor = edge->nodes.second;
            int update_distance = shortest[current_node] + edge->cost;

            if (update_distance < shortest[neighbor])
            {
                shortest[neighbor] = update_distance;
                previous_node[neighbor] = current_node;
            }
        }
    }

    std::list<Node*> shortest_path;
    for (Node* node = final_node; node != nullptr; node = previous_node[node]) {
        shortest_path.push_back(node);
    }

    for (auto node : shortest_path) {
        qDebug() << QString::fromStdString(node->name);
    }

    return shortest_path;
}
