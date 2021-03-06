#pragma once

#include "Node.h"

#define NOMINMAX
#include <Windows.h>

#include <seqan/seq_io.h>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "State.h"
#include "AlphTraits.h"


template<typename TAlphabet, template <class alph, class allocator> class StoreStrategy = NodeOnMap,
    template <class T> class alloc = std::allocator, typename traits = AlphTraits<TAlphabet>>
class SuffixTree {
private:
    typedef typename traits::TSize TSize;
    TSize alphSize = traits::alphSize;
    typedef StoreStrategy<TAlphabet, alloc<char>> StoreStrategy;
    typedef Node<TAlphabet, StoreStrategy> NodeType;


    seqan::String<TAlphabet> base_str;
    std::unique_ptr<NodeType> head;
    alloc<char> allocator;

    void init() {
        State<TAlphabet, StoreStrategy, alloc> state(head.get(), base_str, &allocator);
        for (int i = 0; i < seqan::length(base_str); ++i) {
            bool is_empty_string = false;
            while (!state.is_transition(base_str[i])) {
                state.create_vertex(i);
                if (state.activeNode->end_index == 0) {
                    is_empty_string = true;
                    break;
                }
                state.go_by_link();
            }
            if (!is_empty_string) {
                state.go_by_symbol(base_str[i]);
            }
        }
        //form end vertexes
        while (true) {
            state.set_vertex_final();
            if (state.activeNode->end_index == 0) {
                break;
            }
            state.go_by_link();
        }
    }


public:
    SuffixTree(const seqan::String<TAlphabet> &baseStr, alloc<char> allocator = alloc<char>()) : base_str(baseStr), allocator(std::move(allocator)) {
        head = (std::make_unique<NodeType>(0, 0, &this->allocator));
        init();
        for (TSize i = 0; i < alphSize; ++i)
            std::cout << static_cast<unsigned>(i) << ',' << TAlphabet(i) << "  ";
        std::cout << std::endl;
    }

    void get_all_suffix(std::vector<std::string> &strs, NodeType *node = nullptr, std::string str = "") {
        if (node == nullptr) node = head.get();
        if (node->transitionNodes.is_leaf()) {
            strs.push_back(str);
            return;
        }
        if(node->isSuffixNode){
            strs.push_back(str);
        }

        for(auto &boy : node->transitionNodes){
            std::string tmp = str;
            for (int i = boy.second->start_index; i < boy.second->end_index; ++i) {
                tmp += base_str[i];
            }
            get_all_suffix(strs, boy.second.get(), tmp);
        }

    }

    void print_all_suffix() {
        std::vector<std::string> strs;
        this->get_all_suffix(strs);
        std::sort(strs.begin(), strs.end(),
                  [](const std::string &left, const std::string &right) { return left.size() > right.size(); });
        for (const std::string &str: strs) {
            std::cout << str << " - " << str.size() << '\n';
        }
    }

    long long edge_length(NodeType *node){
        long long summ = 0;
        for(auto& child : node->transitionNodes){
            summ += edge_length(&child);
        }
        return summ + node->end_index - node->start_index;
    }

    bool is_correct(){
        auto state = this->createState();
        for (auto ch : this->base_str) {
            state.go_by_symbol(ch);
        }
        for (int i = 0; i < seqan::length(this->base_str); ++i) {
            state.go_by_link();
        }

        return state.activeNode->parent == nullptr;
    }

    void print_all_info() {
//        std::cout << "edge length : " << edge_length(this->head.get()) << '\n';
//        std::cout <<-- "edge length average : " << edge_length(this->head.get()) / NodeDef<TAlphabet>::total_count << '\n';
//        std::cout << "is correct : " << std::boolalpha << is_correct() << '\n';
        std::cout << "vertex count by constructor: " << NodeType::total_count << std::endl;

    }

    NodeType* getHead() const { return head.get(); }

    const seqan::String<TAlphabet> &getBaseStr() const { return base_str; }

    State<TAlphabet, StoreStrategy, alloc> createState(){return State<TAlphabet, StoreStrategy, alloc>(head.get(), base_str, &allocator);}

