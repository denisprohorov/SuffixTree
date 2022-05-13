#pragma once

#include "SuffixTree.h"
#include "State.h"

namespace tasks {
    template<typename TAlphabet, template <class alph, class allocator> class Strategy, template <class T> class allocator>
    int contains_index(const seqan::String<TAlphabet> &base, const seqan::String<TAlphabet> &find_string) {
        auto start = std::chrono::system_clock::now();
        auto end = std::chrono::system_clock::now();

        start = std::chrono::system_clock::now();
        SuffixTree<TAlphabet, Strategy, allocator> tree(base);
        end = std::chrono::system_clock::now();
        std::cout << "build time = " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000. << "s" << std::endl;
        tree.print_all_info();

        start = std::chrono::system_clock::now();
        auto state = tree.createState();
        for (int i = 0; i < seqan::length(find_string); ++i) {
            if (state.is_transition(find_string[i])) {
                state.go_by_symbol(find_string[i]);
            } else {
                std::cout << "end" << std::endl;
                end = std::chrono::system_clock::now();
                std::cout << "find time = " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000. << "s" << std::endl;
                return -1;
            }
        }
        end = std::chrono::system_clock::now();
        std::cout << "find time = " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000. << "s" << std::endl;
//        std::getchar();
        return state.activeNode->start_index + state.bias - seqan::length(find_string);
    }

//    template<typename TAlphabet, class Strategy>
//    std::string max_common_substring(seqan::String<TAlphabet> &min_str, seqan::String<TAlphabet> &max_str) {
//        if (seqan::length(min_str) > seqan::length(max_str)) {
//            std::swap(min_str, max_str);
//        }
//        int max_end_index = 0, max_length = 0, length = 0;
//        SuffixTree<TAlphabet, Strategy> tree(*min_str);
//        State<TAlphabet, Strategy> state(tree.getHead(), tree.getBaseStr());
//        for (int i = 0; i < seqan::length(max_str); ++i) {
//            do {
//                length = std::max(0, length);
//                if (state.is_transition((max_str)[i])) {
//                    state.go_by_symbol((max_str)[i]);
//                    ++length;
//                    if (length >= max_length) {
//                        max_end_index = state.activeNode->start_index + state.bias;
//                        max_length = length;
//                    }
//                    break;
//                } else {
//                    state.go_by_link();
//                    --length;
//                }
//            } while (length > -1);
//        }
//        int start_index = max_end_index - max_length;
//        std::string ans;
//        for (int i = start_index; i < max_end_index; ++i) {
//            ans.push_back((min_str)[i]);
//        }
//        return ans;
//    }


}



