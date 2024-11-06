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
//经过STFW，有专门针对内存复制的指令，这里直接使用
asm (
"rep movsb;"
: "+D"(dest), "+S"(src), "+c"(n)
:
: "memory"
);
  return dest;
}

int asm_setjmp(asm_jmp_buf env) {
    uint64_t tmp=0;
    asm volatile(
    //由于不太熟练，写的有点丑陋复杂，请助教老师读完我的注释再看代码
    //使用默认存第一个参数的rdi存env，rax类似tmp变量
    //把add和mov合成一条mov会报错，原因不清楚

    //待完善，如果用方括号形式让gcc分配8个寄存器代表env的8个元素，会出现段错误，set_jmp返回值无法修改
    //初步怀疑gcc分配寄存器出现问题，导致某个或者某几个寄存器被重复使用
      "push %%rbp;"
      "mov %%rsp,%%rbp;"
      "movq 0x8(%%rbp),%%rax;" 
      "movq %%rax,(%[a]);"
      "movq (%%rbp),%%rax;"
      "addq $0x8, %[a];"
      "movq %%rax,(%[a]);"
      "movq %%rsp, %%rax;"
      "addq $0x10, %%rax;"
      "addq $0x8, %[a];"
      "movq %%rax,(%[a]);"
      "addq $0x8, %[a];"
      "movq %%rbx,(%[a]);"
      "addq $0x8, %[a];"
      "movq %%r12,(%[a]);"
      "addq $0x8, %[a];"
      "movq %%r13,(%[a]);"
      "addq $0x8, %[a];"
      "movq %%r14,(%[a]);"
      "addq $0x8, %[a];"
      "movq %%r15,(%[a]);"
      "pop %%rbp;"
    :
    :[a] "D" (env)
  );
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  //由于不太熟练，写的有点丑陋复杂，请助教老师读完我的注释再看代码
  //使用默认存第一个参数的rdi存env，存第二个参数的rsi存val
  //通过改rax的方式让set_jmp返回val值，改完rax之后rsi没用了，又用它存env里的pc值来跳转

  //待完善，如果用方括号形式让gcc分配8个寄存器代表env的8个元素，会出现段错误，set_jmp返回值无法修改
  //初步怀疑gcc分配寄存器出现问题，导致某个或者某几个寄存器被重复使用
  //把add和mov合成一条mov会报错，原因不清楚
  uint64_t tmp=0;
  asm volatile(
    "movq %%rsi,%[b];"
    "movq (%[a]),%%rsi;"
    "addq $0x8, %[a];"
    "movq (%[a]),%%rbp;"
    "addq $0x8, %[a];"
    "movq (%[a]),%%rsp;"
    "addq $0x8, %[a];"
    "movq (%[a]),%%rbx;"
    "addq $0x8, %[a];"
    "movq (%[a]),%%r12;"
    "addq $0x8, %[a];"
    "movq (%[a]),%%r13;"
    "addq $0x8, %[a];"
    "movq (%[a]),%%r14;"
    "addq $0x8, %[a];"
    "movq (%[a]),%%r15;"
    "jmp *%%rsi;"
    :
    :[a] "D" (env),"rsi"(val),[b] "a" (tmp)
  );
}