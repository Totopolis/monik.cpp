// static.cpp
//
#include "monik/common/static.h"
#include "monik/common/locale.h"
#include "monik/common/format.h"

#if MONIK_DEBUG
namespace monik { namespace {
    class unit_test {
    public:
        unit_test() {
            static_assert(is_odd(1), "");
            static_assert(is_odd(-1), "");
            static_assert(!is_odd(2), "");
            static_assert(!is_odd(-2), "");
            static_assert(is_pod(0), "");
            {
                test_format_double();
                setlocale_t::auto_locale setl("Russian");
                test_format_double();
            }
            {
                MONIK_ASSERT(strcount("test %d,%d,%d", "%") == 3);
                char buf[256] = {};
                MONIK_ASSERT(!strcmp(format_s(buf, ""), ""));
                MONIK_ASSERT(!strcmp(format_s(buf, "test"), "test"));
                memset_zero(buf);
                MONIK_ASSERT(!strcmp(format_s(buf, "test %d", 1), "test 1"));
                memset_zero(buf);
                MONIK_ASSERT(!strcmp(format_s(buf, "test %d,%d,%d", 1, 2, 3), "test 1,2,3"));
                memset_zero(buf);
                MONIK_ASSERT(!strcmp(format_s(buf, "%s %d,%d,%d", "print", 1, 2, 3), "print 1,2,3"));
            }
        }
        void test_format_double()
        {
            char buf[64]={};
            auto old = debug::is_unit_test();
            debug::is_unit_test() = true;
            MONIK_ASSERT(std::string(format_double(buf, 1.23000123, 0)) == "1");
            MONIK_ASSERT(std::string(format_double(buf, 1.23000123, 100)) == "1.23000123");
            MONIK_ASSERT(std::string(format_double(buf, 1.23000123, 17)) == "1.23000123");
            MONIK_ASSERT(std::string(format_double(buf, 1.23000123, 10)) == "1.23000123");
            MONIK_ASSERT(std::string(format_double(buf, 1.23000123, 8)) == "1.23000123");
            MONIK_ASSERT(std::string(format_double(buf, 1.23000123, 6)) == "1.230001");
            MONIK_ASSERT(std::string(format_double(buf, 1.23000123, 5)) == "1.23");
            MONIK_ASSERT(std::string(format_double(buf, 1.00000123, 6)) == "1.000001");
            MONIK_ASSERT(std::string(format_double(buf, 1.00000123, 5)) == "1");
            MONIK_ASSERT(std::string(format_double(buf, 1.23000, 10)) == "1.23");
            MONIK_ASSERT(std::string(format_double(buf, -1.23000, 10)) == "-1.23");
            MONIK_ASSERT(std::string(format_double(buf, 0.000, 10)) == "0");
            MONIK_ASSERT(std::string(format_double(buf, -0.000, 10)) == "-0");
            debug::is_unit_test() = old;
        }
    };
    static unit_test s_test;
}} // monik
#endif //#if SV_DEBUG
