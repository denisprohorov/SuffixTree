#pragma once

#include "Node.cpp"

#define NOMINMAX
#include <Windows.h>

#include <seqan/seq_io.h>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "State.h"


const char END_SYMBOL = '$';

template<typename TAlphabet>
class SuffixTree {
private:
    typedef typename seqan::ValueSize<TAlphabet>::Type TSize;
    TSize alphSize = seqan::ValueSize<TAlphabet>::VALUE;

    seqan::String<TAlphabet> base_str;
    std::unique_ptr<Node<TAlphabet>> head;

    void init() {
        State<TAlphabet> state(head.get(), base_str);
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

    SuffixTree(const seqan::String<TAlphabet> &baseStr) : base_str(baseStr), head(std::make_unique<Node<TAlphabet>>(0, 0)) {
        init();
        for (TSize i = 0; i < alphSize; ++i)
            std::cout << static_cast<unsigned>(i) << ',' << TAlphabet(i) << "  ";
        std::cout << std::endl;
    }

    void get_all_suffix(std::vector<std::string> &strs, Node<TAlphabet> *node = nullptr, std::string str = "") {
        if (node == nullptr) node = head.get();
        if (node->transitionNodes->is_leaf()) {
            strs.push_back(str);
            return;
        }
        if(node->isSuffixNode){
            strs.push_back(str);
        }

        for(auto &boy : *node->transitionNodes){
            std::string tmp = str;
            for (int i = boy.start_index; i < boy.end_index; ++i) {
                tmp += base_str[i];
            }
            get_all_suffix(strs, &boy, tmp);
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

    long long edge_length(Node<TAlphabet> *node){
        long long summ = 0;
        for(auto& child : *node->transitionNodes){
            summ += edge_length(&child);
        }
        return summ + node->end_index - node->start_index;
    }

    bool is_correct(){
        State<TAlphabet> state(this->head.get(), this->base_str);
        for (auto ch : this->base_str) {
            state.go_by_symbol(ch);
        }
        for (int i = 0; i < seqan::length(this->base_str); ++i) {
            state.go_by_link();
        }

        return state.activeNode->parent == nullptr;
    }



    void print_all_info() {
        std::cout << "edge length : " << edge_length(this->head.get()) << '\n';
//        std::cout <<-- "edge length average : " << edge_length(this->head.get()) / Node<TAlphabet>::total_count << '\n';
        std::cout << "is correct : " << std::boolalpha << is_correct() << '\n';
        std::cout << "vertex count by constructor: " << Node<TAlphabet>::total_count << std::endl;

    }

    const Node<TAlphabet>* getHead() const { return head; }

    const seqan::String<TAlphabet> &getBaseStr() const { return base_str; }
};