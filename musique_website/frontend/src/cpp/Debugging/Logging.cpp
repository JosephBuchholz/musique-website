#include "Debug.h"
#include "Logging.h"

#if DEBUG

#if defined WEB

void _LogLevelAndTag(const char* logLevel, const char* tag, const char* format, va_list args)
{
    printf("%s [%s]: ", logLevel, tag);
    vprintf(format, args);
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

void LOGV_TAG(const char* tag, const char* format, ...)
{
  va_list args;
  va_start(args, format);
  _LogLevelAndTag(LOG_LEVEL_STR_V, tag, format, args);
  va_end(args);
}

void LOGD_TAG(const char* tag, const char* format, ...)
{
  va_list args;
  va_start(args, format);
  _LogLevelAndTag(LOG_LEVEL_STR_D, tag, format, args);
  va_end(args);
}

void LOGI_TAG(const char* tag, const char* format, ...)
{
  va_list args;
  va_start(args, format);
  _LogLevelAndTag(LOG_LEVEL_STR_I, tag, format, args);
  va_end(args);
}

void LOGW_TAG(const char* tag, const char* format, ...)
{
  va_list args;
  va_start(args, format);
  _LogLevelAndTag(LOG_LEVEL_STR_W, tag, format, args);
  va_end(args);
}

void LOGE_TAG(const char* tag, const char* format, ...)
{
  va_list args;
  va_start(args, format);
  _LogLevelAndTag(LOG_LEVEL_STR_E, tag, format, args);
  va_end(args);
}

void LOGF_TAG(const char* tag, const char* format, ...)
{
  va_list args;
  va_start(args, format);
  _LogLevelAndTag(LOG_LEVEL_STR_F, tag, format, args);
  va_end(args);
}

void LOGV(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  _LogLevel(LOG_LEVEL_STR_V, format, args);
  va_end(args);
}

void LOGD(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  _LogLevel(LOG_LEVEL_STR_D, format, args);
  va_end(args);
}

void LOGI(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  _LogLevel(LOG_LEVEL_STR_I, format, args);
  va_end(args);
}

void LOGW(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  _LogLevel(LOG_LEVEL_STR_W, format, args);
  va_end(args);
}

void LOGE(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  _LogLevel(LOG_LEVEL_STR_E, format, args);
  va_end(args);
}

void LOGF(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  _LogLevel(LOG_LEVEL_STR_F, format, args);
  va_end(args);
}

#endif // WEB

#endif // DEBUG
