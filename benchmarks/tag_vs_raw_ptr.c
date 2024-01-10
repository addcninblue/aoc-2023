#include <shared/b63/b63.h>
#include <shared/tagged_ptr.h>
#include <shared/logging.h>
#include <stdbool.h>

const int n = 524288;

struct linked_list
{
    int data;
    void* next_linked_list;
};

struct linked_list* _init_linked_lists(int num_elements, void*(*create_next_ptr)(void*))
{
    struct linked_list* head = malloc(num_elements * sizeof(struct linked_list));
    struct linked_list* tail = head;
    for (int i = 0; i < num_elements-1; i++)
    {
        struct linked_list* next_linked_list = &head[i+1];
        *tail = (struct linked_list) {.data = i, .next_linked_list = create_next_ptr(next_linked_list)};
        tail = next_linked_list;
    }
    tail->data = num_elements-1;
    tail->next_linked_list = NULL;
    return head;
}

void* baseline_create_next_ptr(void* ptr)
{
    return ptr;
}

void* tagged_create_next_ptr(void* ptr)
{
    return (void*) _shr_tagged_ptr_get_uintptr(shr_tagged_ptr_create(ptr, 0));
}

B63_BASELINE(raw_ptr, _n)
{
    struct linked_list* head;
    struct linked_list* linked_list;
    B63_SUSPEND
    {
        linked_list = _init_linked_lists(n, baseline_create_next_ptr);
        head = linked_list;
    }
    int i = 0;
    for(; i < n; i++)
    {
        if (linked_list == NULL)
        {
            break;
        }
        PROD_ASSERT(i == linked_list->data);
        linked_list = linked_list->next_linked_list;
    }
    PROD_ASSERT(i == n);
    B63_KEEP(linked_list);
    free(head);
}

B63_BENCHMARK(tagged_ptr, _n)
{
    shr_tagptr_t* linked_list = NULL;
    shr_tagptr_t* head = NULL;
    B63_SUSPEND
    {
        shr_tagptr_t* head = malloc(n * sizeof(shr_tagptr_t));
        shr_tagptr_t* tail = head;
        for (int i = 0; i < n-1; i++)
        {
            shr_tagptr_t* next_linked_list = &head[i+1];
            *tail = shr_tagged_ptr_create(next_linked_list, i);
            tail = next_linked_list;
        }
        *tail = shr_tagged_ptr_create(NULL, n-1);
        linked_list = head;
    }
    int i = 0;
    for(; i < n; i++)
    {
        if (linked_list == NULL)
        {
            break;
        }
        PROD_ASSERT(i == shr_tagged_ptr_get_tag(*linked_list));
        linked_list = shr_tagged_ptr_get_ptr(*linked_list);
    }
    PROD_ASSERT(i == n);
    B63_KEEP(linked_list);
    free(head);
}

int main(int argc, char **argv) {
    B63_RUN(argc, argv);
    return 0;
}
