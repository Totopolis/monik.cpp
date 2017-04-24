// format.cpp
//
#include "monik/common/format.h"

#if SDL_DEBUG
namespace sdl { namespace {
    class unit_test {
    public:
        unit_test()
        {
            SDL_ASSERT(trim_string("  ").empty());
            SDL_ASSERT(trim_string("1") == "1");
            SDL_ASSERT(trim_string(" 1") == "1");
            SDL_ASSERT(trim_string("1 ") == "1");
            SDL_ASSERT(trim_string(" 1 ") == "1");
            SDL_ASSERT(trim_string(L"1") == L"1");
            SDL_ASSERT(trim_string(L" 1") == L"1");
            SDL_ASSERT(trim_string(L"1 ") == L"1");
            SDL_ASSERT(trim_string(L" 1 ") == L"1");
        }
    };
    static unit_test s_test;
}} // sdl
#endif //#if SV_DEBUG

