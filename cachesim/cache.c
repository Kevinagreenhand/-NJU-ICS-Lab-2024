#include "common.h"
#include <inttypes.h>
#include<assert.h>
void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);
static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }


typedef struct 
{
  bool validbit;
  bool dirtybit;
  uint32_t tag;
  //为了配合mem_read mem_write，这里用32位，所以右移2
  uint32_t data[BLOCK_SIZE>>2];
}ACacheLine;

static ACacheLine *cachearr;
static uint64_t associativity_size;
static uint64_t group_nums_size=0;

// TODO: implement the following functions
int line_choose(uintptr_t addr,uint32_t group_index,uint32_t tag){
      for(int i=group_index*associativity_size;i<(group_index+1)*associativity_size;i++){
      if(cachearr[i].validbit==false){
        uintptr_t mem_addr=(addr>>BLOCK_WIDTH);
        mem_read(mem_addr,(uint8_t*)(cachearr[i].data));
        cachearr[i].validbit=true;
        cachearr[i].dirtybit=false;
        cachearr[i].tag=tag;
        return i;
      }
  }
    int replace_index=group_index*associativity_size+rand()%associativity_size;
    cachearr[replace_index].validbit=false;
    if(cachearr[replace_index].dirtybit==true){
      uintptr_t mem_addr=(cachearr[replace_index].tag<<group_nums_size)+group_index;
      mem_write(mem_addr,(uint8_t*)(cachearr[replace_index].data));
      cachearr[replace_index].dirtybit=false;}
    uintptr_t  mem_addr=(addr>>BLOCK_WIDTH);
    mem_read(mem_addr,(uint8_t*)(cachearr[replace_index].data));
    cachearr[replace_index].validbit=true;
    cachearr[replace_index].dirtybit=false;
    cachearr[replace_index].tag=tag;
    return replace_index;
}

uint32_t cache_read(uintptr_t addr) {
  uint32_t tag=addr>>(group_nums_size+BLOCK_WIDTH);
  uint32_t group_index=(addr>>BLOCK_WIDTH)&((1<<group_nums_size)-1);
  uint32_t group_addr=(addr&0x3f)>>2;
  int line_index=-114514;
  for(int i=group_index*associativity_size;i<(group_index+1)*associativity_size;i++){
    if(cachearr[i].tag==tag&&cachearr[i].validbit==true){
      line_index=i;
      break;}
  }
  if(line_index==-114514){
    line_index=line_choose(addr,group_index,tag);
  }
  return cachearr[line_index].data[group_addr];
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  uint32_t tag=addr>>(group_nums_size+BLOCK_WIDTH);
  uint32_t group_index=(addr>>BLOCK_WIDTH)&((1<<group_nums_size)-1);
  uint32_t group_addr=(addr&0x3f)>>2;
  bool is_hit=false;
  int line_index=-1919810; 
  for(int i=group_index*associativity_size;i<(group_index+1)*associativity_size;i++){
    if(cachearr[i].tag==tag){
      is_hit=true;
      line_index=i;
      break;
      }
  }
  if(is_hit==false){
    line_index=line_choose(addr,group_index,tag);
  }
  cachearr[line_index].dirtybit=true;
  cachearr[line_index].data[group_addr] &= (~wmask);
	cachearr[line_index].data[group_addr] |= (data & wmask);
}


void init_cache(int total_size_width, int associativity_width) {
  associativity_size=exp2(associativity_width);
  assert(total_size_width > associativity_width);
  group_nums_size=total_size_width-BLOCK_WIDTH-associativity_width;
  cachearr = (ACacheLine*)malloc(sizeof(ACacheLine) * exp2(total_size_width-BLOCK_WIDTH));
  for(int i=0;i<exp2(total_size_width-BLOCK_WIDTH);i++){
    cachearr[i].validbit=false;
    cachearr[i].dirtybit=false;
  }
}

void display_statistic(void) {
}