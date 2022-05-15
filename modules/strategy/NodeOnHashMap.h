#pragma once
#include "Node.h"
#include <unordered_map>


template<typename TAlphabet, class alloc>
class NodeOnHashMap : public ChildContainer<TAlphabet, NodeOnHashMap<TAlphabet, alloc>, alloc> {
public:
    typedef int Key;
    typedef std::unique_ptr<NodeType> T;
    typedef typename std::unordered_map<Key, T, std::hash<Key>, std::equal_to<Key>, alloc>::iterator iterator;

    std::unordered_map<Key, T, std::hash<Key>, std::equal_to<Key>, alloc> nodes;

    NodeOnHashMap(void *allocator) : ChildContainer<TAlphabet, NodeOnHashMap<TAlphabet, alloc>, alloc>((alloc *)allocator),
                                     nodes(std::unordered_map<Key, T, std::hash<Key>, std::equal_to<Key>, alloc>(*(alloc *)allocator)){};

    NodeOnHashMap(NodeOnHashMap &&mov) : ChildContainer<TAlphabet, NodeOnHashMap<TAlphabet, alloc>, alloc>(mov.allocator),
                                         nodes(std::move(mov.nodes)) {}

    ~NodeOnHashMap() override = default;

    bool has_transition_by_symbol(const TAlphabet symbol) override {
        auto it = nodes.find(symbol);
        if (it == nodes.end()) {
            return false;
        }
        return true;
    }

    NodeType* get_transition_node(const TAlphabet symbol) override {
        return nodes[symbol].get();
    }

    void create_transition(const TAlphabet symbol, std::unique_ptr<NodeType> transition_node) override {
        nodes[symbol] = std::move(transition_node);
    }

    std::unique_ptr<NodeType>
    replace_transition(const TAlphabet symbol, std::unique_ptr<NodeType> transition_node) override {
        std::unique_ptr<NodeType> old_transition = std::move(nodes[symbol]);
        create_transition(symbol, std::move(transition_node));
        return old_transition;
    }

    bool is_leaf() override {
        return nodes.empty();
    }

    iterator begin() {
        return nodes.begin();
    }

    iterator end() {
        return nodes.end();
    }

};
