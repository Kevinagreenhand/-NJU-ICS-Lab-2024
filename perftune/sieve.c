#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

static bool is_prime[N];
static int  primes[N];

int *sieve(int n) {
  int cnt=0;
    for (int i = 0; i <= n; i++)
    is_prime[i] = true;
    is_prime[1] = false; // 1不是素数
    for(int i = 2; i <= n; ++i) // i从2循环到n（外层循环）
    {
        if(is_prime[i]){ 
          primes[cnt++] = i;}
        // 如果i没有被前面的数筛掉，则i是素数
        for(int j = 0; j <= cnt-1&& i * primes[j] <= n; ++j)
        // 筛掉i的素数倍，即i的prime[j]倍
        // j循环枚举现在已经筛出的素数（内层循环）
        {
            is_prime[i * primes[j]] = false;
            // 倍数标记为合数，也就是i用prime[j]把i * prime[j]筛掉了
            if(i % primes[j] == 0) break;
            // 最神奇的一句话，如果i整除prime[j]，退出循环
            // 这样可以保证线性的时间复杂度
        }
    }
    primes[cnt]=0;
//  for (int i = 2; i <= n; i++) {
//    for (int j = i + i; j <= n; j += i) {
//      is_prime[j] = false;
//    }
//  }
  return primes;
}
