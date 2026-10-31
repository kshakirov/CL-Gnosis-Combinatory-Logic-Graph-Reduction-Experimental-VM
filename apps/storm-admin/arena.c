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


void aux_create_arena_node(int idx, uint32_t tag, uint32_t left, uint32_t right, uint32_t payload ){
  arena[idx].tag = tag;
  arena[idx].left= left;
  arena[idx].right= right;
  arena[idx].payload = payload;
  
}



void aux_add_string_to_obj_pool(const char* string, const int length, const uint32_t offset ){
  object_pool[offset] = length;
  for (uint32_t i=offset + 1; i <= offset + length; i ++){
    object_pool[i] = string[i - offset -1];
  }
}

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

char* topologies[] ={"Core M","Test a","Test b"} ;
void init_arena(void) {
  aux_add_string_to_obj_pool(topologies[0], 6, 0);
  aux_create_arena_node(0, ATOM_STRING, 0, 0, 0);
  aux_add_string_to_obj_pool(topologies[1], 6, 7);
  aux_create_arena_node(1, ATOM_STRING, 0, 0, 7);
  aux_add_string_to_obj_pool(topologies[2], 6, 14);
  aux_create_arena_node(2, ATOM_STRING, 0, 0, 14);
  dirty_count = 0;
}
