// json_parser.h
//
#pragma once
#ifndef __MONIK_LOG_JSON_PARSER_H__
#define __MONIK_LOG_JSON_PARSER_H__

#include "monik/common/common.h"

namespace sdl { namespace log {

struct json_parser : is_static {
    enum class proto_syntax { _2 = 2, _3 = 3 };
    static std::string read_text_file(const char * filename);
    static std::string json_to_proto(std::string const & json, proto_syntax);
};    

}} // sdl

#endif // __MONIK_LOG_JSON_PARSER_H__
