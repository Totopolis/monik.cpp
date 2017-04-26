// console_log.cpp
//
#include "monik/log/console_log.h"
#include "monik/log/log_thread.h"

namespace monik { namespace log {

class console_log::data_type : public log_thread {
public:
    explicit data_type(buf_size_t);
private:
    static void write(const message_with_severity & s) {
        std::cout << s.m_message << std::endl;
    }
    static void overflow(const message_with_severity & s) {
        MONIK_TRACE(s.m_message);
        MONIK_ASSERT(!"overflow");
    }
};

console_log::data_type::data_type(const buf_size_t max_size)
    : log_thread(std::launch::async, max_size, second_t(0),
        log_thread::overflow_policy::refuse,
        log_thread::write_exception_policy::ignore,
        [](message_with_severity const & s){ 
            data_type::write(s);
        },
        [](message_with_severity const & s){
            data_type::overflow(s);
        }
    )
{
    MONIK_ASSERT(this->running());
}

//--------------------------------------------------------

console_log::console_log(buf_size_t max_size)
    : m_data(new data_type(max_size))
{
}

console_log::~console_log()
{
}

bool console_log::empty() const 
{
    return m_data->empty();
}

void console_log::log(message_with_severity && s)
{
    m_data->log(std::move(s));
}

} // log
} // monik

#if MONIK_DEBUG
namespace monik { namespace log { namespace {
    class unit_test {
    public:
        unit_test()
        {
            if (0) {
                console_log test(megabyte<1>::value);
                for (size_t i = 0; i < 10; ++i) {
                    test.log(message_with_severity(severity::verbose, "console_log: " + std::to_string(i)));
                }
                while (!test.empty()) {
                    sleep_for_milliseconds(100);
                }
            }
        }
    };
    static unit_test s_test;

}}} // monik
#endif //#if SV_DEBUG
