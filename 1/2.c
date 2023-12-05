#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <shared/file_io.h>
#include <shared/memory.h>
#include <shared/logging.h>
#include <strings.h>

#define FOR_LIST_OF_PAIRINGS(DO_PAIRING, buf, offset, next_to_set) \
    DO_PAIRING("one", 1, buf, offset, next_to_set);    \
    DO_PAIRING("two", 2, buf, offset, next_to_set);    \
    DO_PAIRING("three", 3, buf, offset, next_to_set);  \
    DO_PAIRING("four", 4, buf, offset, next_to_set);   \
    DO_PAIRING("five", 5, buf, offset, next_to_set);   \
    DO_PAIRING("six", 6, buf, offset, next_to_set);    \
    DO_PAIRING("seven", 7, buf, offset, next_to_set);  \
    DO_PAIRING("eight", 8, buf, offset, next_to_set);  \
    DO_PAIRING("nine", 9, buf, offset, next_to_set);

#define CHECK_PAIRING(str, num, buf, offset, next_to_set)      \
    if (0 == memcmp(buf+offset, str, sizeof(str)-1))           \
    {                                                          \
        offset += 1;                             \
        *next_to_set = (struct u64) {.val = num, .set = true}; \
        continue;                                              \
    }

struct u64
{
    uint64_t val;
    bool set;
};

int main()
{
    struct shr_mmap_file f = SHR_SAFE_MMAP_FILE("./1/puzzle.in");
    uint64_t decoded_ints[1024]; ZERO(decoded_ints);
    struct u64 vals[2]; ZERO(vals);
    struct u64* next_to_set = &vals[0];
    int line = 0;
    for (int32_t i = 0; i < f.stat.st_size; i++)
    {
        char c = f.file[i];
        next_to_set = !vals[0].set ? &vals[0] : &vals[1];
        FOR_LIST_OF_PAIRINGS(CHECK_PAIRING, f.file, i, next_to_set);
        if ('0' <= c && c <= '9')
        {
            int8_t val = c - '0';
            *next_to_set = (struct u64) {.val = val, .set = true};
        }
        else if ('\n' == c)
        {
            if (!vals[1].set)
            {
                vals[1] = vals[0];
            }
            decoded_ints[line] = vals[0].val * 10 + vals[1].val;
            DEBUG("%lu", decoded_ints[line]);
            line += 1;
            ZERO(vals);
        }
    }
    uint64_t sum = 0;
    for (int i = 0; i < line; i++)
    {
        sum += decoded_ints[i];
    }
    DEBUG("%lu", sum);
    shr_mmap_file_cleanup(&f);
    return 0;
}
