#pragma once

#define NOMINMAX
#include <Windows.h>

#include <seqan/seq_io.h>

template <typename TAlphabet>
class AlphTraits {
public:

    typedef typename seqan::ValueSize<TAlphabet>::Type TSize;
    static TSize alphSize;

};
template <typename TAlphabet>
typename AlphTraits<TAlphabet>::TSize AlphTraits<TAlphabet>::alphSize = seqan::ValueSize<TAlphabet>::VALUE;