#pragma once
#include "Node.h"
#include <unordered_map>


template<typename TAlphabet, class alloc>
class NodeOnHashMap : public ChildContainer<TAlphabet, NodeOnHashMap<TAlphabet, alloc>, alloc> {
public:
    typedef int Key;
    typedef std::unique_ptr<NodeType> T;
    typedef typename std::unordered_map<Key, T, std::hash<Key>, std::equal_to<Key>, alloc>::iterator iterator;

    std::unordered_map<Key, T, std::hash<Key>, std::equal_to<Key>, alloc> boys;

    NodeOnHashMap(void *allocator) : ChildContainer<TAlphabet, NodeOnHashMap<TAlphabet, alloc>, alloc>((alloc *)allocator),
            boys(std::unordered_map<Key, T, std::hash<Key>, std::equal_to<Key>, alloc>(*(alloc *)allocator)){};

    NodeOnHashMap(NodeOnHashMap &&mov) : ChildContainer<TAlphabet, NodeOnHashMap<TAlphabet, alloc>, alloc>(mov.allocator),
    boys(std::move(mov.boys)) {}

    ~NodeOnHashMap() override = default;

    bool has_transition_by_symbol(const TAlphabet symbol) override {
        auto it = boys.find(symbol);
        if (it == boys.end()) {
            return false;
        }
        return true;
    }

    NodeType* get_transition_node(const TAlphabet symbol) override {
        return boys[symbol].get();
    }

    void create_transition(const TAlphabet symbol, std::unique_ptr<NodeType> transition_node) override {
        boys[symbol] = std::move(transition_node);
    }

    std::unique_ptr<NodeType>
    replace_transition(const TAlphabet symbol, std::unique_ptr<NodeType> transition_node) override {
        std::unique_ptr<NodeType> old_transition = std::move(boys[symbol]);
        create_transition(symbol, std::move(transition_node));
        return old_transition;
    }

    bool is_leaf() override {
        return boys.empty();
    }

    iterator begin() {
        return boys.begin();
    }

    iterator end() {
        return boys.end();
    }

};
