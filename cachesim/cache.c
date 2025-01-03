#include "common.h"
#include <inttypes.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions
typedef struct {
  bool validbit;
  bool dirtybit;
  uint32_t tag;
  //uint8_t不好读取，改成uint32_t
  uint32_t data[BLOCK_SIZE>>2];
} ACacheLine;

ACacheLine* cachearr;
static uint32_t associativity_size=0;
static uint32_t group_num_width=0;
static uint64_t random_replace_a_line(uint64_t addr,uint64_t group_index,uint32_t tag){
  for(uint64_t i=group_index*associativity_size;i<(group_index+1)*associativity_size;i++){
    if (cachearr[i].validbit==false){
      cachearr[i].validbit=true;
      cachearr[i].dirtybit=false;
      cachearr[i].tag=tag;
      mem_read(addr>>BLOCK_WIDTH,(uint8_t*)(cachearr[i].data));
      return i;
    }
  }
  uint64_t replace_index=rand()%associativity_size+group_index*associativity_size;
  cachearr[replace_index].validbit=false;
  if(cachearr[replace_index].dirtybit==true){
    mem_write((cachearr[replace_index].tag<<group_num_width)+group_index,(uint8_t*)(cachearr[replace_index].data));
    cachearr[replace_index].dirtybit=false;
  }
  mem_read(addr>>BLOCK_WIDTH,(uint8_t*)(cachearr[replace_index].data));
  cachearr[replace_index].tag=tag;
  cachearr[replace_index].dirtybit=false;
  cachearr[replace_index].validbit=true;
  return replace_index;
}
uint32_t cache_read(uintptr_t addr) {
  uint32_t tag=addr>>(group_num_width+BLOCK_WIDTH);
  uint64_t group_index=(addr>>BLOCK_WIDTH)&((uint64_t)((1<<group_num_width)-1));
  uint64_t group_addr=(addr&0x3f)>>2;
  bool findhelp=false;
  uint64_t findrecord=0;
  for(int i=group_index*associativity_size;i<(group_index+1)*associativity_size;i++){
    if (cachearr[i].validbit==true&&cachearr[i].tag==tag){
      findrecord=i;
      findhelp=true;
      break;}
  }
  if(findhelp!=false){
      return cachearr[findrecord].data[group_addr];
  }
  else
    return cachearr[random_replace_a_line(addr,group_index,tag)].data[group_addr];
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  // if(addr==0x1fbd23a ) printf("write 8\n %x",wmask);
  // printf("write %lx\n",addr);
  uint32_t tag=addr>>(group_num_width+BLOCK_WIDTH);//tag
  //get group number
  uint32_t temp=(1<<group_num_width)-1;
  uint32_t group_num=(addr>>BLOCK_WIDTH)&temp;//组号
  uint32_t group_addr=(addr&0x3f)>>2;//组内地址 32wei 
  // if(addr==0x1fbd23a ) printf("\n%d %d \n",group_num,group_addr);
  int line_number=-1; //行号
  int every_group=exp2(associativity_size);
  int start=group_num*every_group;
  int end=(group_num+1)*(every_group);
  for(int i=start;i<end;i++){//需要<不然会找到下一组
    if(cachearr[i].tag==tag){
      line_number=i;
      }
  }
  if(line_number!=-1){//has data
    cachearr[line_number].dirtybit=1;
    cachearr[line_number].data[group_addr] = ((cachearr[line_number].data[group_addr])&(~wmask))|(data & wmask);
    // cache[line_number].data[group_addr] = (data & wmask);
  }
  else{//write new
    int new_line=random_replace_a_line(addr,group_num,tag);
    // if(addr==0x1fbd23a ) printf("new is %d\n",new_line);
    cachearr[new_line].dirtybit=1;
    cachearr[new_line].data[group_addr] = ((cachearr[new_line].data[group_addr])&(~wmask))|(data & wmask);
    // cache[line_number].data[group_addr] = (data & wmask);
  }
}

void init_cache(int total_size_width, int associativity_width) {
  associativity_size = exp2(associativity_width);
  group_num_width = total_size_width - associativity_width-BLOCK_SIZE;
  cachearr = (ACacheLine*)malloc(exp2(group_num_width)*associativity_size*sizeof(ACacheLine));
  for(int i=0;i<exp2(group_num_width)*associativity_size;i++) {
    cachearr[i].validbit=false;
    cachearr[i].dirtybit=false;
    //cachearr[i].tag=0;
    //for(int j=0;j<BLOCK_SIZE;j++) {
      //cachearr[i].data[j]=0;
    //}
  }
}

//这个函数疑似不需要写，先空着
void display_statistic(void) {
}