#pragma once
#include "Node.h"
#include <forward_list>

template<typename TAlphabet, class alloc>
class NodeOnList : public ChildContainer<TAlphabet, NodeOnList<TAlphabet, alloc>, alloc> {
public:
    typedef TAlphabet Key;
    typedef std::unique_ptr<NodeType> T;
    typedef typename std::forward_list<std::pair<TAlphabet, std::unique_ptr<NodeType>>>::iterator iterator;

    std::forward_list<std::pair<Key, T>, alloc> nodes;

    NodeOnList(void *allocator) : ChildContainer<TAlphabet, NodeOnList<TAlphabet, alloc>, alloc>((alloc *)allocator),
                                  nodes(std::forward_list<std::pair<Key, T>, alloc> (*(alloc *)allocator)){};

    NodeOnList(NodeOnList &&mov) : ChildContainer<TAlphabet, NodeOnList<TAlphabet, alloc>, alloc>(mov.allocator),
                                   nodes(std::move(mov.nodes)) {}

    ~NodeOnList() override = default;

    bool has_transition_by_symbol(const TAlphabet symbol) override {
        auto it = std::find_if( std::begin(nodes ),
                                std::end(nodes ),
//                                [&]( const std::pair<TAlphabet, std::unique_ptr<NodeDef<TAlphabet>>> &v ){ return v.first >= symbol; } );
                                [&]( const std::pair<TAlphabet, std::unique_ptr<NodeType>> &v ){ return v.first == symbol; } );
        if (it == nodes.end()) {
            return false;
        }
        return it->first == symbol;
    }

    NodeType* get_transition_node(const TAlphabet symbol) override {
        auto it = std::find_if(nodes.begin(),
                               nodes.end(),
                               [&]( const std::pair<TAlphabet, std::unique_ptr<NodeType>> &v ){ return v.first == symbol; } );
        return it->second.get();
    }

    void create_transition(const TAlphabet symbol, std::unique_ptr<NodeType> transition_node) override {
//        auto it = std::find_if(  next.begin(),
//                                 next.end(),
//                                 [&]( const std::pair<TAlphabet, std::unique_ptr<NodeDef<TAlphabet>>> &v ){ return v.first > symbol; } );
//        next.insert(it, {symbol, std::move(transition_node)});
        nodes.push_front({symbol, std::move(transition_node)});
    }

    std::unique_ptr<NodeType>
    replace_transition(const TAlphabet symbol, std::unique_ptr<NodeType> transition_node) override {
        auto it = std::find_if(nodes.begin(),
                               nodes.end(),
                               [&]( const std::pair<TAlphabet, std::unique_ptr<NodeType>> &v ){ return symbol == v.first; } );
        std::unique_ptr<NodeType> old_transition = std::move(it->second);
        it->second = std::move(transition_node);
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

template<typename TAlphabet, class alloc>
class NodeOnListSort : public ChildContainer<TAlphabet, NodeOnListSort<TAlphabet, alloc>, alloc> {
public:
    typedef TAlphabet Key;
    typedef std::unique_ptr<NodeType> T;
    typedef typename std::forward_list<std::pair<TAlphabet, std::unique_ptr<NodeType>>>::iterator iterator;

    std::forward_list<std::pair<Key, T>, alloc> nodes;

    NodeOnListSort(void *allocator) : ChildContainer<TAlphabet, NodeOnListSort<TAlphabet, alloc>, alloc>((alloc *)allocator),
                                  nodes(std::forward_list<std::pair<Key, T>, alloc> (*(alloc *)allocator)){};

    NodeOnListSort(NodeOnListSort &&mov) : ChildContainer<TAlphabet, NodeOnListSort<TAlphabet, alloc>, alloc>(mov.allocator),
                                   nodes(std::move(mov.nodes)) {}

    ~NodeOnListSort() override = default;

    bool has_transition_by_symbol(const TAlphabet symbol) override {
        auto it = std::find_if( std::begin(nodes ),
                                std::end(nodes ),
                                [&]( const std::pair<TAlphabet, T> &v ){ return v.first >= symbol; } );
//                                [&]( const std::pair<TAlphabet, std::unique_ptr<NodeType>> &v ){ return v.first == symbol; } );
        if (it == nodes.end()) {
            return false;
        }
        return it->first == symbol;
    }

    NodeType* get_transition_node(const TAlphabet symbol) override {
        auto it = std::find_if(nodes.begin(),
                               nodes.end(),
                               [&]( const std::pair<TAlphabet, std::unique_ptr<NodeType>> &v ){ return v.first == symbol; } );
        return it->second.get();
    }

    void create_transition(const TAlphabet symbol, std::unique_ptr<NodeType> transition_node) override {
        if(nodes.empty()) return nodes.push_front({symbol, std::move(transition_node)});
        auto fin_it = nodes.begin();
        for (auto it = nodes.begin(); it != nodes.end(); it++) {
            if ((it.operator*().first > symbol)) break;
            fin_it = it;
        }
        if(fin_it == nodes.begin() && fin_it.operator*().first > symbol) return nodes.push_front({symbol, std::move(transition_node)});
        nodes.insert_after(fin_it, {symbol, std::move(transition_node)});
    }

    std::unique_ptr<NodeType>
    replace_transition(const TAlphabet symbol, std::unique_ptr<NodeType> transition_node) override {
        auto it = std::find_if(nodes.begin(),
                               nodes.end(),
                               [&]( const std::pair<TAlphabet, std::unique_ptr<NodeType>> &v ){ return v.first == symbol; } );
        std::unique_ptr<NodeType> old_transition = std::move(it->second);
        it->second = std::move(transition_node);
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
