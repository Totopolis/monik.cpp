// logger_config.h
//
#pragma once
#ifndef __MONIK_LOG_LOGGER_CONFIG_H__
#define __MONIK_LOG_LOGGER_CONFIG_H__

#include "monik/common/common.h"

namespace monik { namespace log {

class logger;
class logger_config : is_static {
public:
    static bool setup_logs_file(logger &, const char * filename);
    static bool setup_logs_json(logger &, const std::string & json);
};

} // log
} // monik

#endif // __MONIK_LOG_LOGGER_CONFIG_H__

