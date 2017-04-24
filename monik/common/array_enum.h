// array_enum.h
//
#pragma once
#ifndef __MONIK_COMMON_ARRAY_ENUM_H__
#define __MONIK_COMMON_ARRAY_ENUM_H__

#include "monik/common/common.h"

namespace monik {

template<typename T, size_t N, class enum_type>
struct array_enum
{
    using type = T;
    T elem[N];

    static constexpr size_t size() { return N; }
    T const & operator[](enum_type t) const {
        MONIK_ASSERT(size_t(t) < N);
        return elem[size_t(t)];
    }
    T & operator[](enum_type t) {
        MONIK_ASSERT(size_t(t) < N);
        return elem[size_t(t)];
    }
    T const * begin() const {
        return std::begin(elem);
    }
    T const * end() const {
        return std::end(elem);
    }
    T * begin() {
        return std::begin(elem);
    }
    T * end() {
        return std::end(elem);
    }
};

} // monik

#endif // __MONIK_COMMON_ARRAY_ENUM_H__

