// message.cpp
//
#include "monik/log/message.h"

namespace monik { namespace log {

message_source_ptr monik_source::system("system");
message_source_ptr monik_source::application("application");
message_source_ptr monik_source::logic("logic");
message_source_ptr monik_source::security("security");

} // log
} // monik





