#include "modules/SuffixTree.h"

#define NOMINMAX
#include <Windows.h>
#include <iostream>
#include <seqan/seq_io.h>
#include <memory>
#include <chrono>


constexpr int EXAMPLE_SIZE = 10;

//TODO add templates for alphabet
//TODO add different realizations

//template class SuffixTree<seqan::Dna>;
int main()
{
    seqan::CharString seqFileName = "fill.fastq";

    showAllLettersOfMyAlphabet(seqan::Dna());
//    return 0;

    seqan::SeqFileIn seqFileIn;
    if (!open(seqFileIn, toCString(seqFileName)))
    {
        std::cerr << "ERROR: Could not open the file.\n";
        return 1;
    }

    seqan::StringSet<seqan::CharString> ids;
    seqan::StringSet<seqan::Dna5String> seqs;
    seqan::StringSet<seqan::CharString> quals;

    try
    {
        readRecords(ids, seqs, quals, seqFileIn, 10);
    }
    catch (seqan::Exception const & e)
    {
        std::cout << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "size = " << length(ids) << std::endl;
    seqan::CharString str;
    for (unsigned i = 0; i < length(ids); ++i) {
        seqan::append(str, seqs[i]);
    }

    std::string dna;
    for(int i = 0; i < EXAMPLE_SIZE; ++i) {
        dna.push_back(str[i]);
    }
    std::cout << dna << std::endl;
//    auto start = std::chrono::system_clock::now();

    SuffixTree<seqan::Rna5> tree(dna);

//    auto end = std::chrono::system_clock::now();
//    std::cout << "time = " << (end - start).count();
    tree.print_all_suffix();

    return 0;
}
