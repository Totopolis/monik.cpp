// logger_config.cpp
//
#include "monik/log/logger_config.h"
#include "monik/log/logger.h"
#include "monik/log/console_log.h"
#include "monik/log/file_log.h"
#include "monik/log/monik_log.h"
#include "monik/log/third_party/json/json_parser.h"
#include "monik/log/third_party/json/reader.h"
#include "monik/common/algorithm.h" // for sdl::algo::lex::split

namespace sdl { namespace log { 

bool logger_config::setup_logs_file(logger & dest, const char * const filename)
{
    return setup_logs_json(dest, json_parser::read_text_file(filename));
}

bool logger_config::setup_logs_json(logger & dest, const std::string & json)
{
    if (!json.empty()) {
        Json::Value root; // will contains the root value after parsing
        Json::Reader reader;
        if (reader.parse(json, root)) {
            {
                const auto filter = root["filter"].asString();
                if (!filter.empty()) {
                    dest.set_filter(logger::from_string(filter.c_str()));
                }
            }
            for (const auto & m : root["loggers"]) {
                const auto severity_arr = algo::lex::split(m["severity"].asString(), ',');
                SDL_ASSERT(!severity_arr.empty());
                if (!severity_arr.empty()) {
                    const buf_size_t bufsize = m["bufsize"].asInt(); // can be 0
                    const auto format = m["format"].asString();
                    const auto channel = m["channel"].asString();
                    shared_channel sp;
                    if (channel == "console") {
                        sp = std::make_shared<console_log>(bufsize);
                    }
                    else if (channel == "file") {
                        const auto path1 = m["path1"].asString();
                        const auto path2 = m["path2"].asString();
                        const log_size_t logsize = m["logsize"].asInt(); // can be 0
                        if (!path1.empty()) {
                            sp = std::make_shared<file_log>(logsize, bufsize, path1, path2);
                        }
                        else {
                            SDL_ASSERT(0);
                        }
                    }
#if SDL_INCLUDE_AMQP
                    else if (channel == "monik") {
                        const auto pp = m["params"];
                        if (!pp.isNull()) {
                            remote_log::Params params {};
                            params.host = pp["host"].asString();
                            params.port = pp["port"].asInt();
                            params.user = pp["user"].asString();
                            params.password = pp["password"].asString();
                            params.exchange = pp["exchange"].asString();
                            params.source = pp["source"].asString();
                            params.instance = pp["instance"].asString();
                            params.tags = pp["tags"].asString();
                            params.durable = pp["durable"].asBool();
                            params.retrytimeout = pp["retrytimeout"].asInt();
                            params.bufsize = bufsize;
                            sp = std::make_shared<monik_log>(std::move(params));
                        }
                    }
                    else if (channel == "keepalive") {
                        const auto pp = m["params"];
                        if (!pp.isNull()) {
                            const second_t period = pp["period"].asInt();
                            if (period.value() > 0) {
                                keepalive_log::Params params {};
                                params.host = pp["host"].asString();
                                params.port = pp["port"].asInt();
                                params.user = pp["user"].asString();
                                params.password = pp["password"].asString();
                                params.exchange = pp["exchange"].asString();
                                params.source = pp["source"].asString();
                                params.instance = pp["instance"].asString();
                                params.tags = pp["tags"].asString();
                                params.durable = pp["durable"].asBool();
                                params.retrytimeout = pp["retrytimeout"].asInt();
                                params.bufsize = bufsize;
                                SDL_ASSERT(params.retrytimeout.value() > 0);
                                for (const auto & s : severity_arr) {
                                    dest.add_keepalive(std::make_shared<keepalive_log>(
                                        logger::from_string(s), period, std::move(params)));
                                }
                            }
                            else {
                                SDL_ASSERT(!"keepalive period");
                            }
                        }
                    }
#endif
                    if (sp) {
                        for (const auto & s : severity_arr) {
                            dest.add_channel(logger::from_string(s), sp, format);
                        }
                    }
                }
            }
            return true;
        }
        SDL_ASSERT(!"Json::Reader::parse");
    }
    SDL_WARNING(0);
    return false;
}

} // log
} // sdl

#if defined(SDL_OS_WIN32) && SDL_DEBUG
namespace sdl { namespace log { namespace {
    class unit_test {
    public:
        unit_test()
        {
            if (0) {
                const char test_path[] = CMAKE_CURRENT_SOURCE_DIR R"(/dataserver/log/ignore/logger.json)";
                if (logger_config::setup_logs_file(logger::ST::instance(), test_path)) {
                    logger::ST::cinstance().log(severity::debug, "debug", nullptr);
                    for (const severity t : severity_all()) {
                        std::string s("severity_"); s += logger::to_string(t);
                        logger::ST::cinstance().log(t, std::move(s), nullptr);
                    }
                }
            }
            if (0) {
                logger::ST::instance().reset();
            }
        }
    };
    static unit_test s_test;
}}} // sdl
#endif //#if SV_DEBUG