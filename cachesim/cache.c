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
  uint32_t data[BLOCK_SIZE>>2];
}ACacheLine;

static ACacheLine *cachearr;
static uint64_t associativity_size;
static uint64_t group_nums_size=0;
void write_back(uint32_t group_number,uint32_t lines){
  cachearr[lines].validbit=false;
  if(cachearr[lines].dirtybit==1){
  uintptr_t mem_addr=(cachearr[lines].tag<<group_nums_size)+group_number;
  mem_write(mem_addr,(uint8_t*)(cachearr[lines].data));
  cachearr[lines].dirtybit=false;
  }
}

// TODO: implement the following functions
int line_choose(uintptr_t addr,uint32_t group_number,uint32_t tag){
    int every_group=associativity_size;
    int start=group_number*every_group;
    int end=(group_number+1)*every_group;
    assert(start>=0&&end>=0);
      for(int i=start;i<end;i++){
      if(cachearr[i].validbit==false){
        uintptr_t mem_addr=(addr>>BLOCK_WIDTH);
        mem_read(mem_addr,(uint8_t*)(cachearr[i].data));
        cachearr[i].validbit=true;
        cachearr[i].dirtybit=false;
        cachearr[i].tag=tag;
        return i;
      }
  }
    int random=rand()%every_group;
    int replace=start+random;
    assert(replace<=end);
    write_back(group_number,replace);
    uintptr_t  mem_addr=(addr>>BLOCK_WIDTH);
    mem_read(mem_addr,(uint8_t*)(cachearr[replace].data));
    cachearr[replace].validbit=true;
    cachearr[replace].dirtybit=false;
    cachearr[replace].tag=tag;
    return replace;
}

int find(uint32_t group_number,uint32_t tag){
  int every_group=associativity_size;
  int start=group_number*every_group;
  int end=(group_number+1)*(every_group);
  for(int i=start;i<end;i++){
      if(cachearr[i].tag==tag&&cachearr[i].validbit==true)  return i;
  }
  return -1;
}


uint32_t cache_read(uintptr_t addr) {
  uint32_t tag=addr>>(group_nums_size+BLOCK_WIDTH);
  uint32_t temp=(1<<group_nums_size)-1;
  uint32_t group_num=(addr>>BLOCK_WIDTH)&temp;
  uint32_t group_addr=(addr&0x3f)>>2;
  int line_number=find(group_num,tag);
  if(line_number!=-1){
      return cachearr[line_number].data[group_addr];
  }
  else{
    int line=line_choose(addr,group_num,tag);
    return cachearr[line].data[group_addr];
  }
  return 0;
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  uint32_t tag=addr>>(group_nums_size+BLOCK_WIDTH);
  uint32_t temp=(1<<group_nums_size)-1;
  uint32_t group_num=(addr>>BLOCK_WIDTH)&temp;
  uint32_t group_addr=(addr&0x3f)>>2;
  int line_number=-1; 
  int every_group=associativity_size;
  int start=group_num*every_group;
  int end=(group_num+1)*(every_group);
  for(int i=start;i<end;i++){
    if(cachearr[i].tag==tag){
      line_number=i;
      }
  }
  if(line_number!=-1){
    cachearr[line_number].dirtybit=true;
    cachearr[line_number].data[group_addr] &= (~wmask);
	  cachearr[line_number].data[group_addr] |= (data & wmask);
  }
  else{
    int new_line=line_choose(addr,group_num,tag);
    cachearr[new_line].dirtybit=true;
    cachearr[new_line].data[group_addr] &= (~wmask);
	  cachearr[new_line].data[group_addr] |= (data & wmask);
  }
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