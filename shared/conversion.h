#pragma once

#include <stdint.h>
#include "misc.h"

#define CREATE_CONVERSION(fn, in_type, out_type, out_min, out_max) \
    out_type fn(in_type x)                                          \
    {                                                               \
        PROD_ASSERT(x <= out_max, "Too big");                       \
        PROD_ASSERT(out_min <= x, "Too small");                     \
        return (out_type) x;                                        \
    }

CREATE_CONVERSION(shr_safe_u8_from_u64,  uint64_t, uint8_t,  0, UINT8_MAX);
CREATE_CONVERSION(shr_safe_u16_from_u64, uint64_t, uint16_t, 0, UINT16_MAX);
CREATE_CONVERSION(shr_safe_u32_from_u64, uint64_t, uint32_t, 0, UINT32_MAX);
CREATE_CONVERSION(shr_safe_i8_from_u64,  uint64_t, int8_t,   0, INT8_MAX);
CREATE_CONVERSION(shr_safe_i16_from_u64, uint64_t, int16_t,  0, INT16_MAX);
CREATE_CONVERSION(shr_safe_i32_from_u64, uint64_t, int32_t,  0, INT32_MAX);

#undef CREATE_CONVERSION
