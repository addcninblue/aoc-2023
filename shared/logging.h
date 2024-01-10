#pragma once

#include <stdio.h>

// https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
// https://stackoverflow.com/questions/63740056/variadic-macros-with-zero-arguments-doesnt-compile-even-with-va-args

#ifdef NDEBUG
#    define DEBUG(fmt, ...) (void)0
#else
#    define DEBUG(fmt, ...) \
            do { fprintf(stdout, "[DEBUG] %s:%d:%s(): " fmt "\n", __FILE__, \
                                    __LINE__, __func__,##__VA_ARGS__); fflush(stdout); } while (0)
#endif

#define INFO(fmt, ...) \
        do { fprintf(stdout, "[INFO ] %s:%d:%s(): " fmt "\n", __FILE__, \
                                __LINE__, __func__,##__VA_ARGS__); } while (0)

#define WARN(fmt, ...) \
        do { fprintf(stdout, "[WARN ] %s:%d:%s(): " fmt "\n", __FILE__, \
                                __LINE__, __func__,##__VA_ARGS__); } while (0)

#define ERROR(fmt, ...) \
        do { fprintf(stdout, "[ERROR] %s:%d:%s(): " fmt "\n", __FILE__, \
                                __LINE__, __func__,##__VA_ARGS__); fflush(stdout); } while (0)

#define DEATH(fmt, ...) \
        do { fprintf(stdout, "[DEATH] %s:%d:%s(): " fmt "\n", __FILE__, \
                                __LINE__, __func__,##__VA_ARGS__); fflush(stdout); } while (0)
