#include <stdint.h>

#define ARENA_SIZE 1024
#define OBJECT_POOL_SIZE (64 * 1024)
#define DIRTY_SIZE 1024

typedef enum {
    ATOM_NUMBER = 1,
    ATOM_STRING = 2,
    COMBINATION = 3
} NodeType;

typedef struct {
    uint32_t tag;
    uint32_t left;
    uint32_t right;
    uint32_t payload;
} Node;

static Node arena[ARENA_SIZE];
static uint8_t object_pool[OBJECT_POOL_SIZE];
static uint32_t dirty_nodes[DIRTY_SIZE];
static uint32_t dirty_count;

uint32_t get_arena_base(void) {
    return (uint32_t)(uintptr_t)arena;
}

uint32_t get_object_pool_ptr(void) {
    return (uint32_t)(uintptr_t)object_pool;
}

uint32_t get_dirty_nodes_ptr(void) {
    return (uint32_t)(uintptr_t)dirty_nodes;
}

uint32_t get_dirty_count(void) {
    return dirty_count;
}

void clear_dirty_nodes(void) {
    dirty_count = 0;
}

void init_arena(void) {
    dirty_count = 0;
}
