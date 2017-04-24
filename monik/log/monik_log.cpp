// monik_log.cpp
//
#include "monik/log/monik_log.h"
#include "monik/common/time_util.h"

#if MONIK_INCLUDE_AMQP

#if defined(MONIK_OS_WIN32) && defined(ERROR)
#undef ERROR // windows may conflict with monik.proto
#endif
#include "monik/protobuf/monik.pb.h"

namespace monik { namespace log {

monik_log::monik_log(Params && p)
    : m_log(std::move(p))
{
    MONIK_TRACE_FUNCTION;
}

monik_log::~monik_log()
{
    MONIK_TRACE_FUNCTION;
}

bool monik_log::empty() const
{
    return m_log.empty();
}

namespace monik_log_ {

tutorial::LevelType monik_level(const char * const s)
{
    if (is_str_valid(s)) {        
        if (0 == strcmp(s, monik_source::system))      return tutorial::LevelType::SYSTEM;
        if (0 == strcmp(s, monik_source::application)) return tutorial::LevelType::APPLICATION;
        if (0 == strcmp(s, monik_source::logic))       return tutorial::LevelType::LOGIC;
        if (0 == strcmp(s, monik_source::security))    return tutorial::LevelType::SECURITY;
    }
    return tutorial::LevelType::APPLICATION;
}

tutorial::SeverityType monik_severity(const severity t)
{
    switch (t) {
    case severity::info:       return tutorial::SeverityType::INFO;
    case severity::warning:    return tutorial::SeverityType::WARNING;
    case severity::error:      return tutorial::SeverityType::ERROR;
    case severity::fatal:      return tutorial::SeverityType::FATAL;
    default:
        MONIK_ASSERT((t == severity::trace) || (t == severity::debug));
        return tutorial::SeverityType::VERBOSE;
    }
}

} // monik_log_

std::string monik_log::to_string(const message_with_severity & s, const char * const source) const
{
    const auto & params = m_log.params();
    tutorial::Event event;
    event.set_created(static_cast<std::int64_t>(time_since_epoch_milliseconds()));
    event.set_source(params.source);
    event.set_instance(params.instance);
    if (tutorial::Log* const log = event.mutable_lg()) {
        log->set_level(monik_log_::monik_level(source));
        log->set_severity(monik_log_::monik_severity(s.m_severity));
        log->set_format(tutorial::PLAIN);
        log->set_body(s.m_message.c_str());
        if (!params.tags.empty()) {
            log->set_tags(params.tags);
        }
        std::string out;
        if (event.SerializeToString(&out)) {
            MONIK_ASSERT(!out.empty());
            return out;
        }
    }
    MONIK_ASSERT(0);
    return{};
}

void monik_log::log(message_with_severity && s)
{
    monik_log::log(std::move(s), nullptr);
}

void monik_log::log(message_with_severity && s, message_source_ptr source)
{
    std::string str = this->to_string(s, source);
    if (!str.empty()) {
        m_log.log(message_with_severity(s.m_severity, std::move(str)));
    }
}

#if MONIK_DEBUG
void monik_log::test()
{
    if (1) {
        for (severity t : severity_all()) {
            MONIK_ASSERT(!this->to_string(message_with_severity(t, "test"), nullptr).empty());
        }
    }
}
#endif

} // log
} // monik

#endif // MONIK_INCLUDE_AMQP

