#pragma once

#include <map>
#include <memory>

#include <array>
#include <list>
#define NOMINMAX
#include <seqan/basic/alphabet_storage.h>

template<typename TAlphabet>
class ChildContainer;

template<typename TAlphabet>
class NodeOnMap;

template<typename TAlphabet>
class NodeOnArray;

template<typename TAlphabet>
class Node {
public:
    Node* parent;
    Node* link = nullptr;
    int start_index;
    int end_index;
    bool isSuffixNode = false;
    std::unique_ptr<ChildContainer<TAlphabet>> transitionNodes;


    static int total_count;

    Node(int startIndex, int endIndex, Node* parent = nullptr) :
            parent(parent), start_index(startIndex), end_index(endIndex),
//            transitionNodes(std::make_unique<NodeOnArray<TAlphabet>>()) {}
            transitionNodes(std::make_unique<NodeOnMap<TAlphabet>>()) { ++total_count;}
};

template<typename TAlphabet>
int Node<TAlphabet>::total_count = 0;

template<typename TAlphabet>
class ChildContainer {
public:
    typedef typename seqan::ValueSize<TAlphabet>::Type TSize;
    TSize alphSize = seqan::ValueSize<TAlphabet>::VALUE;

    struct Iterator {
        using pointer = Node<TAlphabet> *;
        using reference = Node<TAlphabet> &;

        virtual reference operator*() const = 0;

        virtual pointer operator->() = 0;

        virtual Iterator &operator++() = 0;

        virtual bool operator==(const Iterator &b) = 0;

        virtual bool operator!=(const Iterator &b) { return !this->operator==(b); }

        virtual ~Iterator() = default;
    };

    struct IteratorWrapper{
    public:
        std::unique_ptr<Iterator> iterator;

        IteratorWrapper(std::unique_ptr<Iterator> &&iterator) : iterator(std::move(iterator)) {}

        using pointer = Node<TAlphabet> *;
        using reference = Node<TAlphabet> &;

        reference operator*() const { return iterator->operator*();}

        pointer operator->() {return iterator->operator->();}

        void operator++() {iterator->operator++();}

        bool operator==(const IteratorWrapper &b) const {return iterator->operator==(*b.iterator);}

        bool operator!=(const IteratorWrapper &b) const { return !this->operator==(b); }
    };

    virtual bool has_transition_by_symbol(const TAlphabet symbol) = 0;

    virtual Node<TAlphabet>* get_transition_node(const TAlphabet symbol) = 0;

    virtual void create_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet>> transition_node) = 0;

    virtual std::unique_ptr<Node<TAlphabet>> replace_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet>> transition_node) = 0;

    virtual bool is_leaf() = 0;

    virtual IteratorWrapper begin() = 0;

    virtual IteratorWrapper end() = 0;

    virtual ~ChildContainer() = default;
};

