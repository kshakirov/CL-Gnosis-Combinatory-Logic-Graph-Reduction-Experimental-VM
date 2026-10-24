#include <stdint.h>

#define ARENA_SIZE 100000
#define OBJECT_POOL_SIZE 64*1024


// Тег типа узла (Атом или Комбинация)
typedef enum {
    ATOM_NUMBER = 0,       // Листовой узел: значение, число или комбинатор (S, K, I)
    COMBINATION = 1, // Узел аппликации (@): связывает left и right
    ATOM_STRING=2
} NodeType;

#define DIRTY_SIZE 1024




typedef uint8_t OBJECT_POOL[OBJECT_POOL_SIZE];


// Компактный 32-битный узел Арены (все адреса — сдвиги/индексы)
typedef struct {
    uint8_t  tag;        // NODE_ATOM (0) или NODE_COMBINATION (1)
    uint32_t left;       // Индекс левого ребенка (если NODE_COMBINATION)
    uint32_t right;      // Индекс правого ребенка (если NODE_COMBINATION)
    uint32_t payload;    // Значение (если NODE_ATOM)
} Node;

// Статическая бинарная память (Zero allocations)

static uint32_t dirty_nodes[DIRTY_SIZE];
static uint32_t dirty_count = 0;
#define OBJECT_POOL_OFFSET ARENA_SIZE * 16
#define OBJECT_POOL_TOP 0


typedef struct {
  Node arena[ARENA_SIZE];
  uint8_t object_pool[OBJECT_POOL_SIZE];
} Memory;


Memory memory;
// Export: указатель на массив измененных узлов
uint32_t* get_dirty_nodes_ptr(void) {
    return dirty_nodes;
}

// Export: количество грязных узлов
uint32_t get_dirty_count(void) {
    return dirty_count;
}

static void mark_dirty(uint32_t node_id) {
    if (dirty_count < DIRTY_SIZE) {
        dirty_nodes[dirty_count++] = node_id;
    }
}

// Инициализация Арены
void init_arena(void) {
    dirty_count = 0;

    // Узел #42 — это Атом со стартовым значением 100
    memory.arena[42].tag = ATOM_NUMBER;
    memory.arena[42].left = 0;
    memory.arena[42].right = 0;
    memory.arena[42].payload = 0;

    // Узел #100 — это Комбинация (@), объединяющая узел 42 и узел 43
    memory.arena[100].tag = COMBINATION;
    memory.arena[100].left = 42;
    memory.arena[100].right = 43;

    memory.arena[1].tag=ATOM_STRING;
    memory.arena[1].left = 0;
    memory.arena[1].right=0;
    memory.arena[1].payload=0;

    memory.object_pool[0] =2;
    memory.object_pool[1] ='h';
    memory.object_pool[2] ='i';
    memory.object_pool[3] =3;
    memory.object_pool[4] ='b';
    memory.object_pool[5] ='y';
    memory.object_pool[6] ='e';
    

    
}

// Симуляция редукции: мутация Атома #42
void increment_node_42(void) {
    dirty_count = 0;

    // Проверяем, что узел 42 действительно Атом
    if (memory.arena[42].tag == ATOM_NUMBER) {
        memory.arena[42].payload += 1;
        
        // Арена сама фиксирует изменение Атома #42
        mark_dirty(42);


    }

    memory.arena[1].payload = memory.arena[42].payload % 2 == 0 ? 0 : 3 ;
    mark_dirty(1);
    /* if (memory.arena[1].tag == ATOM_STRING) { */
    /*   memory.arena[1].payload = OBJECT_POOL_TOP; */
    /*     mark_dirty(1); */
    
    /* } */

    
    
}

// Экспорт тега и payload для чтения из JS
uint8_t get_node_tag(uint32_t node_id) {
    return memory.arena[node_id].tag;
}

uint32_t get_node_payload_number(uint32_t node_id) {
  if(memory.arena[node_id].tag == ATOM_NUMBER){
    return memory.arena[node_id].payload;
  }
  return -1;
}

uint32_t get_node_payload_string_length(uint32_t node_id){
  if(memory.arena[node_id].tag == ATOM_STRING){
    return memory.object_pool[memory.arena[node_id].payload];
  }
    return -1;
}


uint32_t get_node_payload_string_pool(uint32_t node_id){
  if(memory.arena[node_id].tag == ATOM_STRING){
      return  memory.arena[node_id].payload + 1 ;
  }
    return -1;
}

uint32_t get_memory_base(void) {
    return (uint32_t)(uintptr_t)&memory;
}
