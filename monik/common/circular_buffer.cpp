// circular_buffer.cpp
//
#include "monik/common/circular_buffer.h"

#if MONIK_DEBUG
namespace sdl { namespace log { namespace {
    class unit_test {
    public:
        unit_test()
        {
            using T = circular_buffer<std::string, 10>;
            T buf;
            MONIK_ASSERT(buf.empty());
            for (size_t i = 0; i < T::capacity(); ++i) {
                MONIK_ASSERT(buf.size() == i);
                buf.push_front(std::to_string(i));
            }
            MONIK_ASSERT(buf.full());
            MONIK_ASSERT(buf.size() == T::capacity());
            MONIK_ASSERT(buf.front() == "9");
            MONIK_ASSERT(buf.back() == "0");
            for (size_t i = 0; i < T::capacity(); ++i) {
                buf.push_front(std::to_string(i + T::capacity()));
            }
            MONIK_ASSERT(buf.size() == T::capacity());
            MONIK_ASSERT(buf.front() == "19");
            MONIK_ASSERT(buf.back() == "10");
            {
                auto p = buf.find_if([](std::string const & s) { 
                    return s.empty(); 
                });
                MONIK_ASSERT(p == nullptr);
                p = buf.find_if([](std::string const & s) { 
                    return s == "15"; 
                });
                MONIK_ASSERT(*p == "15");
            }
            buf.pop_back();
            MONIK_ASSERT(buf.back() == "11");
            MONIK_ASSERT(buf.size() == 9);
            buf.clear();
            MONIK_ASSERT(buf.empty());
        }

    };
    static unit_test s_test;

}}} // sdl
#endif //#if SV_DEBUG
