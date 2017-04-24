// log_util.cpp
//
#include "monik/log/log_util.h"

#if MONIK_DEBUG
namespace monik { namespace log { namespace {

struct test_string {};

std::ostream & operator <<(std::ostream & out, test_string const & x) {
    out << "test_string";
    return out;
}

class unit_test {
public:
    unit_test()
    {
        static_assert(is_std_to_string<int>::value, "");
        static_assert(!is_std_to_string<unit_test>::value, "");
        static_assert(!is_std_to_string<test_string>::value, "");
        const std::string test = "apple";
        const auto s = log_util::to_string(1, "..", 2, "_", test, "_", test_string());
        MONIK_ASSERT(s == "1..2_apple_test_string");
    }
};
static unit_test s_test;
}}} // monik
#endif //#if SV_DEBUG




