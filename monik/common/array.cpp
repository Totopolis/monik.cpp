// array.cpp
//
#include "monik/common/array.h"

#if MONIK_DEBUG
namespace sdl { namespace {
    class unit_test {
    public:
        unit_test()
        {
            if (1) {
                unique_vec<size_t> v1;
                MONIK_ASSERT(v1.empty());
                MONIK_ASSERT(!v1);
                MONIK_ASSERT(v1->empty());
                MONIK_ASSERT(!v1);
                using T = vector_buf<size_t, 16>;
                T test;
                MONIK_ASSERT(test.size() == 0);
                MONIK_ASSERT(test.capacity() == T::BUF_SIZE);
                enum { N = T::BUF_SIZE * 2 };
                for (size_t i = 0; i < T::BUF_SIZE; ++i) {
                    test.push_back(N - i);
                }
                MONIK_ASSERT(test.use_buf());
                MONIK_ASSERT(test.size() == T::BUF_SIZE);
                MONIK_ASSERT(test.capacity() == T::BUF_SIZE);
                std::sort(test.begin(), test.end());
                test.push_sorted(100);
                test.push_sorted(100);
                test.push_sorted(99);
                test.push_sorted(0);
                test.push_sorted(97);
                test.push_sorted(98);
                test.push_sorted(98);
                test.push_sorted(101);
                {
                    MONIK_ASSERT(!test.push_unique(test.front()));
                    MONIK_ASSERT(!test.push_unique(test.back()));
                    const auto v = test.back() + 1;
                    MONIK_ASSERT(test.push_unique(v));
                    MONIK_ASSERT(test.back() == v);
                }
                {
                    set_buf<size_t, 16> test_set;
                    for (auto const & v : test) {
                        test_set.insert(v);
                    }
                    test_set.insert(test.back());
                    test_set.insert(test.front());
                    MONIK_ASSERT(std::is_sorted(test_set.begin(), test_set.end()));
                    MONIK_ASSERT(algo::is_unique(test_set));
                }
                MONIK_ASSERT(test[0] < test[test.size() - 1]);
                MONIK_ASSERT(std::is_sorted(test.begin(), test.end()));
                for (size_t i = T::BUF_SIZE; i < N; ++i) {
                    test.push_back(N - i);
                }
                MONIK_ASSERT(!test.use_buf());
                std::sort(test.begin(), test.end());
                MONIK_ASSERT(test[0] < test[test.size() - 1]);
                MONIK_ASSERT(std::is_sorted(test.begin(), test.end()));
                test.fill_0();

            }
        }
    };
    static unit_test s_test;
}
} // sdl
#endif //#if SV_DEBUG


#if 0
template <class ForwardIterator>
  void rotate (ForwardIterator first, ForwardIterator middle,
               ForwardIterator last)
{
  ForwardIterator next = middle;
  while (first!=next)
  {
    swap (*first++,*next++);
    if (next==last) next=middle;
    else if (first==middle) middle=next;
  }
}
#endif