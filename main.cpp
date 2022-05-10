#include "strategy/NodeOnArray.h"
#include "strategy/NodeOnList.h"
#include "strategy/NodeOnHashMap.h"
#include "strategy/NodeOnMap.h"
#include "strategy/MixedStrategy.h"
#include "modules/SuffixTree.h"

#define NOMINMAX
#include <Windows.h>
#include <seqan/seq_io.h>

#include <iostream>
#include "Tasks.h"
#include <memory>

#include <chrono>


//constexpr size_t DEBUG_SIZE = 100;
//int max[DEBUG_SIZE];
//const long long MAX_MEM = 7e8;
//long long mpos = 0;
//char mem[MAX_MEM];
//void * operator  new ( size_t n ) {
//    if(n > DEBUG_SIZE) {std::cerr << " n > DEBUG_SIZE in operator new"; throw("can't allocate memory");}
//    max[n]++;
//    char *res = mem + mpos;
//    mpos += n;
//    assert(mpos <= MAX_MEM);
//    return (void *)res;
//}
//inline void operator delete ( void * ) { }

constexpr int EXAMPLE_SIZE = 10000000;

//int main(){
//    std::map<int, char, std::less<>,  my_allocator<long long>> v1;
//    v1[10] = (53);
//
//    for(auto val : v1){
//        std::cout << val.second << " ";
//    }
//}

int main()
{
    seqan::CharString seqFileName = "./resources/amino.fasta";
//    seqan::CharString seqFileName = "./resources/fill.fastq";
    typedef seqan::AminoAcid TAlphabet;
//    typedef seqan::Dna TAlphabet;
//    typedef char TAlphabet;

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
    seqan::String<TAlphabet > dna;
    for (unsigned i = 0; i < length(seqs)  && seqan::length(dna) < EXAMPLE_SIZE; ++i) {
        seqan::append(dna, seqs[i]);
    }
    std::cout << "dna.size() = " << length(dna) << std::endl;

//    std::cout << dna << std::endl;

    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();

    seqan::String<TAlphabet> second_string = "AACAGAGAGAGGAGAGAG";
    start = std::chrono::system_clock::now();
    std:: cout << tasks::contains_index<TAlphabet, NodeOnArray<TAlphabet>>(dna, dna);
    end = std::chrono::system_clock::now();
    std::cout << "time = " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000. << "s" << std::endl;

    start = std::chrono::system_clock::now();
    std:: cout << tasks::contains_index<TAlphabet, NodeOnMap<TAlphabet>>(dna, dna);
    end = std::chrono::system_clock::now();
    std::cout << "time = " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000. << "s" << std::endl;

    std:: cout << tasks::contains_index<TAlphabet, NodeOnHashMap<TAlphabet>>(dna, dna);
    end = std::chrono::system_clock::now();
    std::cout << "time = " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000. << "s" << std::endl;

    start = std::chrono::system_clock::now();
    std:: cout << tasks::contains_index<TAlphabet, NodeOnList<TAlphabet>>(dna, dna);
    end = std::chrono::system_clock::now();
    std::cout << "time = " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000. << "s" << std::endl;


//    SuffixTree<TAlphabet, NodeOnArray<TAlphabet>> tree(dna);
//    SuffixTree<TAlphabet, NodeOnMap<TAlphabet>> tree(dna);
//    SuffixTree<TAlphabet, NodeOnHashMap<TAlphabet>> tree(dna);
//    SuffixTree<TAlphabet, NodeOnList<TAlphabet>> tree2(dna);

//    std::cout << mpos / 1000000 << " MB" << std::endl;

//    std::cout << "time = " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000. << "s" << std::endl;
//    std::cout << mpos / 1000000 << " MB" << std::endl;

//    tree.print_all_info();
//    tree2.print_all_info();
//    std::cout << mpos / 1000000 << " MB" << std::endl;
//
//    for(int i = 0; i < DEBUG_SIZE; ++i){
//        if(max[i] != 0){
//            std::cout << i << " - " << max[i] << std::endl;
//        }
//    }


//    tree.print_all_suffix();

    std::cin.get();


    return 0;
}





