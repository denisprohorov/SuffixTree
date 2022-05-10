#pragma once
#include "Node.h"


template<typename TAlphabet>
class NodeOnArray : public ChildContainer<TAlphabet, NodeOnArray<TAlphabet>> {
public:
    typedef typename seqan::ValueSize<TAlphabet>::Type TSize;
    static const TSize alphSize = seqan::ValueSize<TAlphabet>::VALUE;

    std::array<std::unique_ptr<Node<TAlphabet, NodeOnArray<TAlphabet>>>, alphSize> boys;

//    struct iterator {
//    public:
//        typedef typename std::array<std::unique_ptr<Node<TAlphabet, NodeOnArray<TAlphabet>>>, alphSize>::iterator arr_iterator;
//
//        iterator(arr_iterator ptr) : m_ptr(std::move(ptr)), end_ptr(m_ptr) {}
//
//        iterator(arr_iterator ptr, arr_iterator end_ptr) : m_ptr(std::move(ptr)), end_ptr(end_ptr) {find_valid_pointer();}
//
//        ~iterator() = default;
//
//        reference operator*() const override {
//            return *m_ptr.operator*().get();
//        }
//
//        pointer operator->() override {
//            return m_ptr.operator->()->get();
//        }
//
//        iterator &operator++() override {
//            m_ptr++;
//            find_valid_pointer();
//            return *this;
//        }
//
//        bool operator==(const iterator &b) override {
//            return this->m_ptr == b.m_ptr;
//        }
//
//    private:
//        arr_iterator m_ptr;
//        arr_iterator end_ptr;
//
//        void find_valid_pointer(){
//            while(m_ptr != end_ptr && m_ptr->get() == nullptr){
//                m_ptr++;
//            }
//        }
//    };

    NodeOnArray() = default;

    ~NodeOnArray() override = default;

    bool has_transition_by_symbol(const TAlphabet symbol) override {
        auto &node = boys[symbol];
        return node != nullptr;
    }

    Node<TAlphabet, NodeOnArray<TAlphabet>> *get_transition_node(const TAlphabet symbol) override {
        return boys[symbol].get();
    }

    void create_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet, NodeOnArray<TAlphabet>>> transition_node) override {
        boys[symbol] = std::move(transition_node);
    }

    std::unique_ptr<Node<TAlphabet, NodeOnArray<TAlphabet>>>
    replace_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet, NodeOnArray<TAlphabet>>> transition_node) override {
        std::unique_ptr<Node<TAlphabet, NodeOnArray<TAlphabet>>> old_transition = std::move(boys[symbol]);
        create_transition(symbol, std::move(transition_node));
        return old_transition;
    }

    bool is_leaf() override {
        for (int i = 0; i < alphSize; ++i) {
            if (boys[i] != nullptr) return false;
        }
        return true;
    }

//    iterator begin() override {
//        return {(boys.begin(), boys.end())};
//    }
//
//    iterator end() override {
//        return {(boys.end())};
//    }

};