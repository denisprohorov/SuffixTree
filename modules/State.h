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

    void create_vertex(int symbol_index) {
        if (activeNode->start_index + bias == activeNode->end_index) {
            activeNode->transitionNodes->create_transition(base_str[symbol_index],
                                                           std::make_shared<Node>(symbol_index, base_str.size(), activeNode));
        } else {
            std::shared_ptr<Node> new_vertex = std::make_shared<Node>(activeNode->start_index,
                                                                      activeNode->start_index + bias, activeNode->parent);
            activeNode->start_index += bias;
            new_vertex->parent.lock()->transitionNodes->create_transition(base_str[new_vertex->start_index], new_vertex);

            activeNode->parent = new_vertex;
            new_vertex->transitionNodes->create_transition(base_str[symbol_index], std::make_shared<Node>(
                    symbol_index, base_str.size(), new_vertex));
            new_vertex->transitionNodes->create_transition(base_str[activeNode->start_index], activeNode);

            this->bias = new_vertex->end_index - new_vertex->start_index;
            this->activeNode = std::move(new_vertex);
        }
    }

    void set_vertex_final(){
        if(activeNode->start_index + bias >= activeNode->end_index){
            //we in the end of edge
            activeNode->isSuffixNode = true;
        }else{
            std::shared_ptr<Node> new_vertex = std::make_shared<Node>(activeNode->start_index,
                                                                      activeNode->start_index + bias, activeNode->parent);
            new_vertex->isSuffixNode = true;
            activeNode->start_index += bias;
            new_vertex->parent.lock()->transitionNodes->create_transition(base_str[new_vertex->start_index], new_vertex);
            activeNode->parent = new_vertex;
            new_vertex->transitionNodes->create_transition(base_str[activeNode->start_index], activeNode);

            this->bias = new_vertex->end_index - new_vertex->start_index;
            this->activeNode = std::move(new_vertex);
        }
    }

    void go_by_symbol(const char symbol);
};

