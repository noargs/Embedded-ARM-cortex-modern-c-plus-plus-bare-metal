#pragma once
#include <cstdio>

#define DEBUG_GEN
#define DEBUG_LEVEL2

#ifdef DEBUG_GEN
#define debug_log(format, ...) std::printf("[DEBUG] <%s:%d>: " format "\n", __func__, __LINE__, ## __VA_ARGS__)
#else
#define debug_log(...)
#endif

#ifdef DEBUG_LEVEL2
#define debug_log2(format, ...) std::printf("[DEBUG LEVEL2] <%s:%d>: " format "\n", __func__, __LINE__, ## __VA_ARGS__)
#else
#define debug_log2(...)
#endif

void log_error(char* p);
void log_info(char* p);
