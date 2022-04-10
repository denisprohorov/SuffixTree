#include "modules/SuffixTree.h"

#define NOMINMAX
#include <Windows.h>
#include <seqan/seq_io.h>

#include <iostream>
#include "Tasks.h"
#include <memory>
#include <chrono>


constexpr int EXAMPLE_SIZE = 100000;

int main()
{
    seqan::CharString seqFileName = "./resources/fill.fastq";
    typedef seqan::Dna TAlphabet;

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
        readRecords(ids, seqs, quals, seqFileIn, EXAMPLE_SIZE);
    }
    catch (seqan::Exception const & e)
    {
        std::cout << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "size = " << length(ids) << std::endl;
    seqan::String<TAlphabet > dna;
    for (unsigned i = 0; i < std::min((int)length(seqs), EXAMPLE_SIZE); ++i) {
        seqan::append(dna, seqs[i]);
    }
    std::cout << "dna.size() = " << length(dna) << std::endl;

//    std::cout << dna << std::endl;

    auto start = std::chrono::system_clock::now();

    seqan::String<TAlphabet> second_string = "AACAGAGAGAGGAGAGAG";

    std:: cout << tasks::contains_index<TAlphabet>(dna, "GG");
//    std:: cout << tasks::max_common_substring<TAlphabet>(&dna, &second_string);
//    SuffixTree<seqan::Rna5> tree(dna);

    auto end = std::chrono::system_clock::now();
    std::cout << "time = " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000. << "s";
//    tree.print_all_suffix();

    return 0;
}
