#include <stdint.h>

#define ARENA_SIZE 100000


// Тег типа узла (Атом или Комбинация)
typedef enum {
  ATOM_NUMBER = 0,       // Листовой узел: значение, число или комбинатор (S, K, I)
  COMBINATION = 1, // Узел аппликации (@): связывает left и right
  ATOM_STRING=2
} NodeType;

#define DIRTY_SIZE 1024


// Компактный 32-битный узел Арены (все адреса — сдвиги/индексы)
typedef struct {
  uint8_t  tag;        // NODE_ATOM (0) или NODE_COMBINATION (1)
  uint32_t left;       // Индекс левого ребенка (если NODE_COMBINATION)
  uint32_t right;      // Индекс правого ребенка (если NODE_COMBINATION)
  uint32_t payload;    // Значение (если NODE_ATOM)
} Node;


Node arena[ARENA_SIZE];
// Статическая бинарная память (Zero allocations)

static uint32_t dirty_nodes[DIRTY_SIZE];
static uint32_t dirty_count = 0;

enum {
  PAGE_A = 0,
  INFO_A = 1,
  GO_A = 2,
  SHOW_A = 3,
  PAGE_B = 4,
  INFO_B = 5,
  GO_B = 6,
  SHOW_B = 7

};

#define PAGE_A_ROOT 10
#define PAGE_B_ROOT 13


static uint32_t active_page_root = PAGE_A_ROOT;

uint32_t get_active_page_root(void) {
    return active_page_root;
}

void  change_root_page(void){
  if(active_page_root== PAGE_A_ROOT)
    active_page_root=  PAGE_B_ROOT;
  else
    active_page_root = PAGE_A_ROOT;
}
void init_arena(void) {

  arena[0].payload = PAGE_A;
  arena[0].tag = ATOM_NUMBER;
  arena[1].payload = INFO_A;
  arena[1].tag = ATOM_NUMBER;
  arena[2].payload = GO_A;
  arena[2].tag = ATOM_NUMBER;
  arena[3].payload = SHOW_A;
  arena[3].tag = ATOM_NUMBER;  
  arena[4].payload = PAGE_B;
  arena[4].tag = ATOM_NUMBER;
  arena[5].payload = INFO_B;
  arena[5].tag = ATOM_NUMBER;
  arena[6].payload = GO_B;
  arena[6].tag = ATOM_NUMBER;
  arena[7].payload = SHOW_B;
  arena[7].tag = ATOM_NUMBER;

  // now combinations

  arena[8].left = 0;
  arena[8].right = 1;
  arena[8].tag = COMBINATION;
  arena[9].left = 8;
  arena[9].right = 2;
  arena[9].tag = COMBINATION;
  arena[10].left = 9;
  arena[10].right = 3;
  arena[10].tag = COMBINATION;
  //on purpose here


  arena[11].left = 4;
  arena[11].right = 5;
  arena[11].tag = COMBINATION;
  arena[12].left = 11;
  arena[12].right = 6;
  arena[12].tag = COMBINATION;
  arena[13].left = 12;
  arena[13].right = 7;
  arena[13].tag = COMBINATION;

  
}

uint8_t get_node_tag(uint32_t node_id) {
    return arena[node_id].tag;
}

uint32_t get_node_left(uint32_t node_id) {
    return arena[node_id].left;
}

uint32_t get_node_right(uint32_t node_id) {
    return arena[node_id].right;
}

uint32_t get_node_payload(uint32_t node_id) {
    return arena[node_id].payload;
}
