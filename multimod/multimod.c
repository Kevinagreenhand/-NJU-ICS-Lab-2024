#include <stdint.h>
#include<stdbool.h>
//the functions below are provided by the guide web ,not from copying others' code
static inline uint64_t bit_of(uint64_t x,uint64_t i) {
  return (x >> i) & 1;
}
uint64_t mymod(uint64_t x,uint64_t m){
  if(x<m)
    return x;
  uint64_t xpos=0;
  uint64_t mpos=0;
  uint64_t tmpx=x;
  for(int i=63;i>=0;i--){
    if(bit_of(m,i)){
      mpos=i;
      break;
    }
  }
  while(true){
  for(int i=63;i>=0;i--){
    if(bit_of(tmpx,i)){
      xpos=i;
      break;
    }
  }
  if(xpos>mpos){
    if((m<<(xpos-mpos))<=tmpx)
      tmpx-=(m<<(xpos-mpos));
    else{
      tmpx-=(m<<(xpos-mpos-1));
    }
  }
  else
    tmpx-=m;
  if(tmpx<m)
    break;
  }
  return tmpx;
}
bool wrapbound(uint64_t x,uint64_t i)
{for(uint64_t j=64-i;j<64;j++){
        if(bit_of(x,j)){
          return true;
        }
      }
return false;}
uint64_t wrapboundshift(uint64_t x,uint64_t m,uint64_t i){
  uint64_t tag=0;
  uint64_t tmpx=x;
  uint64_t tmpi=i;
  while(true){
  for(int j=63;j>=0;j--){
    if(bit_of(tmpx,j)){
      tag=j;
      break;
    }
  }
  if(tmpi<=63-tag){
    tmpx=mymod(tmpx<<tmpi,m);
    break;
  }
  tmpx=mymod(tmpx<<(63-tag),m);
  if(tag==63&&m>tmpx)
    {tmpx=mymod(mymod(UINT64_MAX,m)+(m!=1?1:0)+mymod((tmpx-(1ULL<<63))<<1,m),m);
    tmpi-=1;}
  tmpi-=(63-tag);
  tag=0;
  }
  return tmpx;
}
uint64_t wrapboundadd(uint64_t x,uint64_t y,uint64_t m){
  uint64_t result=x+y;
  if(result<x||result<y)
    return mymod(mymod(UINT64_MAX,m)+(m!=1?1:0)+mymod(result,m),m);
  return mymod(result,m);
}
uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  uint64_t result=0;
  uint64_t tmp=0;
  uint64_t tmpb=mymod(b,m);
  uint64_t tmpa=mymod(a,m);
  for(int i=0;i<64;i++){
    if(bit_of(tmpa,i)){
      if (wrapbound(tmpb,i)){
        tmp=wrapboundshift(tmpb,m,i);}
      else{
        tmp=mymod(tmpb<<i,m);
      }
      result=wrapboundadd(result,tmp,m);
      tmp=0;
    }
  }
  return mymod(result,m);
}

