// format.cpp
//
#include "monik/common/format.h"

#if MONIK_DEBUG
namespace sdl { namespace {
    class unit_test {
    public:
        unit_test()
        {
            MONIK_ASSERT(trim_string("  ").empty());
            MONIK_ASSERT(trim_string("1") == "1");
            MONIK_ASSERT(trim_string(" 1") == "1");
            MONIK_ASSERT(trim_string("1 ") == "1");
            MONIK_ASSERT(trim_string(" 1 ") == "1");
            MONIK_ASSERT(trim_string(L"1") == L"1");
            MONIK_ASSERT(trim_string(L" 1") == L"1");
            MONIK_ASSERT(trim_string(L"1 ") == L"1");
            MONIK_ASSERT(trim_string(L" 1 ") == L"1");
        }
    };
    static unit_test s_test;
}} // sdl
#endif //#if SV_DEBUG

