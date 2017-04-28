// logger.cpp
//
#include "monik/log/logger.h"
#include "monik/log/console_log.h"
#include "monik/log/pattern_formatter.h"
#include "monik/common/format.h"
#include "monik/common/algorithm.h" // for monik::algo::lex::split
#if MONIK_DEBUG
#include "monik/log/file_log.h"
#endif

namespace monik { namespace log {

class logger::impl final : noncopyable {
public:
    using channel_format = std::pair<shared_channel, std::string>; // pair<channel, format>
    using vector_channel = std::vector<channel_format>;
    struct data_type {
        ex_handler handler;
        severity filter = severity::verbose;
        array_severity<vector_channel> channel;
#if MONIK_INCLUDE_AMQP
        array_severity<shared_keepalive> keepalive;
#endif
    } d;
    void log(message_with_severity &&, message_source_ptr) const;
    impl();
    ~impl();
    bool is_pass(const severity t) const {
        return (d.filter <= t);
    }
    bool empty() const;
private:
    void log_message_with_level(channel_format const &,
        message_with_severity &&, message_source_ptr) const;
};

logger::impl::impl()
{
    MONIK_TRACE_FUNCTION;
}

logger::impl::~impl()
{
    MONIK_TRACE_FUNCTION;
}

bool logger::impl::empty() const
{
    for (const auto & p : d.channel) {
        if (!p.empty())
            return false;
    }
    for (const auto & p : d.keepalive) {
        if (p != nullptr)
            return false;
    }
    return true;
}

void logger::impl::log_message_with_level(channel_format const & dest, 
    message_with_severity && s, message_source_ptr source) const
{
    MONIK_ASSERT(!s.m_message.empty() || source);
    if (dest.second.empty() || (dest.second == pattern_formatter::message_text())) {
        if (!s.m_message.empty()) {
            dest.first->log(std::move(s), source); // message text without formatting
        }
    }
    else {
        std::string text = pattern_formatter::format(dest.second, s, source);
        if (!text.empty()) {
            dest.first->log(message_with_severity(s.m_severity, std::move(text)), source);
        }
    }
}

void logger::impl::log(message_with_severity && s, message_source_ptr source) const
{
    const severity t = s.m_severity;
    if (is_pass(t) && (!s.m_message.empty() || source)) {
        try {
            const auto & channels = d.channel[t];
            const size_t size = channels.size();
            if (size) {
                if (size == 1) {
                    log_message_with_level(channels[0], std::move(s), source);
                }
                else {
                    for (size_t i = 0, end = size - 1; i < end; ++i) {
                        const message_with_severity & cs = s;
                        log_message_with_level(channels[i], message_with_severity(cs), source); // copy message
                    }
                    log_message_with_level(channels.back(), std::move(s), source); // move message
                }
            }
        }
        catch (...) {
            if (d.handler != nullptr) {
                d.handler(t, std::current_exception());
            }
            else {
                throw;
            }
        }
    }
}

//----------------------------------------------------

logger::logger()
    : m_data(std::make_unique<impl>())
{
    MONIK_TRACE_FUNCTION;
}

logger::~logger()
{
    MONIK_TRACE_FUNCTION;
    MONIK_ASSERT(empty() && "call reset_logger before ~logger for safety");
}

bool logger::empty() const
{
    return m_data->empty();
}

void logger::reset()
{
    MONIK_TRACE_FUNCTION;
    m_data = std::make_unique<impl>();
}

bool logger::is_pass(severity t) const
{
    return m_data->is_pass(t);
}

severity logger::get_filter() const
{
    return m_data->d.filter;
}

void logger::set_filter(const severity t)
{
    m_data->d.filter = t;
}

void logger::set_handler(ex_handler && f)
{
    m_data->d.handler = std::move(f);
}

void logger::set_console(const std::initializer_list<severity> in, const std::string & aformat)
{
    MONIK_ASSERT(in.size());
    if (in.size()) {
        const std::string format = trim_string(aformat);
        const shared_channel p(new console_log(0));
        for (const severity t : in) {
            auto & dest = m_data->d.channel[t];
            dest.clear();
            dest.emplace_back(p, format);
        }
    }
}

void logger::set_channel(const severity t, shared_channel const & p, const std::string & format)
{
    MONIK_ASSERT(t < severity::_end);
    MONIK_ASSERT(p);
    if (p) {
        auto & dest = m_data->d.channel[t];
        dest.clear();
        dest.emplace_back(p, trim_string(format));
    }
}

void logger::add_channel(const severity t, shared_channel const & p, const std::string & format)
{
    MONIK_ASSERT(t < severity::_end);
    MONIK_ASSERT(p);
    if (p) {
        m_data->d.channel[t].emplace_back(p, trim_string(format));
    }
}

#if MONIK_INCLUDE_AMQP
bool logger::add_keepalive(shared_keepalive const & p)
{
    if (p && !p->running()) {
        m_data->d.keepalive[p->get_severity()] = p;
        return p->launch();
    }
    MONIK_ASSERT(0);
    return false;
}
#endif

void logger::log(const severity t, std::string const & s) const
{
    m_data->log(message_with_severity(t, s), nullptr);
}

void logger::log(const severity t, std::string const & s, message_source_ptr source) const
{
    m_data->log(message_with_severity(t, s), source);
}

void logger::log(const severity t, std::string && s) const
{
    m_data->log(message_with_severity(t, std::move(s)), nullptr);
}

void logger::log(const severity t, std::string && s, message_source_ptr source) const
{
    m_data->log(message_with_severity(t, std::move(s)), source);
}

void logger::log(message_with_severity && s) const
{
    m_data->log(std::move(s), nullptr);
}

void logger::log(message_with_severity && s, message_source_ptr source) const
{
    m_data->log(std::move(s), source);
}

char severity_str::abbreviated(const severity t)
{
    MONIK_ASSERT(t < severity::_end);
    static char table[severity_size() + 1] = {
        'V', // verbose
        'I', // info
        'W', // warning
        'E', // error
        'F', // fatal
        char(0)
    };
    return table[static_cast<size_t>(t)];
}

const char * severity_str::to_string(const severity t)
{
    switch (t) {
    case severity::verbose : return "verbose";
    case severity::info    : return "info";
    case severity::warning : return "warning";
    case severity::error   : return "error";
    case severity::fatal   : return "fatal";
    default:
        MONIK_ASSERT(0);
        return "";
    }
}

severity severity_str::from_string(const char * const s)
{
    if (is_str_valid(s)) {
        if (0 == strcmp(s, "verbose"))  { return severity::verbose; }
        if (0 == strcmp(s, "info"))     { return severity::info; }
        if (0 == strcmp(s, "warning"))  { return severity::warning; }
        if (0 == strcmp(s, "error"))    { return severity::error; }
        if (0 == strcmp(s, "fatal"))    { return severity::fatal; }
    }
    MONIK_ASSERT(0);
    return severity::verbose;
}

std::vector<severity> 
severity_str::parse(std::string const & str)
{
    if (!str.empty()) {
        if (str == "all") {
            return { severity_all() };
        }
        const auto arr = algo::lex::split(str, ',');
        std::vector<severity> result;
        result.reserve(arr.size());
        for (const auto & s : arr) {
            result.push_back(severity_str::from_string(s));
        }
        algo::sort_erase_unique(result);
        MONIK_ASSERT(result.size() == arr.size());
        return result;
    }
    return{};
}

init_logger_t & init_logger() {
    struct initialize_once {
        init_logger_t obj;
        initialize_once() {
            obj = [](){
                return &logger::ST::instance();
            };
        }
    };
    static initialize_once init;
    return init.obj;
}

} // log
} // monik

