#include "strategy/NodeOnArray.h"
#include "strategy/NodeOnList.h"
#include "strategy/NodeOnHashMap.h"
#include "strategy/NodeOnMap.h"
#include "modules/SuffixTree.h"

#define NOMINMAX
#include <Windows.h>
#include <seqan/seq_io.h>

#include <iostream>
#include "Tasks.h"
#include <memory>

#include <chrono>



template<typename TAlphabet>
void test(const seqan::String<TAlphabet> &base, const seqan::String<TAlphabet> &find_string, std::string description, float (*foo)(const seqan::String<TAlphabet> &, const seqan::String<TAlphabet> &)){
    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    float time = 0;

    start = std::chrono::system_clock::now();
    std::cout << description << std::endl;

    time = foo(base, find_string);

    end = std::chrono::system_clock::now();
    std::cout << "destructor time = " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000. - time << "s" << std::endl;
    std::cout << "total time = " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000.<< "s" << std::endl;
    std::cout << "----------------------------------\n" << std::endl;
}



constexpr int EXAMPLE_SIZE = 5000000;
//constexpr int EXAMPLE_SIZE = 500000;
//constexpr int EXAMPLE_SIZE = 10000;
//constexpr int EXAMPLE_SIZE = 10;
//constexpr int EXAMPLE_SIZE = 1;
constexpr int size = 26;

template <> struct seqan::ValueSize<char>
{
    typedef uint8_t Type;
    static const Type VALUE = size;
};



int main()
{

    seqan::CharString seqFileName = "./resources/amino.fasta";
//    seqan::CharString seqFileName = "./resources/fill.fastq";
//    typedef seqan::AminoAcid TAlphabet;
//    typedef seqan::Dna TAlphabet;
    typedef char TAlphabet;

    std::srand(0);
    std::string str;
    for (int i = 0; i < EXAMPLE_SIZE; ++i) {
        unsigned char symbol = std::rand() % size;
        str.push_back((TAlphabet)symbol);
    }

//    std::srand(45989);
    seqan::String<TAlphabet> max_str;
    for (int i = 0; i < EXAMPLE_SIZE; ++i) {
        unsigned char symbol = std::rand() % size;
        seqan::append(max_str, (TAlphabet) symbol);
    }

    seqan::SeqFileIn seqFileIn;
    if (!open(seqFileIn, toCString(seqFileName)))
    {
        std::cerr << "ERROR: Could not open the file.\n";
        return 1;
    }

    seqan::StringSet<seqan::CharString> ids;
    seqan::StringSet<seqan::CharString> seqs;
    seqan::StringSet<seqan::CharString> quals;

    try
    {
        readRecords(ids, seqs, quals, seqFileIn, EXAMPLE_SIZE);
    }
    catch (seqan::Exception const & e)
    {
        std::cout << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "size = " << length(ids) << std::endl;
    seqan::String<TAlphabet> dna(str);
//    seqan::String<TAlphabet> dna;
//    for (unsigned i = 0; i < length(seqs)  && seqan::length(dna) < EXAMPLE_SIZE; ++i) {
//        seqan::append(dna, seqs[i]);
//    }
    std::cout << "dna.size() = " << length(dna) << std::endl;

    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    float time = 0;
    std::cout << "all init" << std::endl;
//    std::getchar();

//    test(dna, dna, "NodeOnArray alloc 1", tasks::contains_index<TAlphabet, NodeOnArray, std::allocator>);
//    test(dna, max_str, "NodeOnArray alloc 2", tasks::max_common_substring<TAlphabet, NodeOnArray, std::allocator>);
//    test(dna, dna, "NodeOnArray my_alloc 1", tasks::contains_index<TAlphabet, NodeOnArray, my_allocator>);
//    test(dna, max_str, "NodeOnArray my_alloc 2", tasks::max_common_substring<TAlphabet, NodeOnArray, my_allocator>);

//    test(dna, dna, "NodeOnList alloc 1", tasks::contains_index<TAlphabet, NodeOnList, std::allocator>);
//    test(dna, max_str, "NodeOnList alloc 2", tasks::max_common_substring<TAlphabet, NodeOnList, std::allocator>);
//    test(dna, dna, "NodeOnList my_alloc 1", tasks::contains_index<TAlphabet, NodeOnList, my_allocator>);
    test(dna, max_str, "NodeOnList my_alloc 2", tasks::max_common_substring<TAlphabet, NodeOnList, my_allocator>);
//
//    test(dna, dna, "NodeOnMap alloc 1", tasks::contains_index<TAlphabet, NodeOnMap, std::allocator>);
//    test(dna, max_str, "NodeOnMap alloc 2", tasks::max_common_substring<TAlphabet, NodeOnMap, std::allocator>);
//    test(dna, dna, "NodeOnMap my_alloc 1", tasks::contains_index<TAlphabet, NodeOnMap, my_allocator>);
//    test(dna, max_str, "NodeOnMap my_alloc 2", tasks::max_common_substring<TAlphabet, NodeOnMap, my_allocator>);
//
//    test(dna, dna, "NodeOnHashMap alloc 1", tasks::contains_index<TAlphabet, NodeOnHashMap, std::allocator>);
//    test(dna, max_str, "NodeOnHashMap alloc 2", tasks::max_common_substring<TAlphabet, NodeOnHashMap, std::allocator>);
//    test(dna, dna, "NodeOnHashMap my_alloc 1", tasks::contains_index<TAlphabet, NodeOnHashMap, my_allocator>);
//    test(dna, max_str, "NodeOnHashMap my_alloc 2", tasks::max_common_substring<TAlphabet, NodeOnHashMap, my_allocator>);




    return 0;
}





