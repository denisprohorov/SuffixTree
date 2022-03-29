#include "modules/SuffixTree.h"

#define NOMINMAX
#include <Windows.h>
#include <iostream>
#include <memory>
#include <seqan/seq_io.h>



int main() {

    const std::string str = "ilyailyaillayaidlsaif";

    SuffixTree tree(str);
    tree.print_all_suffix();
    return 0;
}

using namespace seqan;

//int main()
//{
//    CharString seqFileName = "fill.fastq";
//
//    SeqFileIn seqFileIn;
//    if (!open(seqFileIn, toCString(seqFileName)))
//    {
//        std::cerr << "ERROR: Could not open the file.\n";
//        return 1;
//    }
//
//    StringSet<CharString> ids;
//    StringSet<Dna5String> seqs;
//    StringSet<CharString> quals;
//
//    try
//    {
//        readRecords(ids, seqs, quals, seqFileIn);
//    }
//    catch (Exception const & e)
//    {
//        std::cout << "ERROR: " << e.what() << std::endl;
//        return 1;
//    }
//
//    std::cout << "size = " << length(ids) << std::endl;
//    for (unsigned i = 0; i < length(ids); ++i)
//        std::cout << "seqs " + std::to_string(i) + " : " << seqs[i] << "\n\n";
//
//    return 0;
//}
