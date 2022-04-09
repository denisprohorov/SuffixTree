#pragma once

#include <map>
#include <memory>

#include <vector>

struct Node;

class ChildContainer {
public:
    struct Iterator {
        using pointer = Node *;
        using reference = Node &;

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

        using pointer = Node *;
        using reference = Node &;

        reference operator*() const { return iterator->operator*();}

        pointer operator->() {return iterator->operator->();}

        Iterator &operator++(int) {return iterator->operator++();}

        Iterator &operator++() {return iterator->operator++();}

        bool operator==(const IteratorWrapper &b) const {return iterator->operator==(*b.iterator);}

        bool operator!=(const IteratorWrapper &b) const { return !this->operator==(b); }
    };

    virtual bool has_transition_by_symbol(const char symbol) = 0;

    virtual std::shared_ptr<Node> get_transition_node(const char symbol) = 0;

    virtual void create_transition(const char symbol, std::shared_ptr<Node> transition_node) = 0;

    virtual bool is_leaf() = 0;

    virtual IteratorWrapper begin() = 0;

    virtual IteratorWrapper end() = 0;

    virtual ~ChildContainer() = default;
};

class NodeOnMap : public ChildContainer {
public:
    struct It : ChildContainer::Iterator {
    public:
        It(std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const char, std::shared_ptr<Node>>>>> ptr)
                : m_ptr(std::move(ptr)) {}

        ~It() override = default;

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
            return this->m_ptr == ((It &) b).m_ptr;
        }

    private:
        std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const char, std::shared_ptr<Node>>>>> m_ptr;
    };

    std::map<const char, std::shared_ptr<Node>> boys;

    NodeOnMap() = default;

    ~NodeOnMap() override = default;

    bool has_transition_by_symbol(const char symbol) override {
        auto it = boys.find(symbol);
        if (it == boys.end()) {
            return false;
        }
        return true;
    }

    std::shared_ptr<Node> get_transition_node(const char symbol) override {
        return boys[symbol];
    }

    void create_transition(const char symbol, std::shared_ptr<Node> transition_node) override {
        boys[symbol] = std::move(transition_node);
    }

    bool is_leaf() override {
        return boys.empty();
    }

    IteratorWrapper begin() override {
        return {std::make_unique<It>(boys.begin())};
    }

    IteratorWrapper end() override {
        return {std::make_unique<It>(boys.end())};
    }

};

//class NodeOnVector

class Node {
public:
    std::weak_ptr<Node> parent;
    std::weak_ptr<Node> link = std::shared_ptr<Node>(nullptr);
    int start_index;
    int end_index;
    bool isSuffixNode = false;
    std::unique_ptr<ChildContainer> transitionNodes;

    Node(int startIndex, int endIndex, std::weak_ptr<Node> parent = std::shared_ptr<Node>(nullptr)) :
            parent(std::move(parent)), start_index(startIndex), end_index(endIndex),
            transitionNodes(std::make_unique<NodeOnMap>()) {}

};

