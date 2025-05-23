#include "node.hpp"
#include <utility>

Node::Node() :
    name(""), x(0), y(0) { }

Node::Node(std::string name, double x, double y) :
    name(name), x(x), y(y) { }

Node::Node(const Node& other) :
    name(other.name), x(other.x), y(other.y) { }

Node& Node::operator=(const Node& other) {
    if (this == &other) return *this;

    name = other.name;
    x = other.x;
    y = other.y;

    return *this;
}

Node::Node(Node&& other) noexcept :
    name(std::move(other.name)), x(std::move(other.x)), y(std::move(other.y)) { }

Node& Node::operator=(Node&& other) noexcept {
    if (this == &other) return *this;

    name = std::move(other.name);
    x = std::move(other.x);
    y = std::move(other.y);

    return *this;
}

bool Node::operator==(const Node& other) const {
    return name == other.name;
}







