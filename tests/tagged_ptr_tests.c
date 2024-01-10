#include <shared/tagged_ptr.h>
#include <shared/unity/unity.h>
#include <stdlib.h>
#include <stdbool.h>
#include <shared/logging.h>

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_simple_tagged_ptr_roundtrip(void)
{
    void* ptr = malloc(1);
    uint32_t tag = 10;
    shr_tagptr_t tagged_ptr = shr_tagged_ptr_create(ptr, tag);
    TEST_ASSERT_EQUAL_PTR(ptr, shr_tagged_ptr_get_ptr(tagged_ptr));
    TEST_ASSERT_EQUAL_UINT32(tag, shr_tagged_ptr_get_tag(tagged_ptr));
}

void test_simple_tagged_ptr_roundtrip2(void)
{
    void* ptr = malloc(1);
    uint32_t tag = 786433;
    shr_tagptr_t tagged_ptr = shr_tagged_ptr_create(ptr, tag);
    TEST_ASSERT_EQUAL_PTR(ptr, shr_tagged_ptr_get_ptr(tagged_ptr));
    TEST_ASSERT_EQUAL_UINT32(tag, shr_tagged_ptr_get_tag(tagged_ptr));
}

void test_simple_tagged_ptr_die_on_unaligned(void)
{
    uintptr_t bad_ptr = 1;
    TEST_ASSERT_ABORTS("Pointer passed in was not byte-aligned", shr_tagged_ptr_create((void*) bad_ptr, 10));
}

void test_simple_tagged_ptr_die_on_tag_too_big(void)
{
    uint32_t bad_tag = (1 << SHR_TAG_BITS);
    TEST_ASSERT_ABORTS("Too much data in tag", shr_tagged_ptr_create(0, bad_tag));
}

void test_null(void)
{
    void* ptr = NULL;
    uint32_t tag = 10;
    shr_tagptr_t tagged_ptr = shr_tagged_ptr_create(ptr, tag);
    TEST_ASSERT_NULL(shr_tagged_ptr_get_ptr(tagged_ptr));
    TEST_ASSERT_EQUAL_UINT32(tag, shr_tagged_ptr_get_tag(tagged_ptr));
}

void test_linked_list(void)
{
    shr_tagptr_t* linked_list;
    uint32_t n = 524288;
    shr_tagptr_t* head = malloc(n * sizeof(shr_tagptr_t));
    shr_tagptr_t* tail = head;
    for (uint32_t i = 0; i < n-1; i++)
    {
        shr_tagptr_t* next_linked_list = &head[i+1];
        *tail = shr_tagged_ptr_create(next_linked_list, i);
        tail = next_linked_list;
    }
    *tail = shr_tagged_ptr_create(NULL, n-1);
    linked_list = head;

    uint32_t i = 0;
    for(; i < n; i++)
    {
        if (linked_list == NULL)
        {
            break;
        }
        PROD_ASSERT(i == shr_tagged_ptr_get_tag(*linked_list));
        shr_tagptr_t* ptr = shr_tagged_ptr_get_ptr(*linked_list);
        /* INFO("%p", shr_tagged_ptr_get_ptr(*linked_list)); */
        if (ptr == NULL)
        {
            break;
            TEST_ASSERT_NULL(ptr);
        }
        linked_list = ptr;
    }
    TEST_ASSERT_EQUAL(n-1, i);
    TEST_ASSERT_NULL(shr_tagged_ptr_get_ptr(*linked_list));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_simple_tagged_ptr_roundtrip);
    RUN_TEST(test_simple_tagged_ptr_die_on_unaligned);
    RUN_TEST(test_simple_tagged_ptr_die_on_tag_too_big);
    RUN_TEST(test_null);
    RUN_TEST(test_linked_list);
    return UNITY_END();
}
