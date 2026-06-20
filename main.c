#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "arena.h"

typedef struct
{
    int a;
    double b;
    char c;
}
TestStruct;

int main(void)
{
    Arena arena;
    arena_init(&arena);

    puts("=== Basic integer test ===");

    int x = 123;
    int* px = arena_push(&arena, &x, sizeof(x));

    assert(px != NULL);
    assert(*px == 123);

    puts("OK");

    puts("=== Multiple allocations ===");

    int values[100];

    for (int i = 0; i < 100; i++)
    {
        values[i] = i * 10;

        int* ptr = arena_push(
            &arena,
            &values[i],
            sizeof(values[i])
        );

        assert(ptr != NULL);
        assert(*ptr == values[i]);
    }

    puts("OK");

    puts("=== Struct test ===");

    TestStruct ts =
    {
        .a = 42,
        .b = 3.14159,
        .c = 'A'
    };

    TestStruct* pts =
        arena_push(&arena, &ts, sizeof(ts));

    assert(pts != NULL);
    assert(pts->a == 42);
    assert(pts->b == 3.14159);
    assert(pts->c == 'A');

    puts("OK");

    puts("=== Pointer stability test ===");

    int first = 111;
    int* first_ptr =
        arena_push(&arena, &first, sizeof(first));

    for (int i = 0; i < 10000; i++)
    {
        int tmp = i;
        arena_push(&arena, &tmp, sizeof(tmp));
    }

    assert(*first_ptr == 111);

    puts("OK");

    puts("=== Large allocation test ===");

    size_t big_size =
        ARENA_MINIMUM_BLOCK_CAPACITY * 2;

    char* big = malloc(big_size);

    memset(big, 0xAA, big_size);

    char* arena_big =
        arena_push(&arena, big, big_size);

    assert(arena_big != NULL);

    for (size_t i = 0; i < big_size; i++)
    {
        assert(arena_big[i] == (char)0xAA);
    }

    free(big);

    puts("OK");

    puts("=== Alignment test ===");

    double d = 123.456;

    double* pd =
        arena_push(&arena, &d, sizeof(d));

    assert(pd != NULL);

    uintptr_t addr = (uintptr_t)pd;

    printf("double address = %p\n", (void*)pd);

    assert(addr % ARENA_ALIGNMENT == 0);

    puts("OK");

    puts("=== Arena statistics ===");

    printf("Block count: %zu\n", arena.size);

    for (size_t i = 0; i < arena.size; i++)
    {
        ArenaBlock* b = &arena.body[i];

        printf(
            "Block %zu: size=%zu capacity=%zu\n",
            i,
            b->size,
            b->capacity
        );
    }

    arena_free(&arena);

    puts("All tests passed.");

    return 0;
}
