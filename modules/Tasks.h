#pragma once

#include "SuffixTree.h"
#include "State.h"

namespace tasks {
    template<typename TAlphabet, template <class alph, class allocator> class Strategy, template <class T> class allocator>
    float contains_index(const seqan::String<TAlphabet> &base, const seqan::String<TAlphabet> &find_string) {
        auto start = std::chrono::system_clock::now();
        auto end = std::chrono::system_clock::now();

        start = std::chrono::system_clock::now();
        SuffixTree<TAlphabet, Strategy, allocator> tree(base);
        end = std::chrono::system_clock::now();
        tree.print_all_info();
        float buildTime = (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000.;
        std::cout << "build time = " << buildTime << "s" << std::endl;

        start = std::chrono::system_clock::now();

        for(int j = 0; j < 1500; ++j){
            auto state = tree.createState();
            for (int i = j; i < seqan::length(find_string); ++i) {
                if (state.is_transition(find_string[i])) {
                    state.go_by_symbol(find_string[i]);
                } else {
                    std::cout << "end" << std::endl;
                    return -1;
                }
            }
        }

        end = std::chrono::system_clock::now();
        float findTime = (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000.;
        std::cout << "find time = " << findTime << "s" << std::endl;
        std::cout << "time without destructor" << buildTime + findTime << "s" << std::endl;
        return buildTime + findTime;
    }

    template<typename TAlphabet, template <class alph, class allocator> class Strategy, template <class T> class allocator>
    float max_common_substring(const seqan::String<TAlphabet> &min_str, const seqan::String<TAlphabet> &max_str) {
        int max_end_index = 0, max_length = 0;

        auto start = std::chrono::system_clock::now();
        auto end = std::chrono::system_clock::now();
        float findTime = 0;
        SuffixTree<TAlphabet, Strategy, allocator> tree(min_str);
        end = std::chrono::system_clock::now();
        tree.print_all_info();
        float buildTime = (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000.;
        std::cout << "build time = " << buildTime << "s" << std::endl;

        start = std::chrono::system_clock::now();
        for (int j = 0; j < 1; ++j) {
            auto state = tree.createState();
            int length = 0;
            for (int i = j; i < seqan::length(max_str); ++i) {
                TAlphabet symbol = max_str[i];
                do {
                    length = std::max(0, length);
                    if (state.is_transition(symbol)) {
                        state.go_by_symbol(symbol);
                        ++length;
                        if (length >= max_length) {
                            max_end_index = state.activeNode->start_index + state.bias;
                            max_length = length;
                        }
                        break;
                    } else {
                        state.smart_go_by_link();
                        --length;
                    }
                } while (length > -1);
            }
        }
        end = std::chrono::system_clock::now();
        findTime += (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000.;
        std::cout << "max length = " << max_length << std::endl;
        std::cout << "find time = " << findTime << "s" << std::endl;
        return buildTime + findTime;
    }


}



