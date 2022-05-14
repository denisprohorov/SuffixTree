#pragma once

#include <memory>

#define NOMINMAX

#include <seqan/basic/alphabet_storage.h>
#include <iostream>
#include <cassert>

struct memory {
    size_t DEBUG_SIZE_node = 3000;
    int *max_node = new int[DEBUG_SIZE_node] ();
    long long MAX_MEM_node = 10e8;
    long long mpos_node = 0;
    char *mem_node = new char[MAX_MEM_node];

    ~memory() {
        delete[] max_node;
        delete[] mem_node;
    }
};

template<class T>
class my_allocator {
public:

    std::shared_ptr<memory> mem;

    typedef size_t size_type;

    typedef ptrdiff_t difference_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T &const_reference;
    typedef T value_type;

    my_allocator() : mem(std::make_shared<memory>()) {}

    template<class U>
    my_allocator(const my_allocator<U> &old_alloc) : mem(old_alloc.mem) {}

    pointer allocate(size_type number_type, const void * = 0) {
        size_t n = sizeof(T) * number_type;

//        std::cout << n / number_type << std::endl;
        if (n > mem->DEBUG_SIZE_node) {
            std::cerr << " n > DEBUG_SIZE_node in operator new";
            throw ("can't allocate memory");
        }
        mem->max_node[n]++;
        char *res = mem->mem_node + mem->mpos_node;
        mem->mpos_node += n;
        assert(mem->mpos_node <= mem->MAX_MEM_node);
        return (pointer) res;
    }

    void deallocate(void *p, size_type) {
    }

    void construct (T* p, T&& value){
        char *data = (char*)&value;
        std::memcpy(p, data, sizeof(T));
    }

};

template<typename TAlphabet, class StoreStrategy>
class Node {
public:
    Node *parent;
    Node *link = nullptr;
    int start_index;
    int end_index;
    bool isSuffixNode = false;
    StoreStrategy transitionNodes;

    static int total_count;

    Node(int startIndex, int endIndex, void* allocator = nullptr, Node *parent = nullptr) :
            parent(parent), start_index(startIndex), end_index(endIndex), transitionNodes(allocator) { ++total_count; }

    Node(Node &&mov) : parent(mov.parent), link(mov.link), start_index(mov.start_index), end_index(mov.end_index),
                       isSuffixNode(mov.isSuffixNode), transitionNodes(std::move(mov.transitionNodes)) {}

    Node &operator=(Node &&mov) {
        parent = mov.parent;
        link = mov.link;
        start_index = mov.start_index;
        end_index = mov.end_index;
        isSuffixNode = mov.isSuffixNode;
        transitionNodes = std::move(mov.transitionNodes);
        return *this;
    }

    static void *operator new(size_t n) {
        std::cout << "new Node ----------------" << std::endl;
        return (void *) new char[n];
    }

    inline void operator delete(void *) {}

};

template<typename TAlphabet, class Strategy>
int Node<TAlphabet, Strategy>::total_count = 0;


template<typename TAlphabet, class StoreStrategy, class alloc>
class ChildContainer {
public:
    typedef typename seqan::ValueSize<TAlphabet>::Type TSize;
    TSize alphSize = seqan::ValueSize<TAlphabet>::VALUE;
    typedef Node<TAlphabet, StoreStrategy> NodeType;

    alloc *allocator;

    ChildContainer(alloc *allocator) : allocator(allocator) {}

    virtual bool has_transition_by_symbol(const TAlphabet symbol) = 0;

    virtual NodeType *get_transition_node(const TAlphabet symbol) = 0;

    virtual void create_transition(const TAlphabet symbol, std::unique_ptr<NodeType> transition_node) = 0;

    virtual std::unique_ptr<NodeType>
    replace_transition(const TAlphabet symbol, std::unique_ptr<NodeType> transition_node) = 0;

    virtual bool is_leaf() = 0;

    virtual ~ChildContainer() = default;
};


