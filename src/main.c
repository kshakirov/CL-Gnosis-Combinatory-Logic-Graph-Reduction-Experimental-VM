
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <string.h>  // ✅ В C используется .h
enum {
  INVALID,
  ATOM,
  APP
};

struct Node {
  uint32_t tag;
  uint32_t lindex;
  uint32_t rindex;
  uint32_t reserved;
    
};


extern void curry_app(
    struct Node *arena,
    uint32_t destination,
    uint32_t scale,
    uint32_t left,
    uint32_t right
);


int main(void){
  printf("Approaching Curry\n");
  static_assert(sizeof(struct Node) == 16);
  struct Node arena[3]= {0};
  arena[0].tag = ATOM;
  arena[1].tag = ATOM;
  
  curry_app(arena,2,sizeof(struct Node),0,1);
  //  assert(arena_1.tag == arena_2.tag);
  //curry_app(&arena_1,1,0,0);
  printf("The arena now contains  2 atoms and on ob applcation its tag is  %d the left index is  %d the right is %d \n", arena[2].tag, arena[2].lindex, arena[2].rindex);
  
  return 0;
}
