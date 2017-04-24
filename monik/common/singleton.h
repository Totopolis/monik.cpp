// singleton.h
//
#pragma once
#ifndef __MONIK_LOG_COMMON_SINGLETON_H__
#define __MONIK_LOG_COMMON_SINGLETON_H__

namespace sdl { namespace log {

template <class T>
class singleton : private T {
    singleton() = default;
    ~singleton() = default;
public:
    static T & instance() {
        static singleton<T> object;
        return object;
    }
    static T const & cinstance() {
		return instance();
	}
};

} // log
} // sdl

#endif // __MONIK_LOG_COMMON_SINGLETON_H__
