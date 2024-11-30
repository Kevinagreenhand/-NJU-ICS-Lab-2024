#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

static bool is_prime[N];
static int  primes[N];

int *sieve(int n) {
  int tmpnum=0;
  is_prime[1] = true; 
    for(int i = 2; i <= n; ++i) 
    {
        if(!is_prime[i]){ 
          primes[tmpnum] = i;
          tmpnum++;}
        for(int j = 0; j <= tmpnum-1 && i * primes[j] <= n; ++j)
        {
            is_prime[i * primes[j]] = true;
            if(i % primes[j] == 0) 
              break;
        }
    }
  primes[tmpnum]=0;
  return primes;
}
