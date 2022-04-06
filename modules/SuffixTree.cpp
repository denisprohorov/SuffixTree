#include "SuffixTree.h"
#include "State.h"


void SuffixTree::init() {
    std::shared_ptr<State> state(new State(head, base_str));
    for (int i = 0; i < base_str.length(); ++i) {
        bool is_empty_string = false;
        while (!state->is_transition(base_str[i])) {
            state->create_vertex(i);
            if (state->activeNode->end_index == 0) {
                is_empty_string = true;
                break;
            }
            state->go_by_link();
        }
        if (!is_empty_string) {
            state->go_by_symbol(base_str[i]);
        }
    }
}

void SuffixTree::get_all_suffix(std::vector<std::string> &strs, Node *node, std::string str) {
    if (node == nullptr) node = head.get();
    if (node->transitionNodes->is_leaf()) {
        strs.push_back(str);
        return;
    }

    for (auto it = node->transitionNodes->begin(); it->operator!=(node->transitionNodes->end().operator*()); it->operator++()) {
        auto &boy = it.get()->operator*();
        std::string tmp = str;
        for (int i = boy.start_index; i < boy.end_index; ++i) {
            tmp += base_str[i];
        }
        get_all_suffix(strs, &boy, tmp);
    }

}
