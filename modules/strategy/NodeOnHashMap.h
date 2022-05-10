#pragma once
#include "Node.h"
#include <unordered_map>


template<typename TAlphabet>
class NodeOnHashMap : public ChildContainer<TAlphabet, NodeOnHashMap<TAlphabet>> {
public:
    typedef int Key;
    typedef std::unique_ptr<Node<TAlphabet, NodeOnHashMap<TAlphabet>>> T;
    std::unordered_map<Key, T, std::hash<Key>, std::equal_to<Key>, my_allocator<T>> boys;

    typedef typename std::unordered_map<Key, T, std::hash<Key>, std::equal_to<Key>, my_allocator<T>>::iterator iterator;

    NodeOnHashMap() = default;

    ~NodeOnHashMap() override = default;

    bool has_transition_by_symbol(const TAlphabet symbol) override {
        auto it = boys.find(symbol);
        if (it == boys.end()) {
            return false;
        }
        return true;
    }

    Node<TAlphabet, NodeOnHashMap<TAlphabet>>* get_transition_node(const TAlphabet symbol) override {
        return boys[symbol].get();
    }

    void create_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet, NodeOnHashMap<TAlphabet>>> transition_node) override {
        boys[symbol] = std::move(transition_node);
    }

    std::unique_ptr<Node<TAlphabet, NodeOnHashMap<TAlphabet>>>
    replace_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet, NodeOnHashMap<TAlphabet>>> transition_node) override {
        std::unique_ptr<Node<TAlphabet, NodeOnHashMap<TAlphabet>>> old_transition = std::move(boys[symbol]);
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