#ifndef NODE_HPP
#define NODE_HPP

#include <string>

/* In this case we hold the elements separatly
 * but after, when we have more parameters as z or
 * humidity, we can hold them in container, say list.
 */

struct Node {
    const char* name;
    double x, y;

    bool operator==(const Node& other) const;

    Node(const char* name, double x, double y);

    Node(const Node& other);
    Node& operator=(const Node& other);

    Node(Node&& other) noexcept;
    Node& operator=(Node&& other) noexcept;

    ~Node();
};

namespace std {
    template<>
    struct hash<Node> {
        std::size_t operator()(const Node& node) const {
            return std::hash<std::string>()(std::string(node.name));
        }
    };
}

#endif // NODE_HPP
