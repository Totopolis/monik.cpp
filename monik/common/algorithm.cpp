// algorithm.cpp
//
#include "monik/common/algorithm.h"

namespace monik { namespace algo { namespace {

static_assert('a' == 97, "");
static_assert('z' == 122, "");
static_assert('A' == 65, "");
static_assert('Z' == 90, "");

inline constexpr int char_tolower(const int x) { // ASCII char
    return ((x >= 'A') && (x <= 'Z')) ? (x + 'a' - 'A') : x;
}

} // namespace

bool iequal_range(const char * first1, const char * const last1, const char * first2) {
    MONIK_ASSERT(first1 && last1 && first2);
    MONIK_ASSERT(!(last1 < first1));
    for (; first1 != last1; ++first1, ++first2) {
        MONIK_ASSERT(*first1);
        MONIK_ASSERT(*first2);
        if (char_tolower(*first1) != char_tolower(*first2)) {
            return false;
        }
    }
    return true;
}

std::vector<std::string> 
lex::split(std::string const & s, const char delim)
{
    std::vector<std::string> result;
    if (!s.empty()) {
        size_t start = 0;
        while (start < s.size()) {
            size_t pos = s.find(delim, start);
            if (pos != std::string::npos) {
                result.emplace_back(s.substr(start, pos - start));
                start = pos + 1;
            }
            else {
                result.emplace_back(s.substr(start));
                return result;
            }
        }
    }
    return result;
}

} // algo
} // monik

#if MONIK_DEBUG
namespace monik { namespace algo { namespace {
    class unit_test {
    public:
        unit_test()
        {
            bool unit_test_done = false;
            MONIK_UTILITY_SCOPE_EXIT([&unit_test_done](){
                MONIK_ASSERT(unit_test_done);
            });
            MONIK_UTILITY_ASSERT_EXIT([&unit_test_done](){
                return unit_test_done;
            });
            MONIK_DEBUG_TIMER_SEC(test_timer, "seconds = ");
            MONIK_ASSERT(number_of_1(0x1) == 1);
            MONIK_ASSERT(number_of_1(0x101) == 2);
            MONIK_ASSERT(number_of_1(0x111) == 3);
            MONIK_ASSERT(number_of_1<uint8>(0x80) == 1);
            MONIK_ASSERT(number_of_1<uint8>(0x88) == 2);
            MONIK_ASSERT(number_of_1<int16>(-1) == 16);
            MONIK_ASSERT(number_of_1<int32>(-1) == 32);
            MONIK_ASSERT(number_of_1<int64>(-1) == 64);
            {
                constexpr const char s1[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                constexpr const char s2[] = "abcdefghijklmnopqrstuvwxyz";
                MONIK_ASSERT(iequal_range(s1, s1 + count_of(s1) - 1, s2));
                MONIK_ASSERT(iequal(s1, s2));
                MONIK_ASSERT(iequal_n(s1, s2, count_of(s2) - 1));
                static_assert(char_tolower('A') == 'a', "");
                static_assert(char_tolower('Z') == 'z', "");
                static_assert(char_tolower('G') == 'g', "");
                static_assert(char_tolower('1') == '1', "");
            }
            {
                constexpr const char s1[] = "/GeocodeOne";        
                constexpr const char s2[] = "/GeocodeOne?";        
                const char * const p1 = s1;
                const char * const p2 = s2;
                MONIK_ASSERT(!iequal(s1, s2));
                MONIK_ASSERT(!iequal_n(s1, s2));
                MONIK_ASSERT(iequal_n(s2, s1));
                //
                MONIK_ASSERT(!iequal(p1, p2));
                MONIK_ASSERT(!iequal_n(p1, p2));
                MONIK_ASSERT(iequal_n(p2, p1));
                //
                MONIK_ASSERT(iequal("/geocodeONE", s1));
                MONIK_ASSERT(iequal_n("/geocodeONE?1,2,3", s2, count_of(s2) - 1));
                MONIK_ASSERT(iequal_n("/geocodeONE?1,2,3", s2));
                MONIK_ASSERT(!iequal_n("/geocodeONE", s2, count_of(s2) - 1));
                MONIK_ASSERT(!iequal_n("/geocodeONE", s2));
                MONIK_ASSERT(!iequal("/geocodeONE?1,2,3", s2));
                //
                MONIK_ASSERT(iequal(std::string("/geocodeONE"), s1));
                MONIK_ASSERT(iequal_n(std::string("/geocodeONE?1,2,3"), s2, count_of(s2) - 1));
                MONIK_ASSERT(iequal_n(std::string("/geocodeONE?1,2,3"), s2));
                MONIK_ASSERT(!iequal_n(std::string("/geocodeONE"), s2, count_of(s2) - 1));
                MONIK_ASSERT(!iequal_n(std::string("/geocodeONE"), s2));
                MONIK_ASSERT(!iequal(std::string("/geocodeONE?1,2,3"), s2));
                //
                MONIK_ASSERT(iequal(std::string("/geocodeONE"), p1));
                MONIK_ASSERT(iequal_n(std::string("/geocodeONE?1,2,3"), p2));
                MONIK_ASSERT(!iequal_n(std::string("/geocodeONE"), p2));
                MONIK_ASSERT(!iequal(std::string("/geocodeONE?1,2,3"), p2));
            }
            {
                MONIK_ASSERT(lex::split("", '.').empty());
                {
                    const auto v = lex::split("1", '.');
                    MONIK_ASSERT(v.size() == 1);
                    MONIK_ASSERT(v[0] == "1");
                }
                {
                    const auto v = lex::split("1.", '.');
                    MONIK_ASSERT(v.size() == 1);
                    MONIK_ASSERT(v[0] == "1");
                }
                {
                    const auto v = lex::split("1..", '.');
                    MONIK_ASSERT(v.size() == 2);
                    MONIK_ASSERT(v[0] == "1");
                    MONIK_ASSERT(v[1].empty());
                }
                {
                    const auto v = lex::split("1..2", '.');
                    MONIK_ASSERT(v.size() == 3);
                    MONIK_ASSERT(v[0] == "1");
                    MONIK_ASSERT(v[1].empty());
                    MONIK_ASSERT(v[2] == "2");
                }
                {
                    const auto v = lex::split("1.2.3", '.');
                    MONIK_ASSERT(v.size() == 3);
                    MONIK_ASSERT(v[0] == "1");
                    MONIK_ASSERT(v[1] == "2");
                    MONIK_ASSERT(v[2] == "3");
                }
            }
            unit_test_done = true;
        }
    };
    static unit_test s_test;
}
} // algo
} // monik
#endif //#if SV_DEBUG

