#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
enum {
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
  return 0;
}
