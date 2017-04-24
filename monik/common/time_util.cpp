// time_util.cpp
//
#include "monik/common/time_util.h"

namespace sdl {

bool time_util::safe_gmtime(struct tm & dest, const time_t value) {
    memset_zero(dest);
#if defined(MONIK_OS_WIN32)
    const errno_t err = ::gmtime_s(&dest, &value);
    MONIK_ASSERT(!err);
    return !err;
#else
    struct tm * ptm = ::gmtime_r(&value, &dest);
    MONIK_ASSERT(ptm);
    return ptm != nullptr;
#endif
}

} // sdl



