#pragma once
#include "Node.cpp"


template<typename TAlphabet>
class NodeOnList : public ChildContainer<TAlphabet, NodeOnList<TAlphabet>> {
public:

    std::forward_list<std::pair<TAlphabet, std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>>>> boys;

    struct ConcreteIterator : ChildContainer<TAlphabet, NodeOnList<TAlphabet>>::Iterator {
    public:
        typedef typename std::forward_list<std::pair<TAlphabet, std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>>>>::iterator map_iterator;

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
//                                [&]( const std::pair<TAlphabet, std::unique_ptr<NodeDef<TAlphabet>>> &v ){ return v.first >= symbol; } );
                                [&]( const std::pair<TAlphabet, std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>>> &v ){ return v.first == symbol; } );
        if (it == boys.end()) {
            return false;
        }
        return it->first == symbol;
    }

    Node<TAlphabet, NodeOnList<TAlphabet>>* get_transition_node(const TAlphabet symbol) override {
        auto it = std::find_if(  boys.begin(),
                                 boys.end(),
                                 [&]( const std::pair<TAlphabet, std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>>> &v ){ return v.first == symbol; } );
        return it->second.get();
    }

    void create_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>> transition_node) override {
//        auto it = std::find_if(  boys.begin(),
//                                 boys.end(),
//                                 [&]( const std::pair<TAlphabet, std::unique_ptr<NodeDef<TAlphabet>>> &v ){ return v.first > symbol; } );
//        boys.insert(it, {symbol, std::move(transition_node)});
        boys.push_front({symbol, std::move(transition_node)});
    }

    std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>>
    replace_transition(const TAlphabet symbol, std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>> transition_node) override {
        auto it = std::find_if(  boys.begin(),
                                 boys.end(),
                                 [&]( const std::pair<TAlphabet, std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>>> &v ){ return symbol == v.first; } );
        std::unique_ptr<Node<TAlphabet, NodeOnList<TAlphabet>>> old_transition = std::move(it->second);
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