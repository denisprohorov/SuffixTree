#pragma once
#include "Node.h"
#include <map>



template<typename TAlphabet>
class NodeOnMap : public ChildContainer<TAlphabet, NodeOnMap<TAlphabet>> {
public:
    typedef TAlphabet Key;
    typedef std::unique_ptr<Node<TAlphabet, NodeOnMap<TAlphabet>>> T;

    std::map<const Key, T, std::less<Key>, my_allocator<std::pair<const Key, T>>> boys;

    typedef typename std::map<const Key, T, std::less<Key>, my_allocator<std::pair<const Key, T>>>::iterator iterator;

    NodeOnMap() = default;

    ~NodeOnMap() override = default;

    bool has_transition_by_symbol(const TAlphabet symbol) override {
        auto it = boys.find(symbol);
        if (it == boys.end()) {
            return false;
        }
        return true;
    }

    Node<TAlphabet, NodeOnMap<TAlphabet>>* get_transition_node(const TAlphabet symbol) override {
        return boys[symbol].get();
    }

    void create_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet, NodeOnMap<TAlphabet>>> transition_node) override {
        boys[symbol] = std::move(transition_node);
//        next.emplace(symbol, transition_node);
    }

    std::unique_ptr<Node<TAlphabet, NodeOnMap<TAlphabet>>>
    replace_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet, NodeOnMap<TAlphabet>>> transition_node) override {
        std::unique_ptr<Node<TAlphabet, NodeOnMap<TAlphabet>>> old_transition = std::move(boys[symbol]);
        create_transition(symbol, std::move(transition_node));
//        return std::move(old_transition);
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