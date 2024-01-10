#pragma once

// This implementation only works on 64-bit x86 machines
// Please check your cpu's virtual bits!
// See: https://github.com/golang/go/blob/master/src/runtime/tagptr_64bit.go
// Check `cat /proc/cpuinfo | grep "bits virtual"`

#include <stddef.h>
#include <stdint.h>
#include "misc.h"
#include "typechecked_struct.h"

#define SHR_PTR_BITS       (64)
#define SHR_ADDR_BITS      (48)
#define SHR_ALIGNMENT_BITS (3)
#define SHR_TAG_BITS       (SHR_PTR_BITS - SHR_ADDR_BITS + SHR_ALIGNMENT_BITS)

SHR_DEFINE_DEBUG_TYPECHECKED_STRUCT(shr_tagptr_t, uintptr_t, _shr_tagged_ptr_create_from_uintptr, _shr_tagged_ptr_get_uintptr);

shr_tagptr_t shr_tagged_ptr_create(void* _ptr, uint32_t tag)
{
    PROD_ASSERT(tag < ((uint32_t) 1 << SHR_TAG_BITS), "Too much data in tag");
    uintptr_t ptr = (uintptr_t) _ptr;
    PROD_ASSERT((ptr & 0b111) == 0, "Pointer passed in was not byte-aligned");
    return _shr_tagged_ptr_create_from_uintptr((ptr << (SHR_PTR_BITS - SHR_ADDR_BITS)) | tag);
}

void* shr_tagged_ptr_get_ptr(shr_tagptr_t ptr)
{
    return (void*) (uintptr_t) (((intptr_t)_shr_tagged_ptr_get_uintptr(ptr)) >> SHR_TAG_BITS << SHR_ALIGNMENT_BITS);
}

uint32_t shr_tagged_ptr_get_tag(shr_tagptr_t ptr)
{
    return (uint32_t) _shr_tagged_ptr_get_uintptr(ptr) & ((1 << SHR_TAG_BITS) - 1);
}