//template<typename TAlphabet>
//class NodeOnArray : public ChildContainer<TAlphabet> {
//public:
//    typedef typename seqan::ValueSize<TAlphabet>::Type TSize;
//    static const TSize alphSize = seqan::ValueSize<TAlphabet>::VALUE;
//
//    std::array<std::shared_ptr<Node<TAlphabet>>, alphSize> boys;
//
//    struct ConcreteIterator : ChildContainer<TAlphabet>::Iterator {
//    public:
//        ConcreteIterator(std::_Array_iterator<std::shared_ptr<Node<TAlphabet>>, alphSize> ptr) : m_ptr(std::move(ptr)), end_ptr(m_ptr) {}
//
//        ConcreteIterator(std::_Array_iterator<std::shared_ptr<Node<TAlphabet>>, alphSize> ptr,
//                         std::_Array_iterator<std::shared_ptr<Node<TAlphabet>>, alphSize> end_ptr) : m_ptr(std::move(ptr)), end_ptr(end_ptr) {find_valid_pointer();}
//
//        ~ConcreteIterator() override = default;
//
//        reference operator*() const override {
//            return *m_ptr.operator*().get();
//        }
//
//        pointer operator->() override {
//            return m_ptr.operator->()->get();
//        }
//
//        Iterator &operator++() override {
//            m_ptr++;
//            find_valid_pointer();
//            return *this;
//        }
//
//        bool operator==(const Iterator &b) override {
//            return this->m_ptr == ((ConcreteIterator &) b).m_ptr;
//        }
//
//    private:
//        std::_Array_iterator<std::shared_ptr<Node<TAlphabet>>, alphSize> m_ptr;
//        std::_Array_iterator<std::shared_ptr<Node<TAlphabet>>, alphSize> end_ptr;
//
//        void find_valid_pointer(){
//            while(m_ptr != end_ptr && m_ptr->get() == nullptr){
//                m_ptr++;
//            }
//        }
//    };
//
//    NodeOnArray() = default;
//
//    ~NodeOnArray() override = default;
//
//    bool has_transition_by_symbol(const TAlphabet symbol) override {
//        auto &node = boys[symbol];
//        if (node == nullptr) {
//            return false;
//        }
//        return true;
//    }
//
//    std::shared_ptr<Node<TAlphabet>> get_transition_node(const TAlphabet symbol) override {
//        return boys[symbol];
//    }
//
//    void create_transition(const TAlphabet symbol, std::shared_ptr<Node<TAlphabet>> transition_node) override {
//        boys[symbol] = std::move(transition_node);
//    }
//
//    bool is_leaf() override {
//        for (int i = 0; i < alphSize; ++i) {
//            if (boys[i] != nullptr) return false;
//        }
//        return true;
//    }
//
//    IteratorWrapper begin() override {
//        return {std::make_unique<ConcreteIterator>(boys.begin(), boys.end())};
//    }
//
//    IteratorWrapper end() override {
//        return {std::make_unique<ConcreteIterator>(boys.end())};
//    }
//
//};

template<typename TAlphabet>
class NodeOnMap : public ChildContainer<TAlphabet> {
public:

    struct ConcreteIterator : ChildContainer<TAlphabet>::Iterator {
    public:
        ConcreteIterator(typename std::map<TAlphabet, std::unique_ptr<Node<TAlphabet>>>::iterator ptr)
                : m_ptr(std::move(ptr)) {}

        ~ConcreteIterator() override = default;

        reference operator*() const override {
            return m_ptr->second.operator*();
        }

        pointer operator->() override {
            return m_ptr->second.get();
        }

        Iterator &operator++() override {
            m_ptr++;
            return *this;
        }

        bool operator==(const Iterator &b) override {
            return this->m_ptr == ((ConcreteIterator &) b).m_ptr;
        }

    private:
        typename std::map<TAlphabet, std::unique_ptr<Node<TAlphabet>>>::iterator m_ptr;
    };

    std::map<const TAlphabet, std::unique_ptr<Node<TAlphabet>>> boys;

    NodeOnMap() = default;

    ~NodeOnMap() override = default;

    bool has_transition_by_symbol(const TAlphabet symbol) override {
        auto it = boys.find(symbol);
        if (it == boys.end()) {
            return false;
        }
        return true;
    }

    Node<TAlphabet>* get_transition_node(const TAlphabet symbol) override {
        return boys[symbol].get();
    }

    void create_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet>> transition_node) override {
        boys[symbol] = std::move(transition_node);
//        boys.emplace(symbol, transition_node);
    }

    std::unique_ptr<Node<TAlphabet>>
    replace_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet>> transition_node) override {
        std::unique_ptr<Node<TAlphabet>> old_transition = std::move(boys[symbol]);
        create_transition(symbol, std::move(transition_node));
//        return std::move(old_transition);
        return old_transition;
    }

    bool is_leaf() override {
        return boys.empty();
    }

    IteratorWrapper begin() override {
        return {std::make_unique<ConcreteIterator>(boys.begin())};
    }

    IteratorWrapper end() override {
        return {std::make_unique<ConcreteIterator>(boys.end())};
    }

};

