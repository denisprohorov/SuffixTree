#pragma once

#include <memory>

#define NOMINMAX
#include <seqan/basic/alphabet_storage.h>
#include <iostream>
#include <cassert>


constexpr size_t DEBUG_SIZE_node = 3000;
int max_node[DEBUG_SIZE_node];
const long long MAX_MEM_node = 70e8;
long long mpos_node = 0;
//char mem_node[MAX_MEM_node];
char *mem_node = new char[MAX_MEM_node];

template <class T>
class my_allocator
{
public:

    typedef size_t    size_type;
    typedef ptrdiff_t difference_type;
    typedef T*        pointer;
    typedef const T*  const_pointer;
    typedef T&        reference;
    typedef const T&  const_reference;
    typedef T         value_type;

    my_allocator() {
    }
    my_allocator(const my_allocator&) {}



    pointer   allocate(size_type number_type, const void * = 0) {
        size_t n = sizeof(T) * number_type;

//        std::cout << n / number_type << std::endl;
        if(n > DEBUG_SIZE_node) {std::cerr << " n > DEBUG_SIZE_node in operator new"; throw("can't allocate memory");}
        max_node[n]++;
        char *res = mem_node + mpos_node;
        mpos_node += n;
        assert(mpos_node <= MAX_MEM_node);
        return (pointer)res;
    }

    void      deallocate(void* p, size_type) {
    }

    pointer           address(reference x) const { return &x; }
    const_pointer     address(const_reference x) const { return &x; }
    my_allocator<T>&  operator=(const my_allocator&) { return *this; }
    void              construct(pointer p, const T& val)
    { new ((T*) p) T(val); }
    void              destroy(pointer p) { p->~T(); }

    size_type         max_size() const { return size_t(-1); }

//    template <class U>
//    struct rebind { typedef my_allocator<U> other; };

    template <class U>
    my_allocator(const my_allocator<U>&) {
    }

//    template <class U>
//    my_allocator& operator=(const my_allocator<U>&) { return *this; }
};

//template <class T>
//class my_allocator
//{
//public:
//
//    typedef size_t    size_type;
//    typedef ptrdiff_t difference_type;
//    typedef T*        pointer;
//    typedef const T*  const_pointer;
//    typedef T&        reference;
//    typedef const T&  const_reference;
//    typedef T         value_type;
//
//    my_allocator() {
//    }
//    my_allocator(const my_allocator&) {}
//
//
//
//    pointer   allocate(size_type number_type, const void * = 0) {
//        size_t n = sizeof(T) * number_type;
//        return (pointer) new char[n];
//    }
//
//    void      deallocate(void* p, size_type) {
//        delete (char*)p;
//    }
//
//    pointer           address(reference x) const { return &x; }
//    const_pointer     address(const_reference x) const { return &x; }
//    my_allocator<T>&  operator=(const my_allocator&) { return *this; }
//    void              construct(pointer p, const T& val)
//    { new ((T*) p) T(val); }
//    void              destroy(pointer p) { p->~T(); }
//
//    size_type         max_size() const { return size_t(-1); }
//
////    template <class U>
////    struct rebind { typedef my_allocator<U> other; };
//
//    template <class U>
//    my_allocator(const my_allocator<U>&) {
//    }
//
////    template <class U>
////    my_allocator& operator=(const my_allocator<U>&) { return *this; }
//};

template<typename TAlphabet, class StoreStrategy>
class Node {
public:
    Node* parent;
    Node* link = nullptr;
    int start_index;
    int end_index;
    bool isSuffixNode = false;
    StoreStrategy transitionNodes;

    static int total_count;

    Node(int startIndex, int endIndex, Node* parent = nullptr) :
            parent(parent), start_index(startIndex), end_index(endIndex)
            { ++total_count;}

    void * operator new ( size_t n ) {
        if(n > DEBUG_SIZE_node) {std::cerr << " n > DEBUG_SIZE_node in operator new"; throw("can't allocate memory");}
        max_node[n]++;
        char *res = mem_node + mpos_node;
        mpos_node += n;
        assert(mpos_node <= MAX_MEM_node);
        return (void *)res;
    }

    inline void operator delete ( void * ) { }

};

template<typename TAlphabet, class Strategy>
int Node<TAlphabet, Strategy>::total_count = 0;

template<typename TAlphabet, class StoreStrategy>
class ChildContainer {
public:
    typedef typename seqan::ValueSize<TAlphabet>::Type TSize;
    TSize alphSize = seqan::ValueSize<TAlphabet>::VALUE;


    virtual bool has_transition_by_symbol(const TAlphabet symbol) = 0;

    virtual Node<TAlphabet, StoreStrategy>* get_transition_node(const TAlphabet symbol) = 0;

    virtual void create_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet, StoreStrategy>> transition_node) = 0;

    virtual std::unique_ptr<Node<TAlphabet, StoreStrategy>> replace_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet, StoreStrategy>> transition_node) = 0;

    virtual bool is_leaf() = 0;

    virtual ~ChildContainer() = default;
};


