#pragma once

#include "nodes/Node.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

const char END_SYMBOL = '$';

class SuffixTree {
private:
    std::string base_str;
    std::shared_ptr<Node> head;

    void init();

public:

    SuffixTree(const std::string &baseStr) : base_str(baseStr + END_SYMBOL), head(new Node(0, 0)) { init(); }

    void get_all_suffix(std::vector<std::string> &strs, std::shared_ptr<Node> node = nullptr, std::string str = "");

    void print_all_suffix(){
        std::vector<std::string> strs;
        this->get_all_suffix(strs);
        std::sort(strs.begin(), strs.end(), [](const std::string &left, const std::string &right){return left.size() > right.size();});
        for(const std::string &str : strs){
            std::cout << str << " - " << str.size() << '\n';
        }
    }

    const std::shared_ptr<Node> &getHead() const { return head; }

    const std::string &getBaseStr() const { return base_str; }
};

