#pragma once
#include "Node.cpp"

template<typename TAlphabet>
class NodeOnArray : public ChildContainer<TAlphabet, NodeOnArray<TAlphabet>> {
public:
    typedef typename seqan::ValueSize<TAlphabet>::Type TSize;
    static const TSize alphSize = seqan::ValueSize<TAlphabet>::VALUE;

    std::array<std::unique_ptr<Node<TAlphabet>>, alphSize> boys;

    struct ConcreteIterator : ChildContainer<TAlphabet, NodeOnArray<TAlphabet>>::Iterator {
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
