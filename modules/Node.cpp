#pragma once

#include <map>
#include <memory>
#include <iostream>

#include <array>
#include <list>
#include <forward_list>
#include <unordered_map>
#define NOMINMAX
#include <seqan/basic/alphabet_storage.h>

template<typename TAlphabet, class StoreStrategy>
class ChildContainer;
template<typename TAlphabet>
class NodeOnMap;
template<typename TAlphabet>
class NodeOnArray;
template<typename TAlphabet>
class NodeOnHashMap;
template<typename TAlphabet>
class NodeOnList;

template<typename TAlphabet, class StoreStrategy = NodeOnArray<TAlphabet>>
class Node {
public:
    Node* parent;
    Node* link = nullptr;
    int start_index;
    int end_index;
    bool isSuffixNode = false;
    std::unique_ptr<StoreStrategy> transitionNodes;


    static int total_count;

    Node(int startIndex, int endIndex, Node* parent = nullptr) :
            parent(parent), start_index(startIndex), end_index(endIndex),
//            transitionNodes(std::make_unique<NodeOnArray<TAlphabet>>())
//            transitionNodes(std::make_unique<NodeOnMap<TAlphabet>>())
//            transitionNodes(std::make_unique<NodeOnHashMap<TAlphabet>>())
            transitionNodes(std::make_unique<StoreStrategy>())
//            transitionNodes(std::make_unique<NodeOnList<TAlphabet>>())
            { ++total_count;}

};

template<typename TAlphabet, class Strategy>
int Node<TAlphabet, Strategy>::total_count = 0;

template<typename TAlphabet, class StoreStrategy>
class ChildContainer {
public:
    typedef typename seqan::ValueSize<TAlphabet>::Type TSize;
    TSize alphSize = seqan::ValueSize<TAlphabet>::VALUE;

    struct Iterator {
        using pointer = Node<TAlphabet, StoreStrategy> *;
        using reference = Node<TAlphabet, StoreStrategy> &;

        virtual reference operator*() const = 0;

        virtual pointer operator->() = 0;

        virtual Iterator &operator++() = 0;

        virtual bool operator==(const Iterator &b) = 0;

        virtual bool operator!=(const Iterator &b) { return !this->operator==(b); }

        virtual ~Iterator() = default;
    };

    struct IteratorWrapper{
    public:
        using pointer = Node<TAlphabet, StoreStrategy> *;
        using reference = Node<TAlphabet, StoreStrategy> &;

        std::unique_ptr<Iterator> iterator;

        IteratorWrapper(std::unique_ptr<Iterator> &&iterator) : iterator(std::move(iterator)) {}

        reference operator*() const { return iterator->operator*();}

        pointer operator->() {return iterator->operator->();}

        void operator++() {iterator->operator++();}

        bool operator==(const IteratorWrapper &b) const {return iterator->operator==(*b.iterator);}

        bool operator!=(const IteratorWrapper &b) const { return !this->operator==(b); }
    };

    virtual bool has_transition_by_symbol(const TAlphabet symbol) = 0;

    virtual Node<TAlphabet, StoreStrategy>* get_transition_node(const TAlphabet symbol) = 0;

    virtual void create_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet, StoreStrategy>> transition_node) = 0;

    virtual std::unique_ptr<Node<TAlphabet, StoreStrategy>> replace_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet, StoreStrategy>> transition_node) = 0;

    virtual bool is_leaf() = 0;

    virtual IteratorWrapper begin() = 0;

    virtual IteratorWrapper end() = 0;

    virtual ~ChildContainer() = default;
};