    alloc<char> &getAllocator() {
        return allocator;
    }
};


template<typename TAlphabet, template <class alph, class allocator> class StoreStrategy, typename traits>
class SuffixTree<TAlphabet, StoreStrategy, my_allocator, traits> {

private:
    typedef typename traits::TSize TSize;
    TSize alphSize = traits::alphSize;
    typedef StoreStrategy<TAlphabet, my_allocator<char>> StoreStrategy;
    typedef Node<TAlphabet, StoreStrategy> NodeType;


    seqan::String<TAlphabet> base_str;
    std::unique_ptr<NodeType> head;
    my_allocator<char> allocator;

    void init() {
        State<TAlphabet, StoreStrategy, my_allocator> state(head.get(), base_str, &allocator);
        for (int i = 0; i < seqan::length(base_str); ++i) {
            bool is_empty_string = false;
            while (!state.is_transition(base_str[i])) {
                state.create_vertex(i);
                if (state.activeNode->end_index == 0) {
                    is_empty_string = true;
                    break;
                }
                state.go_by_link();
            }
            if (!is_empty_string) {
                state.go_by_symbol(base_str[i]);
            }
        }
        //form end vertexes
        while (true) {
            state.set_vertex_final();
            if (state.activeNode->end_index == 0) {
                break;
            }
            state.go_by_link();
        }
    }


public:
    SuffixTree(const seqan::String<TAlphabet> &baseStr, my_allocator<char> allocator = my_allocator<char>()) : base_str(baseStr), allocator(std::move(allocator)) {
        head = (std::make_unique<NodeType>(0, 0, &this->allocator));
        init();
        for (TSize i = 0; i < alphSize; ++i)
            std::cout << static_cast<unsigned>(i) << ',' << TAlphabet(i) << "  ";
        std::cout << std::endl;
    }

    ~SuffixTree() {
        head.release();
    }


    void get_all_suffix(std::vector<std::string> &strs, NodeType *node = nullptr, std::string str = "") {
        if (node == nullptr) node = head.get();
        if (node->transitionNodes.is_leaf()) {
            strs.push_back(str);
            return;
        }
        if(node->isSuffixNode){
            strs.push_back(str);
        }

        for(auto &boy : node->transitionNodes){
            std::string tmp = str;
            for (int i = boy.second->start_index; i < boy.second->end_index; ++i) {
                tmp += base_str[i];
            }
            get_all_suffix(strs, boy.second.get(), tmp);
        }

    }

    void print_all_suffix() {
        std::vector<std::string> strs;
        this->get_all_suffix(strs);
        std::sort(strs.begin(), strs.end(),
                  [](const std::string &left, const std::string &right) { return left.size() > right.size(); });
        for (const std::string &str: strs) {
            std::cout << str << " - " << str.size() << '\n';
        }
    }

    long long edge_length(NodeType *node){
        long long summ = 0;
        for(auto& child : node->transitionNodes){
            summ += edge_length(&child);
        }
        return summ + node->end_index - node->start_index;
    }

    bool is_correct(){
        auto state = this->createState();
        for (auto ch : this->base_str) {
            state.go_by_symbol(ch);
        }
        for (int i = 0; i < seqan::length(this->base_str); ++i) {
            state.go_by_link();
        }

        return state.activeNode->parent == nullptr;
    }

    void print_all_info() {
//        std::cout << "edge length : " << edge_length(this->head.get()) << '\n';
//        std::cout <<-- "edge length average : " << edge_length(this->head.get()) / NodeDef<TAlphabet>::total_count << '\n';
//        std::cout << "is correct : " << std::boolalpha << is_correct() << '\n';
        std::cout << "vertex count by constructor: " << NodeType::total_count << std::endl;

    }

    NodeType* getHead() const { return head.get(); }

    const seqan::String<TAlphabet> &getBaseStr() const { return base_str; }

    State<TAlphabet, StoreStrategy, my_allocator> createState(){return State<TAlphabet, StoreStrategy, my_allocator>(head.get(), base_str, &allocator);}

    my_allocator<char> &getAllocator() {
        return allocator;
    }
};

