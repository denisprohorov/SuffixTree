#include "modules/SuffixTree.h"

#define NOMINMAX
#include <Windows.h>
#include <seqan/seq_io.h>

#include <iostream>
#include "Tasks.h"
#include <memory>
#include <chrono>

int max[73];
const size_t MAX_MEM = 7e8;
size_t mpos = 0;
char mem[MAX_MEM];
//char *mem = new char[MAX_MEM];
//char *mem;
/*
x64
16 - 1065653
32 - 4
40 - 1065634
56 - 2131267
72 - 1065634
x32
 8 - 1065653
20 - 1065634
28 - 2131267
32 - 4
44 - 1065634*/
void * operator new ( size_t n ) {
    max[n]++;
    char *res = mem + mpos;
    mpos += n;
    assert(mpos <= MAX_MEM);
    return (void *)res;
}
inline void operator delete ( void * ) { }



constexpr int EXAMPLE_SIZE = 1000000;

int main()
{
//    mem = new char[MAX_MEM];
    seqan::CharString seqFileName = "./resources/amino.fasta";
//    seqan::CharString seqFileName = "./resources/fill.fastq";
    typedef seqan::AminoAcid TAlphabet;
//    typedef seqan::Dna TAlphabet;

    seqan::SeqFileIn seqFileIn;
    if (!open(seqFileIn, toCString(seqFileName)))
    {
        std::cerr << "ERROR: Could not open the file.\n";
        return 1;
    }

    seqan::StringSet<seqan::CharString> ids;
//    seqan::StringSet<seqan::Dna5String> seqs;
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

    seqan::String<TAlphabet> second_string = "AACAGAGAGAGGAGAGAG";

//    std:: cout << tasks::max_common_substring<TAlphabet>(&dna, &second_string);
//    std:: cout << tasks::contains_index<TAlphabet>(dna, "GG") << std::endl;
    SuffixTree<seqan::Rna5> tree(dna);
    tree.print_all_info();

    auto end = std::chrono::system_clock::now();
    std::cout << "time = " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000. << "s" << std::endl;

    for(int i = 0; i < 73; ++i){
        if(max[i] != 0){
            std::cout << i << " - " << max[i] << std::endl;
        }
    }

    std::cin.get();
//    tree.print_all_suffix();


    return 0;
}
