#pragma once

#include <memory>

#define NOMINMAX

#include <seqan/basic/alphabet_storage.h>
#include <iostream>
#include <cassert>

struct memory {
    long long MAX_MEM = 50e8;
    long long mpos = 0;
    char *mem = new char[MAX_MEM];

    ~memory() {
        delete[] mem;
        std::cout << "delete----------------" << std::endl;
    }
};

template<class T>
class my_allocator {

public:
    memory *mem;
    bool isOwner = false;

    typedef size_t size_type;

    typedef ptrdiff_t difference_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T &const_reference;
    typedef T value_type;

    my_allocator() : mem(new memory()), isOwner(true) {}

    my_allocator(my_allocator &&alloc) : mem(alloc.mem), isOwner(alloc.isOwner){alloc.isOwner = false;}

    ~my_allocator() {
        if(isOwner) delete mem;
    }

    template<class U>
    my_allocator(const my_allocator<U> &old_alloc) : mem(old_alloc.mem), isOwner(false) {}

    pointer allocate(size_type number_type) {
        size_t n = sizeof(T) * number_type;

        char *res = mem->mem + mem->mpos;
        mem->mpos += n;
        assert(mem->mpos <= mem->MAX_MEM);
        return (pointer) res;
    }

    void deallocate(void *p, size_type) {
    }

    template <class _Objty, class... _Types>
    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS void construct(_Objty* const _Ptr, _Types&&... _Args) {
        new (std::_Voidify_iter(_Ptr)) _Objty(std::forward<_Types>(_Args)...);
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

//    static void *operator new(size_t n) {
//        std::cout << "new Node ----------------" << std::endl;
//        return (void *) new char[n];
//    }
//
//    inline void operator delete(void *) {}

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


