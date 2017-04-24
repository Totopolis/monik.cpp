// amqp_config.h
//
#pragma once
#ifndef __MONIK_LOG_AMQP_INCLUDE_H__
#define __MONIK_LOG_AMQP_INCLUDE_H__

#include <amqp.h>
#include <amqp_framing.h>

#include "monik/common/common.h"

#if defined(AMQP_NOEXCEPT)
#error AMQP_NOEXCEPT
#endif

#if 0
#define AMQP_NOEXCEPT   throw()
#else
#define AMQP_NOEXCEPT   noexcept
#endif

namespace AmqpClient { 
    using monik::noncopyable;
}

#endif // __MONIK_LOG_AMQP_INCLUDE_H__