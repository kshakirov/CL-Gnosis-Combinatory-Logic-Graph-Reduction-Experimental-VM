#include <stdint.h>

#define ARENA_SIZE 64
#define OBJECT_POOL_SIZE 1024
#define DIRTY_SIZE 64

typedef enum {
    ATOM_NUMBER = 1,       // Листовой узел: значение, число или комбинатор (S, K, I)
    ATOM_STRING=2,
    COMBINATION = 3 // Узел аппликации (@): связывает left и right

} NodeType;


typedef struct {
    uint32_t  tag;        // NODE_ATOM (0) или NODE_COMBINATION (1)
    uint32_t left;       // Индекс левого ребенка (если NODE_COMBINATION)
    uint32_t right;      // Индекс правого ребенка (если NODE_COMBINATION)
    uint32_t payload;    // Значение (если NODE_ATOM)
} Node;


Node arena[ARENA_SIZE] ;
static uint32_t dirty_nodes[DIRTY_SIZE];
static uint32_t dirty_count = 0;

uint32_t get_arena_base(void) {
    return (uint32_t)(uintptr_t)arena;
}

uint32_t* get_dirty_nodes_ptr(void) {
    return dirty_nodes;
}

uint32_t get_dirty_count(void) {
    return dirty_count;
}

uint8_t object_pool[OBJECT_POOL_SIZE];

uint8_t* get_object_pool_ptr(void){
  return object_pool;
}



static void mark_dirty(uint32_t node_id) {
  if (dirty_count < DIRTY_SIZE) {
    dirty_nodes[dirty_count++] = node_id;
  }
}

void init_arena(void) {
  arena[1].tag = ATOM_STRING;
  arena[1].left = 0;
  arena[1].right = 0;
  arena[1].payload = 100;

  arena[2].tag = ATOM_NUMBER;
  arena[2].left = 0;
  arena[2].right = 0;
  arena[2].payload = 2;

  object_pool[0] =2;
  object_pool[1] ='h';
  object_pool[2] ='i';
}


void increment_node(uint32_t node_id){
  if(node_id < ARENA_SIZE && arena[node_id].tag == ATOM_NUMBER){
    arena[node_id].payload  += 1;
    mark_dirty(node_id);
  }
}

void clean_dirty_nodes(void){
  for(uint32_t i=0; i <dirty_count; i++){
    dirty_nodes[i] =0; 
  }
  dirty_count =0;
}
