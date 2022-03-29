#include "State.h"

State::State(const std::shared_ptr<Node> &node, const std::string &baseStr) : activeNode(node), base_str(baseStr), bias(0) {}

bool State::is_transition(const char symbol) const {
    if (activeNode->start_index + bias >= activeNode->end_index) {
        auto it = activeNode->boys.find(symbol);
        if (it == activeNode->boys.end()) {
            return false;
        }
        return true;
    } else {
        int next_index = activeNode->start_index + bias;
        if (base_str[next_index] == symbol) {
            return true;
        }
        return false;
    }
}

std::shared_ptr<Node> State::get_link(const std::shared_ptr<Node> &node) {
    if(node->link.lock() != nullptr) return node->link.lock();
    int l = node->start_index;
    int r = node->end_index;
    if(node->parent.lock() == nullptr) return nullptr;
    node->link = get_link(node->parent.lock());
    if(node->link.lock() == nullptr) {
        node->link = node->parent;
        ++l;
    }
    while(l < r){
        node->link = node->link.lock()->boys[base_str[l]];
        l += (node->link.lock()->end_index - node->link.lock()->start_index);
    }
    return node->link.lock();
}

void State::go_by_link() {
    if (activeNode->parent.lock() == nullptr) return;
    int l = activeNode->start_index;
    int r = activeNode->start_index + bias;
    std::shared_ptr<Node> link = get_link(activeNode->parent.lock());
    if (link == nullptr) {
        link = activeNode->parent.lock();
        ++l;
    }
    this->activeNode = link;
    this->bias = this->activeNode->end_index - this->activeNode->start_index;
    while (l < r) {
        bias = std::min(activeNode->end_index - activeNode->start_index - bias, r - l);
        l += bias;
        if (l < r) {
            this->activeNode = activeNode->boys[base_str[l]];
            bias = 0;
        }
    }
    return void();
}

void State::create_vertex(int symbol_index) {
    if (activeNode->start_index + bias == activeNode->end_index) {
        activeNode->boys[base_str[symbol_index]] = std::make_shared<Node>(activeNode, symbol_index, base_str.size());
    } else {
        std::shared_ptr<Node> new_vertex(new Node(activeNode->parent, activeNode->start_index, activeNode->start_index + bias));
        activeNode->start_index += bias;
        new_vertex->parent.lock()->boys[base_str[new_vertex->start_index]] = new_vertex;
        activeNode->parent = new_vertex;
        new_vertex->boys[base_str[symbol_index]] = std::make_shared<Node>(
                new_vertex, symbol_index, base_str.size());
        new_vertex->boys[base_str[activeNode->start_index]] = activeNode;
        this->activeNode = new_vertex;
        this->bias = new_vertex->end_index - new_vertex->start_index;
    }
}

void State::go_by_symbol(const char symbol) {
    if (activeNode->start_index + bias >= activeNode->end_index) {
        activeNode = activeNode->boys.find(symbol)->second;
        bias = 1;
    } else {
        ++bias;
    }
}
