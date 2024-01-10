#pragma once

#include <memory.h>

#define ZERO(buffer) \
({ \
    memset(buffer, 0, sizeof(buffer)); \
})
