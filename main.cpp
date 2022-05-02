#include "modules/SuffixTree.h"

#define NOMINMAX
#include <Windows.h>
#include <seqan/seq_io.h>

#include <iostream>
#include "Tasks.h"
#include <memory>

#include <chrono>
constexpr size_t DEBUG_SIZE = 3000;
int max[DEBUG_SIZE];
const long long MAX_MEM = 10e8;
long long mpos = 0;
char mem[MAX_MEM];
//void * operator  new ( size_t n ) {
//    if(n > DEBUG_SIZE) {std::cerr << " n > DEBUG_SIZE in operator new"; throw("can't allocate memory");}
//    max[n]++;
//    char *res = mem + mpos;
//    mpos += n;
//    assert(mpos <= MAX_MEM);
//    return (void *)res;
//}
//inline void operator delete ( void * ) { }


constexpr int EXAMPLE_SIZE = 1000000;

int main()
{
//    mem = new char[MAX_MEM];
    seqan::CharString seqFileName = "./resources/amino.fasta";
//    seqan::CharString seqFileName = "./resources/fill.fastq";
//    typedef seqan::AminoAcid TAlphabet;
//    typedef seqan::Dna TAlphabet;
    typedef char TAlphabet;

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
    SuffixTree<TAlphabet> tree(dna);
    std::cout << mpos / 1000000 << " MB" << std::endl;

    auto end = std::chrono::system_clock::now();
    std::cout << "time = " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000. << "s" << std::endl;
    std::cout << mpos / 1000000 << " MB" << std::endl;

    tree.print_all_info();
    std::cout << mpos / 1000000 << " MB" << std::endl;

    for(int i = 0; i < DEBUG_SIZE; ++i){
        if(max[i] != 0){
            std::cout << i << " - " << max[i] << std::endl;
        }
    }

    std::cout << mpos / 1000000 << " MB" << std::endl;

//    tree.print_all_suffix();
//    std::cout << sizeof (tree.getHead()->transitionNodes.get());

    std::cin.get();


    return 0;
}
