#include "asm.h"
#include <string.h>
#include<stdio.h>

int64_t asm_add(int64_t a, int64_t b) {
  asm(
        "addq %[b], %[a]"
        : [a] "+r"(a)
        : [b] "r"(b)
        : 
    );
  return a;
}

int asm_popcnt(uint64_t x) {
int s = 0;
asm (
"xorl %[s], %[s];"
"movq %[x], %[a];"
"1:;"
"testq %[a], %[a];"
"jz 2f;"
"bt $0x0, %[a];"
"adc $0x0, %[s];"
"shrq $0x1, %[a];"
"jmp 1b;"
"2:;"
: [s] "+r"(s), [a] "=&r"(x)
: [x] "r"(x)
);
  return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  asm (
"rep movsb;"
: "+D"(dest), "+S"(src), "+c"(n)
:
: "memory"
);
  return dest;
}

int asm_setjmp(asm_jmp_buf env) {
    asm volatile(
      "push %%rbp;"
      "mov %%rsp,%%rbp;"
      "movq (%%rbp),%%rax;"
      "movq %%rax,(%%rdi);"
      "movq %%rsp, %%rax;"
      "addq $0x10, %%rax;"
      "addq $0x8, %%rdi;"
      "movq %%rax,(%%rdi);"
      "addq $0x8, %%rdi;"
      "movq %%rbx,(%%rdi);"
      "movq 0x8(%%rbp),%%rax;" 
      "addq $0x8, %%rdi;"
      "movq %%rax,(%%rdi);"
      "addq $0x8, %%rdi;"
      "movq %%r12,(%%rdi);"
      "addq $0x8, %%rdi;"
      "movq %%r13,(%%rdi);"
      "addq $0x8, %%rdi;"
      "movq %%r14,(%%rdi);"
      "addq $0x8, %%rdi;"
      "movq %%r15,(%%rdi);"
      "pop %%rbp;"
    :
    :"rdi"(env)
  );
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  asm volatile(
    "movq %%rsi,%%rax;"
    "movq (%%rdi),%%rbp;"
    "addq $0x8, %%rdi;"
    "movq (%%rdi),%%rsp;"
    "addq $0x8, %%rdi;"
    "movq (%%rdi),%%rbx;"
    "addq $0x8, %%rdi;"
    "movq (%%rdi),%%rsi;"
    "addq $0x8, %%rdi;"
    "movq (%%rdi),%%r12;"
    "addq $0x8, %%rdi;"
    "movq (%%rdi),%%r13;"
    "addq $0x8, %%rdi;"
    "movq (%%rdi),%%r14;"
    "addq $0x8, %%rdi;"
    "movq (%%rdi),%%r15;"
    "jmp *%%rsi;"
    :
    :"rdi"(env),"rsi"(val)
  );
}