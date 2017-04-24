// circular_buffer.cpp
//
#include "monik/common/circular_buffer.h"

#if SDL_DEBUG
namespace sdl { namespace log { namespace {
    class unit_test {
    public:
        unit_test()
        {
            using T = circular_buffer<std::string, 10>;
            T buf;
            SDL_ASSERT(buf.empty());
            for (size_t i = 0; i < T::capacity(); ++i) {
                SDL_ASSERT(buf.size() == i);
                buf.push_front(std::to_string(i));
            }
            SDL_ASSERT(buf.full());
            SDL_ASSERT(buf.size() == T::capacity());
            SDL_ASSERT(buf.front() == "9");
            SDL_ASSERT(buf.back() == "0");
            for (size_t i = 0; i < T::capacity(); ++i) {
                buf.push_front(std::to_string(i + T::capacity()));
            }
            SDL_ASSERT(buf.size() == T::capacity());
            SDL_ASSERT(buf.front() == "19");
            SDL_ASSERT(buf.back() == "10");
            {
                auto p = buf.find_if([](std::string const & s) { 
                    return s.empty(); 
                });
                SDL_ASSERT(p == nullptr);
                p = buf.find_if([](std::string const & s) { 
                    return s == "15"; 
                });
                SDL_ASSERT(*p == "15");
            }
            buf.pop_back();
            SDL_ASSERT(buf.back() == "11");
            SDL_ASSERT(buf.size() == 9);
            buf.clear();
            SDL_ASSERT(buf.empty());
        }

    };
    static unit_test s_test;

}}} // sdl
#endif //#if SV_DEBUG
