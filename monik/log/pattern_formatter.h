// pattern_formatter.h
//
#pragma once
#ifndef __SDL_LOG_PATTERN_FORMATTER_H__
#define __SDL_LOG_PATTERN_FORMATTER_H__

#include "monik/log/message.h"

namespace sdl { namespace log {

/*
The format pattern is used as a template to format the message
and is copied character by character except for the following special characters, 
which are replaced by the corresponding value.

%s - message source
%t - message text
%l - message priority level (1 .. 7)
%p - message priority (Fatal, Critical, Error, Warning, Notice, Information, Debug, Trace)
%q - abbreviated message priority (F, C, E, W, N, I, D, T)
(not supported) %P - message process identifier
(not supported) %T - message thread name
%I - message thread identifier (numeric)
%N - node or host name
%U - message source file path (empty string if not set)
%u - message source line number (0 if not set)
%w - message date/time abbreviated weekday (Mon, Tue, ...)
%W - message date/time full weekday (Monday, Tuesday, ...)
%b - message date/time abbreviated month (Jan, Feb, ...)
%B - message date/time full month (January, February, ...)
%d - message date/time zero-padded day of month (01 .. 31)
%e - message date/time day of month (1 .. 31)
%f - message date/time space-padded day of month ( 1 .. 31)
%m - message date/time zero-padded month (01 .. 12)
%n - message date/time month (1 .. 12)
%o - message date/time space-padded month ( 1 .. 12)
%y - message date/time year without century (70)
%Y - message date/time year with century (1970)
%H - message date/time hour (00 .. 23)
%h - message date/time hour (00 .. 12)
%a - message date/time am/pm
%A - message date/time AM/PM
%M - message date/time minute (00 .. 59)
%S - message date/time second (00 .. 59)
%i - message date/time millisecond (000 .. 999)
%c - message date/time centisecond (0 .. 9)
%F - message date/time fractional seconds/microseconds (000000 - 999999)
(not supported) %z - time zone differential in ISO 8601 format (Z or +NN.NN)
(not supported) %Z - time zone differential in RFC format (GMT or +NNNN)
%L - convert time to local time (must be specified before any date/time specifier; does not itself output anything)
%U - convert time to UTC time (must be specified before any date/time specifier; does not itself output anything)
%E - epoch time (UTC, seconds since midnight, January 1, 1970)
(not supported) %v[width] - the message source (%s) but text length is padded/cropped to 'width'
(not supported) %[name] - the value of the message parameter with the given name
%% - percent sign
*/

class pattern_formatter : is_static {
public:
    static constexpr const char * message_text() { return "%t"; };
    static void format(std::string &, const std::string &, message_with_severity const &, const char * const source);
    static std::string format(const std::string & pattern, message_with_severity const & msg, const char * const source) {
        std::string text;
        format(text, pattern, msg, source);
        return text;
    }
};

} // log
} // sdl

#endif // __SDL_LOG_PATTERN_FORMATTER_H__

