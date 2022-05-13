#pragma once

#include <memory>

#define NOMINMAX
#include <seqan/basic/alphabet_storage.h>
#include <iostream>
#include <cassert>

struct memory{
    size_t DEBUG_SIZE_node = 3000;
    int* max_node = new int[DEBUG_SIZE_node];
    long long MAX_MEM_node = 10e8;
    long long mpos_node = 0;
    char *mem_node = new char[MAX_MEM_node];
};

template <class T>
class my_allocator {
public:

    std::shared_ptr<memory> mem;

    typedef size_t    size_type;

    typedef ptrdiff_t difference_type;
    typedef T*        pointer;
    typedef const T*  const_pointer;
    typedef T&        reference;
    typedef const T&  const_reference;
    typedef T         value_type;

    my_allocator() : mem(std::make_shared<memory>()){}
    template <class U>
    my_allocator(const my_allocator<U>& old_alloc) : mem(old_alloc.mem) {}

    pointer   allocate(size_type number_type, const void * = 0) {
        size_t n = sizeof(T) * number_type;

//        std::cout << n / number_type << std::endl;
        if(n > mem->DEBUG_SIZE_node) {std::cerr << " n > DEBUG_SIZE_node in operator new"; throw("can't allocate memory");}
        mem->max_node[n]++;
        char *res = mem->mem_node + mem->mpos_node;
        mem->mpos_node += n;
        assert(mem->mpos_node <= mem->MAX_MEM_node);
        return (pointer)res;
    }

    void      deallocate(void* p, size_type) {
    }

};

template<typename TAlphabet, class StoreStrategy, class alloc>
class Node {
public:
    Node* parent;
    Node* link = nullptr;
    int start_index;
    int end_index;
    bool isSuffixNode = false;
    StoreStrategy transitionNodes;

    static alloc allocator_p;

    static int total_count;

    Node(int startIndex, int endIndex, alloc *allocator = nullptr, Node* parent = nullptr) :
            parent(parent), start_index(startIndex), end_index(endIndex), transitionNodes(allocator)
            { ++total_count;}

    Node() {}

    static void * operator new ( size_t n ) {
        return (void *)allocator_p.allocate(n);
    }

    inline void operator delete ( void * ) { }

};

template<typename TAlphabet, class Strategy, class alloc>
int Node<TAlphabet, Strategy, alloc>::total_count = 0;

template<typename TAlphabet, class Strategy, class alloc>
alloc Node<TAlphabet, Strategy, alloc>::allocator_p = alloc();


template<typename TAlphabet, class StoreStrategy, class alloc>
class ChildContainer {
public:
    typedef typename seqan::ValueSize<TAlphabet>::Type TSize;
    TSize alphSize = seqan::ValueSize<TAlphabet>::VALUE;
    typedef Node<TAlphabet, StoreStrategy, alloc> NodeType;

    alloc *allocator;

    ChildContainer(alloc *allocator) : allocator(allocator) {}

    virtual bool has_transition_by_symbol(const TAlphabet symbol) = 0;

    virtual NodeType* get_transition_node(const TAlphabet symbol) = 0;

    virtual void create_transition(const TAlphabet symbol, std::unique_ptr<NodeType> transition_node) = 0;

    virtual std::unique_ptr<NodeType> replace_transition(const TAlphabet symbol, std::unique_ptr<NodeType> transition_node) = 0;

    virtual bool is_leaf() = 0;

    virtual ~ChildContainer() = default;
};


