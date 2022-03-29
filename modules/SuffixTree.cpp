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

void SuffixTree::get_all_suffix(std::vector<std::string> &strs, std::shared_ptr<Node> node, std::string str) {
    if (node == nullptr) node = head;
    if (node->boys.empty()) {
        strs.push_back(str);
        return;
    }
    for (auto boy: node->boys) {
        std::string tmp = str;
        for (int i = boy.second->start_index; i < boy.second->end_index; ++i) {
            tmp += base_str[i];
        }
        get_all_suffix(strs, boy.second, tmp);
    }
    return;
}
