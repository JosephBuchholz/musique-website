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

void _LogLevelAndTag(const char* logLevel, const char* tag, const char* format, va_list args);
void _LogLevel(const char* logLevel, const char* format, ...);

#define LOG_LEVEL_STR_V "V"
#define LOG_LEVEL_STR_D "Debug"
#define LOG_LEVEL_STR_I "Info"
#define LOG_LEVEL_STR_W "Warning"
#define LOG_LEVEL_STR_E "Error"
#define LOG_LEVEL_STR_F "Fatal"

void LOGV_TAG(const char* tag, const char* format, ...);
void LOGD_TAG(const char* tag, const char* format, ...);
void LOGI_TAG(const char* tag, const char* format, ...);
void LOGW_TAG(const char* tag, const char* format, ...);
void LOGE_TAG(const char* tag, const char* format, ...);
void LOGF_TAG(const char* tag, const char* format, ...);

void LOGV(const char* format, ...);
void LOGD(const char* format, ...);
void LOGI(const char* format, ...);
void LOGW(const char* format, ...);
void LOGE(const char* format, ...);
void LOGF(const char* format, ...);

#define ASSERT_MSG(condition, msg) \
  assert((msg, condition))

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