#if MONIK_DEBUG
namespace monik { namespace log { namespace {
    class unit_test {
    public:
        using T = singleton<logger>;
        unit_test()
        {
            {
                const auto old = init_logger();
                MONIK_ASSERT(get_logger());
                init_logger() = nullptr;
                MONIK_ASSERT(!get_logger());
                init_logger() = old;
                MONIK_ASSERT(get_logger());
            }
            {
                for (const severity t : severity_all()) {
                    MONIK_ASSERT(severity_str::from_string(severity_str::to_string(t)) == t);
                    MONIK_ASSERT((severity_str::abbreviated(t) - 'A' + 'a') == severity_str::to_string(t)[0]);
                }
            }
            if (0) {
                T::instance().set_filter(severity::verbose);
                T::instance().set_console(severity_all());
                set_handler();
                set_file_log();
                test_log();
            }
        }
    private:
        void set_file_log()
        {
            if (0) {
                auto f = std::make_shared<file_log>(
                    (log_size_t) megabyte<10>::value,
                    (buf_size_t) megabyte<1>::value,
                    R"(D:\TEST_LOG\fatal.log)"
                    );
                T::instance().add_channel(severity::error, f);
                T::instance().add_channel(severity::fatal, f);
            }
        }
        void test_log()
        {
            for (size_t i = 0; i < severity_size(); ++i) {
                const severity t = static_cast<severity>(i);
                try {
                    auto s = std::string("log_") + severity_str::to_string(t) + "," + __DATE__ + "," + __TIME__;
                    T::cinstance().log(t, std::move(s));
                }
                catch (...) {
                    MONIK_ASSERT(t == severity::fatal);
                }
            }
        }
        void set_handler()
        {
            T::instance().set_handler([](severity t, std::exception_ptr p) {
                if (t == severity::fatal) {
                    std::rethrow_exception(p);
                }
                else {
                    try {
                        std::rethrow_exception(p);
                    }
                    catch(const std::exception& e) {
                        std::cout << e.what() << std::endl;
                    }
                }
            });
        }
    };
    static unit_test s_test;

}}} // monik
#endif //#if SV_DEBUG
