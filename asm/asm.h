#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint64_t ebx;
    uint64_t ecx;
    uint64_t edx;
    uint64_t esi;
    uint64_t edi;
    uint64_t ebp;
    uint64_t esp;
    uint64_t eip;
} asm_jmp_buf;

int64_t asm_add(int64_t a, int64_t b);
int     asm_popcnt(uint64_t x);
void   *asm_memcpy(void *dest, const void *src, size_t n);
int     asm_setjmp(asm_jmp_buf env);
void    asm_longjmp(asm_jmp_buf env, int val);
