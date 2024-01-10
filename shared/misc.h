#pragma once

#include <stdlib.h>
#include <shared/logging.h>

#define DIE() abort();

#define LIKELY(x)      __builtin_expect(!!(x), 1)
#define UNLIKELY(x)    __builtin_expect(!!(x), 0)

#ifdef TEST

#include <threads.h>
#include <setjmp.h>
#include <shared/unity/unity.h>

jmp_buf _jmp_buf;

enum assert_status
{
    ASSERT_STATUS_INVALID = 0,
    ASSERT_STATUS_NONE,
    ASSERT_STATUS_EXPECTING_ERR,
    ASSERT_STATUS_ERRED
};

static thread_local volatile enum assert_status ASSERT_STATUS = ASSERT_STATUS_NONE;
static thread_local volatile char const* ASSERT_ERR = NULL;

void assert_helper_set(char const* const msg)
{
    if (ASSERT_STATUS != ASSERT_STATUS_EXPECTING_ERR)
    {
        char buf[1024];
        snprintf(buf, 1024, "Hit assert but did not expect it. Reason: %s", msg);
        TEST_FAIL_MESSAGE(buf);
    }
    ASSERT_STATUS = ASSERT_STATUS_ERRED;
    ASSERT_ERR = msg;
    longjmp(_jmp_buf, 1);
}

#ifdef assert
#error assert should not be defined!
#endif

#define PROD_ASSERT_1(expr) PROD_ASSERT_2(expr, NULL);
#define PROD_ASSERT_2(expr, msg)  \
({                                \
    if (expr)                     \
        ;                         \
    else                          \
        assert_helper_set(msg);   \
})

#define TEST_ASSERT_ABORTS(msg, expr) \
({                                                           \
    ASSERT_STATUS = ASSERT_STATUS_EXPECTING_ERR;             \
    if (!setjmp(_jmp_buf))                                   \
    {                                                        \
        ({                                                   \
        expr;                                                \
        });                                                  \
        TEST_FAIL_MESSAGE("Test did not abort!");            \
    }                                                        \
    else                                                     \
    {                                                        \
        switch (ASSERT_STATUS)                               \
        {                                                    \
            case ASSERT_STATUS_INVALID:                      \
            case ASSERT_STATUS_NONE:                         \
                TEST_FAIL_MESSAGE("Test in invalid state!"); \
            case ASSERT_STATUS_EXPECTING_ERR:                \
                TEST_FAIL_MESSAGE("Test did not abort!");    \
            case ASSERT_STATUS_ERRED:                        \
                TEST_ASSERT_EQUAL_STRING(msg, ASSERT_ERR);   \
        }                                                    \
    }                                                        \
})

#else
#pragma GCC poison assert_helper_set _jmp_buf ASSERT_STATUS ASSERT_ERR

#define _ASSERT_HELPER(expr, msg) \
({                                \
    /* See assert.h */            \
    if (LIKELY(expr))             \
        ; /* empty */             \
    else                          \
    {                             \
        DEATH("%s", msg);         \
        abort();                  \
    }                             \
})

#define PROD_ASSERT_1(expr)      _ASSERT_HELPER(expr, "Assertion failure: " #expr)
#define PROD_ASSERT_2(expr, msg) _ASSERT_HELPER(expr, msg)

#endif // TEST

#define STATIC_ASSERT static_assert

#ifndef NDEBUG
#define DEBUG_ASSERT_1 PROD_ASSERT_1
#define DEBUG_ASSERT_2 PROD_ASSERT_2
#else
#define DEBUG_ASSERT_1 ((void) 0)
#define DEBUG_ASSERT_2 ((void) 0)
#endif

// https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
#define PROD_ASSERT_(_1,_2,NAME,...) NAME
#define PROD_ASSERT(...) PROD_ASSERT_(__VA_ARGS__, PROD_ASSERT_2, PROD_ASSERT_1)(__VA_ARGS__)
#define DEBUG_ASSERT_(_1,_2,NAME,...) NAME
#define DEBUG_ASSERT(...) DEBUG_ASSERT_(__VA_ARGS__, DEBUG_ASSERT_2, DEBUG_ASSERT_1)(__VA_ARGS__)
