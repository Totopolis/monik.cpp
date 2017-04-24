// file_log.h
//
#pragma once
#ifndef __MONIK_LOG_FILE_LOG_H__
#define __MONIK_LOG_FILE_LOG_H__

#include "monik/log/message.h"

namespace sdl { namespace log { 

class file_log final : public channel { // thread safe
public:
    file_log(log_size_t, buf_size_t,
             const std::string & path1,
             const std::string & path2 = "");
    ~file_log() override;
    bool empty() const override;
    void log(message_with_severity &&) override; 
private:
    class data_type;
    const std::unique_ptr<data_type> m_data;
};

} // log
} // sdl

#endif // __MONIK_LOG_FILE_LOG_H__

