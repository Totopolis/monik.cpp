// spinlock.cpp
//
#include "monik/log/spinlock.h"

namespace sdl { namespace log {

#if SDL_DEBUG
namespace {
    struct unit_test {
        unit_test()
        {
            enum { N = 10 };
            std::atomic<uint32> counter;
            counter = 1;
            {
                readlock reader(counter);
                SDL_ASSERT(reader.try_lock());
                SDL_ASSERT(reader.commit());
                int x = 0;
                reader.work<int>(x, [](int & x){ 
                    x = 255; 
                });
                SDL_ASSERT(255 == x);
            }
            for (size_t i = 0; i < N; ++i) {
                SDL_ASSERT((i+1) == counter);
                writelock writer(counter);
                readlock reader(counter);
                SDL_ASSERT(!reader.try_lock());
                SDL_ASSERT(!reader.commit());
            }
            SDL_ASSERT((N+1) == counter);
        }
    };
    static unit_test s_test;
}
#endif // SDL_DEBUG

} // log
} // sdl