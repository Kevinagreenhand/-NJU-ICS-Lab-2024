#include "common.h"
#include <inttypes.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions
typedef struct {
  bool validbit;
  bool dirty;
  uint64_t tag;
  uint32_t data[BLOCK_SIZE>>2];
} ACacheLine;

ACacheLine* cachearr;
static int associativity_size=0;
static int group_num_width=0;
uint32_t cache_read(uintptr_t addr) {
  return 0;
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
}

void init_cache(int total_size_width, int associativity_width) {
  associativity_size = exp2(associativity_width);
  group_num_width = total_size_width - associativity_width-BLOCK_SIZE;
  cachearr = (ACacheLine*)malloc(exp2(group_num_width)*associativity_size*sizeof(ACacheLine));
  for(int i=0;i<exp2(group_num_width)*associativity_size;i++) {
    //在把validbit设置为false的同时，进行了一定的初始化。
    cachearr[i].validbit=false;
    cachearr[i].dirty=false;
    cachearr[i].tag=0;
    for(int j=0;j<BLOCK_SIZE>>2;j++) {
      cachearr[i].data[j]=0;
    }
  }
}

//这个函数疑似不需要写，先空着
void display_statistic(void) {
}
