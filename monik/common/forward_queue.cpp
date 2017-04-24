// forward_queue.cpp
//
#include "monik/common/forward_queue.h"

#if MONIK_DEBUG
namespace sdl { namespace log { namespace {
    class unit_test {
    public:
        unit_test()
        {
            using T = forward_queue<int>;
            enum { N = 10 };
            T test;
            test.push_back(1);
            test.pop_front();
            test.push_front(2);
            test.push_back(3);
            test.pop_front();
            test.pop_front();
            MONIK_ASSERT(test.empty());
            for (int i = 0; i < N; ++i) {
                test.push_front(i);
                test.push_back(-i);
                MONIK_ASSERT(test.front() == i);
                MONIK_ASSERT(test.back() == -i);
                const T & t = test;
                MONIK_ASSERT(t.front() == i);
                MONIK_ASSERT(t.back() == -i);
            }
            MONIK_ASSERT(test.size() == N * 2);
            MONIK_ASSERT(!test.empty());
            T test2;
            test2.swap(test);
            MONIK_ASSERT(!test.size());
            MONIK_ASSERT(test2.size());
            test2.clear();
            MONIK_ASSERT(test2.empty());
        }
    };
    static unit_test s_test;

}}} // sdl
#endif //#if SV_DEBUG
