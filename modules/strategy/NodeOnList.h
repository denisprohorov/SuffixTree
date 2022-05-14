#pragma once
#include "Node.h"
#include <forward_list>

template<typename TAlphabet, class alloc>
class NodeOnList : public ChildContainer<TAlphabet, NodeOnList<TAlphabet, alloc>, alloc> {
public:
    typedef TAlphabet Key;
    typedef std::unique_ptr<NodeType> T;
    typedef typename std::forward_list<std::pair<TAlphabet, std::unique_ptr<NodeType>>>::iterator iterator;

    std::forward_list<std::pair<Key, T>, alloc> boys;

    NodeOnList(void *allocator) : ChildContainer<TAlphabet, NodeOnList<TAlphabet, alloc>, alloc>((alloc *)allocator),
            boys(std::forward_list<std::pair<Key, T>, alloc> (*(alloc *)allocator)){};

    NodeOnList(NodeOnList &&mov) : ChildContainer<TAlphabet, NodeOnList<TAlphabet, alloc>, alloc>(mov.allocator),
    boys(std::move(mov.boys)) {}

    ~NodeOnList() override = default;

    bool has_transition_by_symbol(const TAlphabet symbol) override {
        auto it = std::find_if( std::begin( boys ),
                                std::end( boys ),
//                                [&]( const std::pair<TAlphabet, std::unique_ptr<NodeDef<TAlphabet>>> &v ){ return v.first >= symbol; } );
                                [&]( const std::pair<TAlphabet, std::unique_ptr<NodeType>> &v ){ return v.first == symbol; } );
        if (it == boys.end()) {
            return false;
        }
        return it->first == symbol;
    }

    NodeType* get_transition_node(const TAlphabet symbol) override {
        auto it = std::find_if(  boys.begin(),
                                 boys.end(),
                                 [&]( const std::pair<TAlphabet, std::unique_ptr<NodeType>> &v ){ return v.first == symbol; } );
        return it->second.get();
    }

    void create_transition(const TAlphabet symbol, std::unique_ptr<NodeType> transition_node) override {
//        auto it = std::find_if(  next.begin(),
//                                 next.end(),
//                                 [&]( const std::pair<TAlphabet, std::unique_ptr<NodeDef<TAlphabet>>> &v ){ return v.first > symbol; } );
//        next.insert(it, {symbol, std::move(transition_node)});
        boys.push_front({symbol, std::move(transition_node)});
    }

    std::unique_ptr<NodeType>
    replace_transition(const TAlphabet symbol, std::unique_ptr<NodeType> transition_node) override {
        auto it = std::find_if(  boys.begin(),
                                 boys.end(),
                                 [&]( const std::pair<TAlphabet, std::unique_ptr<NodeType>> &v ){ return symbol == v.first; } );
        std::unique_ptr<NodeType> old_transition = std::move(it->second);
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
