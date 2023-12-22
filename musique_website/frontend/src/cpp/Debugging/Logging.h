#pragma once

#if DEBUG

// define log tag if it dosn't exist
#if !defined(LOG_TAG)
#define LOG_TAG "Musique C++"
#endif

#if defined ANDROID

#include "AndroidDebug.h"

#elif defined WEB

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>

void _LogLevelAndTag(const char* logLevel, const char* tag, const char* format, ...)
{
    printf("%s [%s]: ", logLevel, tag);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}

void _LogLevel(const char* logLevel, const char* format, ...)
{
    printf("%s: ", logLevel);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}

#define LOG_LEVEL_STR_V "V"
#define LOG_LEVEL_STR_D "Debug"
#define LOG_LEVEL_STR_I "Info"
#define LOG_LEVEL_STR_W "Warning"
#define LOG_LEVEL_STR_E "Error"
#define LOG_LEVEL_STR_F "Fatal"

#define LOGV_TAG(tag, format, ...) \
    _LogLevelAndTag(LOG_LEVEL_STR_V, tag, format, __VA_ARGS__)
#define LOGD_TAG(tag, format, ...) \
    _LogLevelAndTag(LOG_LEVEL_STR_D, tag, format, __VA_ARGS__)
#define LOGI_TAG(tag, format, ...) \
    _LogLevelAndTag(LOG_LEVEL_STR_I, tag, format, __VA_ARGS__)
#define LOGW_TAG(tag, format, ...) \
    _LogLevelAndTag(LOG_LEVEL_STR_W, tag, format, __VA_ARGS__)
#define LOGE_TAG(tag, format, ...) \
    _LogLevelAndTag(LOG_LEVEL_STR_E, tag, format, __VA_ARGS__)
#define LOGF_TAG(tag, format, ...) \
    _LogLevelAndTag(LOG_LEVEL_STR_F, tag, format, __VA_ARGS__)
#define ASSERT_MSG(condition, msg)

/*#define LOGV(format, ...) \
  _LogLevel(LOG_LEVEL_STR_V, format, __VA_ARGS__)
#define LOGD(format, ...) \
  _LogLevel(LOG_LEVEL_STR_D, format, __VA_ARGS__)
#define LOGI(format, ...) \
  _LogLevel(LOG_LEVEL_STR_I, format, __VA_ARGS__)
#define LOGW(format, ...) \
  _LogLevel(LOG_LEVEL_STR_W, format, __VA_ARGS__)
#define LOGE(format, ...) \
  _LogLevel(LOG_LEVEL_STR_E, format, __VA_ARGS__)
#define LOGF(format, ...) \
  _LogLevel(LOG_LEVEL_STR_F, format, __VA_ARGS__)
#define ASSERT(condition) \
  assert(condition)*/

/*#define LOGV_TAG(tag, format) \
    _LogLevelAndTag(LOG_LEVEL_STR_V, tag, format)
#define LOGD_TAG(tag, format) \
    _LogLevelAndTag(LOG_LEVEL_STR_D, tag, format)
#define LOGI_TAG(tag, format) \
    _LogLevelAndTag(LOG_LEVEL_STR_I, tag, format)
#define LOGW_TAG(tag, format) \
    _LogLevelAndTag(LOG_LEVEL_STR_W, tag, format)
#define LOGE_TAG(tag, format) \
    _LogLevelAndTag(LOG_LEVEL_STR_E, tag, format)
#define LOGF_TAG(tag, format) \
    _LogLevelAndTag(LOG_LEVEL_STR_F, tag, format)
#define ASSERT_MSG(condition, msg)*/

#define LOGV(format) \
  _LogLevel(LOG_LEVEL_STR_V, format)
#define LOGD(format) \
  _LogLevel(LOG_LEVEL_STR_D, format)
#define LOGI(format) \
  _LogLevel(LOG_LEVEL_STR_I, format)
#define LOGW(format) \
  _LogLevel(LOG_LEVEL_STR_W, format)
#define LOGE(format) \
  _LogLevel(LOG_LEVEL_STR_E, format)
#define LOGF(format) \
  _LogLevel(LOG_LEVEL_STR_F, format)
#define ASSERT(condition) \
  assert(condition)

#else

#define LOGV_TAG(tag, ...)
#define LOGD_TAG(tag, ...)
#define LOGI_TAG(tag, ...)
#define LOGW_TAG(tag, ...)
#define LOGE_TAG(tag, ...)
#define LOGF_TAG(tag, ...)
#define ASSERT_MSG(condition, msg)

#define LOGV(...)
#define LOGD(...)
#define LOGI(...)
#define LOGW(...)
#define LOGE(...)
#define LOGF(...)
#define ASSERT(condition)

#endif

#else

#define LOGV_TAG(tag, ...)
#define LOGD_TAG(tag, ...)
#define LOGI_TAG(tag, ...)
#define LOGW_TAG(tag, ...)
#define LOGE_TAG(tag, ...)
#define LOGF_TAG(tag, ...)
#define ASSERT_MSG(condition, msg)

#define LOGV(...)
#define LOGD(...)
#define LOGI(...)
#define LOGW(...)
#define LOGE(...)
#define LOGF(...)
#define ASSERT(condition)

#endif // DEBUG