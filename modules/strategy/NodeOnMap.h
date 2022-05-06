#pragma once
#include "Node.cpp"

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