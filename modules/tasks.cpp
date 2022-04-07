//#include "tasks.h"
//
//int contains_index(const std::string &base, const std::string &find_string) {
//    SuffixTree tree(base);
//    State state(tree.getHead(), base);
//    for (int i = 0; i < find_string.size(); ++i) {
//        if(state.is_transition(find_string[i])){
//            state.go_by_symbol(find_string[i]);
//        }else{
//            return -1;
//        }
//    }
//    return state.activeNode->start_index + state.bias - find_string.size();
//}
//
//std::string max_common_substring(const std::string *min_str, const std::string *max_str) {
//    if(min_str->size() > max_str->size()){
//        std::swap(min_str, max_str);
//    }
//    int max_end_index = 0, max_length = 0, length = 0;
//    SuffixTree tree(*min_str);
//    State state(tree.getHead(), tree.getBaseStr());
//    for (int i = 0; i < max_str->size(); ++i) {
//        do {
//            length = std::max(0, length);
//            if (state.is_transition((*max_str)[i])) {
//                state.go_by_symbol((*max_str)[i]);
//                ++length;
//                if(length >= max_length) {
//                    max_end_index = state.activeNode->start_index + state.bias;
//                    max_length = length;
//                }
//                break;
//            }else{
//                state.go_by_link();
//                --length;
//            }
//        } while (length > -1);
//    }
//    int start_index = max_end_index - max_length;
//    std::string ans;
//    for (int i = start_index; i < max_end_index; ++i) {
//        ans.push_back((*min_str)[i]);
//    }
//    return ans;
//}