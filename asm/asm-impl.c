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
        "mov %%rsp, %%rbp;"
        "movq %%rsp, %%rax;"
        "add $0x10, %%rax;"
        "movq %%rax, (%%rdx);"
        "movq (%%rbp), %%rax;"
        "movq %%rax, $0x8(%%rdx);"
        "movq 0x8(%%rbp), %%rax;"
        "movq %%rax, $0x10(%%rdx);"
        "movq %%rbx, $0x18(%%rdx);"
        "movq %%r12, $0x20(%%rdx);"
        "movq %%r13, $0x28(%%rdx);"
        "movq %%r14, $0x30(%%rdx);"
        "movq %%r15, $0x38(%%rdx);"
        "pop %%rbp;"
        : 
        :"rdx"(env)
        : "memory"
    );
    return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
    asm volatile(
    "movq (%%rdi), %%rsp;"
    "movq 0x8(%%rdi), %%rbp;"
    "movq 0x18(%%rdi), %%rbx;"
    "movq 0x20(%%rdi), %%r12;"
    "movq 0x28(%%rdi), %%r13;"
    "movq 0x30(%%rdi), %%r14;"
    "movq 0x38(%%rdi), %%r15;"
    "movq 0x10(%%rdi), %%rdi;"
    "movq %%rsi, %%rax;"
    "jmp *%%rdi;"
    :
    :"rsi"(val),"rdi"(env)
    : 
  );
  return;
}