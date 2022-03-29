#pragma once

#include <map>
#include <memory>

struct Node {
public:
    std::weak_ptr<Node> parent;
    std::weak_ptr<Node> link = std::shared_ptr<Node>(nullptr);
    int start_index;
    int end_index;

    Node(int startIndex, int endIndex) : start_index(startIndex), end_index(endIndex), parent(std::shared_ptr<Node>(nullptr)) {}

    Node(const std::weak_ptr<Node> &parent, int startIndex, int endIndex) : parent(parent), start_index(startIndex),
                                                                            end_index(endIndex) {}

};

struct Node2 {
public:
    std::map<const char, std::shared_ptr<Node>> boys;
    std::weak_ptr<Node> parent;
    std::weak_ptr<Node> link = std::shared_ptr<Node>(nullptr);
    int start_index;
    int end_index;

    Node2(int startIndex, int endIndex) : start_index(startIndex), end_index(endIndex), parent(std::shared_ptr<Node>(nullptr)) {}

    Node2(const std::weak_ptr<Node> &parent, int startIndex, int endIndex) : parent(parent), start_index(startIndex),
                                                                            end_index(endIndex) {}

};

