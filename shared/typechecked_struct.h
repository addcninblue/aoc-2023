#pragma once
#include "misc.h"

#ifndef NDEBUG
// Typechecking -- see pte_t in kernel code for inspiration
// Also https://www.kernel.org/doc/html/v4.10/process/coding-style.html#typedefs
#define SHR_DEFINE_DEBUG_TYPECHECKED_STRUCT(new_type, orig_type, create_fn, get_fn) \
typedef struct _##new_type                                                          \
{                                                                                   \
    orig_type x;                                                                    \
} new_type;                                                                         \
                                                                                    \
new_type create_fn(orig_type x)                                                     \
{                                                                                   \
    return (new_type) {.x = x};                                                     \
}                                                                                   \
                                                                                    \
orig_type get_fn(new_type y)                                                        \
{                                                                                   \
    return y.x;                                                                     \
}
#else
#define SHR_DEFINE_DEBUG_TYPECHECKED_STRUCT(new_type, orig_type, create_fn, get_fn) \
typedef orig_type new_type;                                                         \
                                                                                    \
new_type create_fn(orig_type x)                                                     \
{                                                                                   \
    return x;                                                                       \
}                                                                                   \
                                                                                    \
orig_type get_fn(new_type y)                                                        \
{                                                                                   \
    return y;                                                                       \
}
#endif
