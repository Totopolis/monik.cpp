// main.cpp : Defines the entry point for the console application.
#include "monik/log/logger_config.h"
#include "monik/log/logger.h"
#include "monik/log/log_define.h"
#include "monik/common/locale.h"
#include "monik/software/cmdLine.h"
#include <fstream>
#include <thread>

namespace {

using namespace monik;

struct cmd_option final : noncopyable {
    struct {
        std::string config;
        std::string message;
        std::string severity;
        std::string source;
        int sleep_for = 0;
    } log;
};

void trace_log(cmd_option const & opt)
{
    if (!opt.log.config.empty()) {
        std::cout
            << "\nconfig = " << opt.log.config
            << "\nmessage = " << opt.log.message
            << "\nseverity = " << opt.log.severity
            << "\nsource = " << opt.log.source
            << "\nsleep_for = " << opt.log.sleep_for
            << std::endl;
        using T = log::logger::ST;
        if (log::logger_config::setup_logs_file(T::instance(), opt.log.config.c_str())) {
            if (!opt.log.message.empty()) {
                log::severity t = log::severity::verbose;
                if (!opt.log.severity.empty()) {
                    t = log::severity_str::from_string(opt.log.severity.c_str());
                    MONIK_ASSERT(log::severity_str::to_string(t) == opt.log.severity);
                }
                T::cinstance().log(t, opt.log.message, opt.log.source.c_str());
            }
        }
        else {
            MONIK_ASSERT(0);
        }
    }
    if (opt.log.sleep_for > 0) {
        for (int i = 0; i < opt.log.sleep_for; ++i) {
            std::cout << ".";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    std::cout << "\nDone\n";
}

void print_version()
{
#if defined(NDEBUG)
    std::cout << "\nNDEBUG version";
#else
    std::cout << "\nDEBUG version";
#endif
#if defined(MONIK_DEBUG)
    std::cout << "\nMONIK_DEBUG = " << MONIK_DEBUG;
#endif
#if defined(MONIK_INCLUDE_AMQP)
    std::cout << "\nMONIK_INCLUDE_AMQP = " << MONIK_INCLUDE_AMQP;
#endif
    std::cout
        << "\nBuild date: " << __DATE__
        << "\nBuild time: " << __TIME__
        << std::endl;
}

void print_help(int argc, char* argv[])
{
    (void)argc;
    print_version();
    std::cout
        << "\nUsage: " << argv[0]
        << "\n[-i|--config] json config file"
        << "\n[-m|--message] test message"
        << "\n[-s|--severity] trace|debug|info|warning|error|fatal"
        << "\n[-u|--source] system|application|logic|security"
        << "\n[-t|--sleep_for] seconds"
        << std::endl;
}

// C++ option parser library, supporting the standard GNU style syntax for options.
//FIXME: https://github.com/jarro2783/cxxopts

int run_main(int argc, char* argv[])
{
    setlocale_t::set("Russian");
    cmd_option opt{};
    CmdLine cmd;
#if MONIK_DEBUG
    cmd.add(make_option(0, debug::warning_level(), "warning"));
#endif
    cmd.add(make_option('i', opt.log.config, "config"));    
    cmd.add(make_option('m', opt.log.message, "message"));    
    cmd.add(make_option('s', opt.log.severity, "severity"));    
    cmd.add(make_option('u', opt.log.source, "source"));
    cmd.add(make_option('t', opt.log.sleep_for, "sleep_for"));
    try {
        if (argc == 1) {
            print_help(argc, argv);
            std::cout << "\nMissing parameters" << std::endl;
            return EXIT_SUCCESS;
        }
        cmd.process(argc, argv);
        if (!opt.log.config.empty()) {
            trace_log(opt);
        }
        else {
            throw std::string("Missing parameters");
        }
    }
    catch (const std::string& s) {
        print_help(argc, argv);
        std::cerr << "\n" << s << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

} // namespace 

int main(int argc, char* argv[])
{
    try {
        return run_main(argc, argv);
    }
    catch (sdl_exception & e) {
        (void)e;
        std::cout << "\ncatch exception [" << typeid(e).name() << "] = " << e.what();
        MONIK_ASSERT(0);
    }
    catch (std::exception & e) {
        (void)e;
        std::cout << "\ncatch std::exception = " << e.what();
        MONIK_ASSERT(0);
    }
}


