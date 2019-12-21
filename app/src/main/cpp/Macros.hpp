#ifndef SNOOKER3D_MOBILE_MACROS_HPP
#define SNOOKER3D_MOBILE_MACROS_HPP


#include <android/log.h>

#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)
#define LOG(...) __android_log_print(ANDROID_LOG_INFO, "Snooker3D:" S__LINE__, __VA_ARGS__)

#endif
