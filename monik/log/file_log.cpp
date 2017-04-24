// file_log.cpp
//
#include "monik/log/file_log.h"
#include "monik/log/log_thread.h"
#include "monik/log/log_util.h"
#include <fstream>

namespace sdl { namespace log {

class file_log::data_type : noncopyable {
    using pair_string = std::pair<std::string, std::string>;
    using file_log_error = sdl_exception_t<file_log>;
    using unique_ofstream = std::unique_ptr<std::ofstream>;
public:
    data_type(log_size_t, buf_size_t, const std::string &, const std::string &);
    ~data_type();
    bool empty() const {
        return m_thread->empty();
    }
    void log(message_with_severity && s) {
        m_thread->log(std::move(s));
    }
private:
    enum class open_mode { append, truncate };
    static size_t file_size(const std::string & fname);
    void create_log(const std::string & fname, open_mode); // throw if error
    bool wait_timeout_seconds(size_t);
    void write(const message_with_severity & s);       // called in thread
    void overflow(const message_with_severity & s);    // called in thread
    void check_log_size();
private:
    const size_t max_file_size_bytes;
    const pair_string m_log_names;
    std::atomic<bool> m_log_second;
    size_t m_log_size = 0;          // changed in thread only
    unique_ofstream m_log_file;     // changed in thread only
    std::unique_ptr<log_thread> m_thread;
};

file_log::data_type::data_type(
    const log_size_t s1, 
    const buf_size_t s2,
    const std::string & path1,
    const std::string & path2)
    : max_file_size_bytes(s1.value())
    , m_log_names(path1, path2)
    , m_log_second(false)
{
    SDL_TRACE("file_log1: ", m_log_names.first);
    SDL_TRACE("file_log2: ", m_log_names.second);
    SDL_ASSERT(!m_log_names.first.empty());
    SDL_ASSERT(m_log_names.second.empty() || (m_log_names.first != m_log_names.second));
    create_log(m_log_names.first, open_mode::append); // throw if error
    m_thread.reset(new log_thread(std::launch::async, s2, second_t(0),
        log_thread::overflow_policy::pop_front,
        log_thread::write_exception_policy::ignore,
        [this](message_with_severity const & s) {
            this->write(s);
        },
        [this](message_with_severity const & s) {
            this->overflow(s);
        }
    ));
    SDL_ASSERT(m_thread->running());
}

file_log::data_type::~data_type()
{
    wait_timeout_seconds(30);
    m_thread.reset();
}

bool file_log::data_type::wait_timeout_seconds(size_t seconds)
{
    while (seconds--) {
        if (this->empty())
            return true;
        sleep_for_milliseconds(1000);
    }
    SDL_WARNING(0);
    return false;
}

size_t file_log::data_type::file_size(const std::string & fname)
{
    std::ifstream file(fname, std::ifstream::ate);
    if (file) {
        return file.tellg();
    }
    return 0;
}

void file_log::data_type::create_log(const std::string & fname, open_mode const mode)
{
    if (mode == open_mode::truncate) {
        m_log_size = 0;
        m_log_file = std::make_unique<std::ofstream>(fname, std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);
    }
    else {
        m_log_size = file_size(fname);
        m_log_file = std::make_unique<std::ofstream>(fname, std::ofstream::binary | std::ofstream::out | std::ofstream::app);
    }
    throw_error_if<file_log_error>(!m_log_file->is_open(), "cannot open log file");
}

void file_log::data_type::check_log_size()
{
    if (max_file_size_bytes && (max_file_size_bytes < m_log_size)) {
        if (m_log_names.second.empty()) {
            create_log(m_log_names.first, open_mode::truncate);
        }
        else {
            m_log_second = !m_log_second;
            create_log(m_log_second ? m_log_names.second : m_log_names.first,
                open_mode::truncate);
        }
    }
}

void file_log::data_type::write(const message_with_severity & s)
{
    (*m_log_file) << s.m_message << std::endl;
    m_log_size += s.m_message.size() + 1;
    check_log_size();
}

void file_log::data_type::overflow(const message_with_severity & s)
{
    SDL_TRACE(s.m_message);
    SDL_ASSERT(!"overflow");
}

//--------------------------------------------------------

file_log::file_log(log_size_t s1, buf_size_t s2,
                   const std::string & path1, 
                   const std::string & path2)
    : m_data(new data_type(s1, s2, path1, path2))
{
}

file_log::~file_log()
{
}

bool file_log::empty() const
{
    return m_data->empty();
}

void file_log::log(message_with_severity && s)
{
    m_data->log(std::move(s));
}

} // log
} // sdl

#if SDL_DEBUG
namespace sdl { namespace log { namespace {
    class unit_test {
        size_t count = 0;
        void test_trace(file_log & test) {
            auto s = log_util::to_string("file_log_", std::to_string(count++), ",", __DATE__, ",", __TIME__);
            test.log(message_with_severity(severity::trace, std::move(s)));
        }
    public:
        unit_test()
        {
            if (0) {
                file_log test(
                    (log_size_t) megabyte<1>::value,
                    (buf_size_t) megabyte<1>::value,
                    R"(D:\TEST_LOG\test.log)");
                test_trace(test);
                for (size_t i = 0; i < 100; ++i) {
                    test_trace(test);
                }
                while (!test.empty()) {
                    sleep_for_milliseconds(100);
                }
            }
            if (0) {
                file_log test(
                    (log_size_t) 100,
                    (buf_size_t) megabyte<1>::value,
                    R"(D:\TEST_LOG\test.log1)", 
                    R"(D:\TEST_LOG\test.log2)");
                for (size_t i = 0; i < 100; ++i) {
                    test_trace(test);
                }
                sleep_for_milliseconds(1000);
                for (size_t i = 0; i < 100; ++i) {
                    test_trace(test);
                }
                sleep_for_milliseconds(1000);
            }
        }
    };
    static unit_test s_test;

}}} // sdl
#endif //#if SV_DEBUG
