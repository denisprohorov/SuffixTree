#pragma once

#include <map>
#include <memory>
#include <iostream>

#include <array>
#include <list>
#include <unordered_map>
#define NOMINMAX
#include <seqan/basic/alphabet_storage.h>

template<typename TAlphabet>
class ChildContainer;
template<typename TAlphabet>
class NodeOnMap;
template<typename TAlphabet>
class NodeOnArray;
template<typename TAlphabet>
class NodeOnHashMap;
template<typename TAlphabet>
class NodeOnList;

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
//            transitionNodes(std::make_unique<NodeOnArray<TAlphabet>>())
//            transitionNodes(std::make_unique<NodeOnMap<TAlphabet>>())
//            transitionNodes(std::make_unique<NodeOnHashMap<TAlphabet>>())
            transitionNodes(std::make_unique<NodeOnList<TAlphabet>>())
            { ++total_count;}

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

template<typename TAlphabet>
class NodeOnArray : public ChildContainer<TAlphabet> {
public:
    typedef typename seqan::ValueSize<TAlphabet>::Type TSize;
    static const TSize alphSize = seqan::ValueSize<TAlphabet>::VALUE;

    std::array<std::unique_ptr<Node<TAlphabet>>, alphSize> boys;

    struct ConcreteIterator : ChildContainer<TAlphabet>::Iterator {
    public:
        typedef typename std::array<std::unique_ptr<Node<TAlphabet>>, alphSize>::iterator arr_iterator;

        ConcreteIterator(arr_iterator ptr) : m_ptr(std::move(ptr)), end_ptr(m_ptr) {}

        ConcreteIterator(arr_iterator ptr, arr_iterator end_ptr) : m_ptr(std::move(ptr)), end_ptr(end_ptr) {find_valid_pointer();}

        ~ConcreteIterator() override = default;

        reference operator*() const override {
            return *m_ptr.operator*().get();
        }

        pointer operator->() override {
            return m_ptr.operator->()->get();
        }

        Iterator &operator++() override {
            m_ptr++;
            find_valid_pointer();
            return *this;
        }

        bool operator==(const Iterator &b) override {
            return this->m_ptr == ((ConcreteIterator &) b).m_ptr;
        }

    private:
        arr_iterator m_ptr;
        arr_iterator end_ptr;

        void find_valid_pointer(){
            while(m_ptr != end_ptr && m_ptr->get() == nullptr){
                m_ptr++;
            }
        }
    };

    NodeOnArray() = default;

    ~NodeOnArray() override = default;

    bool has_transition_by_symbol(const TAlphabet symbol) override {
        auto &node = boys[symbol];
        if (node == nullptr) {
            return false;
        }
        return true;
    }

    Node<TAlphabet> *get_transition_node(const TAlphabet symbol) override {
        return boys[symbol].get();
    }

    void create_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet>> transition_node) override {
        boys[symbol] = std::move(transition_node);
    }

    std::unique_ptr<Node<TAlphabet>>
    replace_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet>> transition_node) override {
        std::unique_ptr<Node<TAlphabet>> old_transition = std::move(boys[symbol]);
        create_transition(symbol, std::move(transition_node));
        return old_transition;
    }

    bool is_leaf() override {
        for (int i = 0; i < alphSize; ++i) {
            if (boys[i] != nullptr) return false;
        }
        return true;
    }

    IteratorWrapper begin() override {
        return {std::make_unique<ConcreteIterator>(boys.begin(), boys.end())};
    }

    IteratorWrapper end() override {
        return {std::make_unique<ConcreteIterator>(boys.end())};
    }

};

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

    std::map<const TAlphabet, std::unique_ptr<Node<TAlphabet>>, std::less<TAlphabet>, std::allocator<std::pair<const TAlphabet, std::unique_ptr<Node<TAlphabet>>> >> boys;

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

template<typename TAlphabet>
class NodeOnHashMap : public ChildContainer<TAlphabet> {
public:

    std::unordered_map<int, std::unique_ptr<Node<TAlphabet>>> boys;

    struct ConcreteIterator : ChildContainer<TAlphabet>::Iterator {
    public:
        typedef typename std::unordered_map<int, std::unique_ptr<Node<TAlphabet>>>::iterator map_iterator;

        ConcreteIterator(map_iterator ptr)
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
        map_iterator m_ptr;
    };

    NodeOnHashMap() = default;

    ~NodeOnHashMap() override = default;

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
    }

    std::unique_ptr<Node<TAlphabet>>
    replace_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet>> transition_node) override {
        std::unique_ptr<Node<TAlphabet>> old_transition = std::move(boys[symbol]);
        create_transition(symbol, std::move(transition_node));
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

template<typename TAlphabet>
class NodeOnList : public ChildContainer<TAlphabet> {
public:

    std::list<std::pair<TAlphabet, std::unique_ptr<Node<TAlphabet>>>> boys;

    struct ConcreteIterator : ChildContainer<TAlphabet>::Iterator {
    public:
        typedef typename std::list<std::pair<TAlphabet, std::unique_ptr<Node<TAlphabet>>>>::iterator map_iterator;

        ConcreteIterator(map_iterator ptr)
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
        map_iterator m_ptr;
    };

    NodeOnList() = default;

    ~NodeOnList() override = default;

    bool has_transition_by_symbol(const TAlphabet symbol) override {
        auto it = std::find_if( std::begin( boys ),
                                std::end( boys ),
//                                [&]( const std::pair<TAlphabet, std::unique_ptr<Node<TAlphabet>>> &v ){ return v.first >= symbol; } );
                                [&]( const std::pair<TAlphabet, std::unique_ptr<Node<TAlphabet>>> &v ){ return v.first == symbol; } );
        if (it == boys.end()) {
            return false;
        }
        return it->first == symbol;
    }

    Node<TAlphabet>* get_transition_node(const TAlphabet symbol) override {
        auto it = std::find_if(  boys.begin(),
                                 boys.end(),
                                 [&]( const std::pair<TAlphabet, std::unique_ptr<Node<TAlphabet>>> &v ){ return v.first == symbol; } );
        return it->second.get();
    }

    void create_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet>> transition_node) override {
//        auto it = std::find_if(  boys.begin(),
//                                 boys.end(),
//                                 [&]( const std::pair<TAlphabet, std::unique_ptr<Node<TAlphabet>>> &v ){ return v.first > symbol; } );
//        boys.insert(it, {symbol, std::move(transition_node)});
        boys.push_back({symbol, std::move(transition_node)});
    }

    std::unique_ptr<Node<TAlphabet>>
    replace_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet>> transition_node) override {
        auto it = std::find_if(  boys.begin(),
                                 boys.end(),
                                [&]( const std::pair<TAlphabet, std::unique_ptr<Node<TAlphabet>>> &v ){ return symbol == v.first; } );
        std::unique_ptr<Node<TAlphabet>> old_transition = std::move(it->second);
        it->second = std::move(transition_node);
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

