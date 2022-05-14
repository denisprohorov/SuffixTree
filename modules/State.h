#pragma once

#include "Node.h"

#define NOMINMAX
#include "strategy/NodeOnMap.h"
#include <Windows.h>
#include <seqan/seq_io.h>

template<typename TAlphabet, class StoreStrategy,  template <typename T> class alloc>
class State {
public:
    typedef Node<TAlphabet, StoreStrategy> NodeDef;

    const seqan::String<TAlphabet> &base_str;
    NodeDef *activeNode;
    int bias;

    alloc<char>* allocator;

    alloc<NodeDef> nodeAlloc;

    template <class _Ty, class... _Types>
     std::unique_ptr<_Ty> make_node(_Types&&... _Args) { // make a unique_ptr
        NodeDef *node = nodeAlloc.allocate(1);
        nodeAlloc.construct(node, _Ty(_STD forward<_Types>(_Args)...));
        return std::unique_ptr<_Ty>(node);
    }


public:
    State(NodeDef *active_node, const seqan::String<TAlphabet> &baseStr, alloc<char> *allocator = nullptr) : activeNode(active_node),
                                                                           base_str(baseStr), bias(0), allocator(allocator), nodeAlloc(*allocator) {}

    bool is_transition(const TAlphabet symbol) const {
        if (activeNode->start_index + bias >= activeNode->end_index) {
            return activeNode->transitionNodes.has_transition_by_symbol(symbol);
        } else {
            int next_index = activeNode->start_index + bias;
            if (base_str[next_index] == symbol) {
                return true;
            }
            return false;
        }
    }

    NodeDef *get_link(NodeDef *node) {
        if (node->link != nullptr) return node->link;
        int l = node->start_index;
        int r = node->end_index;
        if (node->parent == nullptr) return nullptr;
        node->link = get_link(node->parent);
        if (node->link == nullptr) {
            node->link = node->parent;
            ++l;
        }
        while (l < r) {
            //get transition node
            node->link = node->link->transitionNodes.get_transition_node(base_str[l]);
            l += (node->link->end_index - node->link->start_index);
        }
        return node->link;
    }

    void go_by_link() {
        if (activeNode->parent == nullptr) return;
        int l = activeNode->start_index;
        int r = activeNode->start_index + bias;
        NodeDef *link = get_link(activeNode->parent);
        if (link == nullptr) {
            link = activeNode->parent;
            ++l;
        }
        this->activeNode = link;
        this->bias = this->activeNode->end_index - this->activeNode->start_index;
        while (l < r) {
            bias = std::min(activeNode->end_index - activeNode->start_index - bias, r - l);
            l += bias;
            if (l < r) {
                this->activeNode = activeNode->transitionNodes.get_transition_node(base_str[l]);
                bias = 0;
            }
        }
        return void();
    }

    void create_vertex(int symbol_index) {
        if (activeNode->start_index + bias == activeNode->end_index) {
            activeNode->transitionNodes.create_transition(base_str[symbol_index],
                                                           make_node<NodeDef>(symbol_index,
                                                                                     seqan::length(base_str),
                                                                                     allocator,
                                                                                     activeNode));
        } else {
            std::unique_ptr<NodeDef> old_active = activeNode->parent->transitionNodes.replace_transition(
                                                                                base_str[activeNode->start_index],
                                                                                make_node<NodeDef>(
                                                                                        activeNode->start_index,
                                                                                        activeNode->start_index + bias,
                                                                                        allocator,
                                                                                        activeNode->parent));
            NodeDef *new_vertex = activeNode->parent->transitionNodes.get_transition_node(
                    base_str[activeNode->start_index]);

            activeNode->start_index += bias;
            activeNode->parent = new_vertex;

            new_vertex->transitionNodes.create_transition(base_str[symbol_index], make_node<NodeDef>(
                    symbol_index, seqan::length(base_str), allocator, new_vertex));
            new_vertex->transitionNodes.create_transition(base_str[activeNode->start_index],
                                                           std::move(old_active));

            this->bias = new_vertex->end_index - new_vertex->start_index;
            this->activeNode = new_vertex;
        }
    }

    // use this function as alternative way to add terminate symbol.
    // can divide edge.
    void set_vertex_final() {
        if (activeNode->start_index + bias >= activeNode->end_index) {
            activeNode->isSuffixNode = true;
        } else {
            std::unique_ptr<NodeDef> old_active = activeNode->parent->transitionNodes.replace_transition(
                    base_str[activeNode->start_index],
                    make_node<NodeDef>(
                            activeNode->start_index,
                            activeNode->start_index + bias,
                            allocator,
                            activeNode->parent));
            NodeDef *new_vertex = activeNode->parent->transitionNodes.get_transition_node(
                    base_str[activeNode->start_index]);

            activeNode->start_index += bias;
            new_vertex->isSuffixNode = true;
            activeNode->parent = new_vertex;
            new_vertex->transitionNodes.create_transition(base_str[activeNode->start_index], std::move(old_active));

            this->bias = new_vertex->end_index - new_vertex->start_index;
            this->activeNode = new_vertex;
        }
    }

    void go_by_symbol(const TAlphabet symbol) {
        if (activeNode->start_index + bias >= activeNode->end_index) {
            activeNode = activeNode->transitionNodes.get_transition_node(symbol);
            bias = 1;
        } else {
            ++bias;
        }
    }
};

