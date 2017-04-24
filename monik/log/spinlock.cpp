// spinlock.cpp
//
#include "monik/log/spinlock.h"

namespace sdl { namespace log {

#if MONIK_DEBUG
namespace {
    struct unit_test {
        unit_test()
        {
            enum { N = 10 };
            std::atomic<uint32> counter;
            counter = 1;
            {
                readlock reader(counter);
                MONIK_ASSERT(reader.try_lock());
                MONIK_ASSERT(reader.commit());
                int x = 0;
                reader.work<int>(x, [](int & x){ 
                    x = 255; 
                });
                MONIK_ASSERT(255 == x);
            }
            for (size_t i = 0; i < N; ++i) {
                MONIK_ASSERT((i+1) == counter);
                writelock writer(counter);
                readlock reader(counter);
                MONIK_ASSERT(!reader.try_lock());
                MONIK_ASSERT(!reader.commit());
            }
            MONIK_ASSERT((N+1) == counter);
        }
    };
    static unit_test s_test;
}
#endif // MONIK_DEBUG

} // log
} // sdl