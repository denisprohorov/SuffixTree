#pragma once

#include "Node.h"
#include <map>


template<typename TAlphabet, class alloc>
class NodeOnMap : public ChildContainer<TAlphabet, NodeOnMap<TAlphabet, alloc>, alloc> {
public:
    typedef TAlphabet Key;
    typedef std::unique_ptr<NodeType> T;
    typedef typename std::map<const Key, T, std::less<Key>, alloc>::iterator iterator;


    std::map<const Key, T, std::less<Key>, alloc> boys;

    NodeOnMap(void *allocator) : ChildContainer<TAlphabet, NodeOnMap<TAlphabet, alloc>, alloc>((alloc*)allocator),
                                  boys(std::map<const Key, T, std::less<Key>, alloc>(*(alloc*)allocator)) {}

    NodeOnMap(NodeOnMap &&mov) : ChildContainer<TAlphabet, NodeOnMap<TAlphabet, alloc>, alloc>(mov.allocator),
                                 boys(std::move(mov.boys)) {}

    ~NodeOnMap() override = default;

    bool has_transition_by_symbol(const TAlphabet symbol) override {
        auto it = boys.find(symbol);
        if (it == boys.end()) {
            return false;
        }
        return true;
    }

    NodeType *get_transition_node(const TAlphabet symbol) override {
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
