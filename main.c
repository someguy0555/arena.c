#include <stdio.h>
#include "arena.h"

typedef struct
{
    char body[1024];
}
SerialObject;

typedef struct
{
    int32_t x;
    int32_t y;
}
Vec2D;

int main()
{
    SerialObject objs[10];
    Vec2D vec;
    Arena arena = init_arena();

    size_t serial_off = push_arena(&arena, sizeof(SerialObject)* 10, &objs);
    size_t vec_off    = push_arena(&arena, sizeof(Vec2D), &vec);

    printf("%ld:%ld:%ld\n", serial_off, vec_off, arena.capacity);
    return 0;
}
