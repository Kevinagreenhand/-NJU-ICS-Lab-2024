#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

static bool is_prime[N];
static int  primes[N];

int *sieve(int n) {
  //使用了欧拉筛
  int tmpnum=0;
  //这里我为了降低程序运行的时间，用了一个非常奇怪的操作。因为is_prime这个布尔数组
  //的初始化是全为false的，而我们的筛法一开始要把整个数组都设为true，这样需要遍历
  //一遍整个数组，会额外消耗时间。所以在这里我把所有应该是true的改成false，应该是
  //false的改为true，之后把所有的条件表达式取反。这样就可以避免循环初始化浪费时间。
  is_prime[1] = true; 
  for(int i = 2; i <= n; ++i) 
  {
    if(!is_prime[i])
    {
      primes[tmpnum] = i;
      tmpnum++;
    }
    for(int j = 0; j <= tmpnum-1 && i * primes[j] <= n; j++)
    {
      is_prime[i * primes[j]] = true;
      //if(i % primes[j] == 0) 
      //  break;
    }
  }
  primes[tmpnum]=0;
  return primes;
}
