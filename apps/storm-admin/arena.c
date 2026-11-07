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

typedef enum {
  MAIN_PAGE= 100,
  PAGE_TOPOLOGY = 111
  } CURRENT_PAGE;




void aux_create_arena_node(int idx, uint32_t tag, uint32_t left, uint32_t right, uint32_t payload ){
  arena[idx].tag = tag;
  arena[idx].left= left;
  arena[idx].right= right;
  arena[idx].payload = payload;
  
}

void aux_create_combiantion(int idx, int l, int r){
  arena[idx].left = l;
  arena[idx].right = r;
  arena[idx].tag = COMBINATION;
  
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

  aux_create_arena_node(3, ATOM_NUMBER, 0, 0, MAIN_PAGE);
  aux_create_arena_node(4, ATOM_NUMBER, 0, 0, PAGE_TOPOLOGY);
  aux_create_arena_node(5, ATOM_NUMBER, 0, 0, PAGE_TOPOLOGY);
  aux_create_arena_node(6, ATOM_NUMBER, 0, 0, PAGE_TOPOLOGY);

  //main page subgraph
  aux_create_combiantion(7, 3, 0);
  aux_create_combiantion(8, 7, 1);
  aux_create_combiantion(9, 8, 2);
  

  //top  page 1 subgraph
  aux_create_combiantion(10, 4, 0);

  //top  page 2 subgraph
  aux_create_combiantion(11, 5, 1);

  //top  page 2 subgraph
  aux_create_combiantion(12, 6, 2);
  
  
  dirty_count = 0;
}
