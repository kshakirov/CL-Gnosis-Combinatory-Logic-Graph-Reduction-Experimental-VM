#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
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

int main(void){
  printf("Approaching Curry\n");
  static_assert(sizeof(struct Node) == 16);
  struct Node arena[2];
  struct Node x;
  x.tag = ATOM;
  arena[0]=x;
  arena[1].tag=INVALID;
  printf("The arena now contains one real node %d and one invalid %d \n", arena[0].tag, arena[1].tag);

  return 0;
}
