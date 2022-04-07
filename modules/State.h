#pragma once

#include "nodes/Node.h"
#include <string>

class State {
public:
    const std::string &base_str;
    std::shared_ptr<Node> activeNode;
    int bias;

public:
    State(const std::shared_ptr<Node> &node, const std::string &baseStr);

    bool is_transition(const char symbol) const;

    std::shared_ptr<Node> get_link(const std::shared_ptr<Node> &node);

    void go_by_link();

    void create_vertex(int symbol_index);

    void go_by_symbol(const char symbol);
};

