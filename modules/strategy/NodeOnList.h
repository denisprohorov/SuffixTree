#pragma once
#include "Node.h"
#include <forward_list>

template<typename TAlphabet>
class NodeOnList : public ChildContainer<TAlphabet, NodeOnList<TAlphabet>> {
public:

    typedef TAlphabet Key;
    typedef std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>> T;

    std::forward_list<std::pair<Key, T>, my_allocator<std::pair<Key, T>>> boys;

    typedef typename std::forward_list<std::pair<TAlphabet, std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>>>>::iterator iterator;

    NodeOnList() = default;

    ~NodeOnList() override = default;

    bool has_transition_by_symbol(const TAlphabet symbol) override {
        auto it = std::find_if( std::begin( boys ),
                                std::end( boys ),
//                                [&]( const std::pair<TAlphabet, std::unique_ptr<NodeDef<TAlphabet>>> &v ){ return v.first >= symbol; } );
                                [&]( const std::pair<TAlphabet, std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>>> &v ){ return v.first == symbol; } );
        if (it == boys.end()) {
            return false;
        }
        return it->first == symbol;
    }

    Node<TAlphabet, NodeOnList<TAlphabet>>* get_transition_node(const TAlphabet symbol) override {
        auto it = std::find_if(  boys.begin(),
                                 boys.end(),
                                 [&]( const std::pair<TAlphabet, std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>>> &v ){ return v.first == symbol; } );
        return it->second.get();
    }

    void create_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>> transition_node) override {
//        auto it = std::find_if(  next.begin(),
//                                 next.end(),
//                                 [&]( const std::pair<TAlphabet, std::unique_ptr<NodeDef<TAlphabet>>> &v ){ return v.first > symbol; } );
//        next.insert(it, {symbol, std::move(transition_node)});
        boys.push_front({symbol, std::move(transition_node)});
    }

    std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>>
    replace_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>> transition_node) override {
        auto it = std::find_if(  boys.begin(),
                                 boys.end(),
                                 [&]( const std::pair<TAlphabet, std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>>> &v ){ return symbol == v.first; } );
        std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>> old_transition = std::move(it->second);
        it->second = std::move(transition_node);
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