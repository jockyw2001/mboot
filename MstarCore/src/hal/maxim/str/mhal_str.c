#include <mhal_str.h>

void Mhal_StrSetGICInterface(void)
{
    __asm__ volatile (
        "ldr     r3, =0x16001080\n\t"
        "ldr     r2, [r3]\n\t"
        "ldr     r4, =(0x3 << 30)\n\t"
        "orr     r2, r2, r4\n\t"
        "str     r2, [r3]\n\t"
        "ldr     r3, =0x16001000\n\t"
        "mov     r2, #3\n\t"
        "str     r2, [r3]\n\t"
        "ldr     r3, =0x16002000\n\t"
        "mov     r2, #3\n\t"
        "str     r2, [r3]\n\t"
        "ldr     r3, =0x16002004\n\t"
        "mov     r2, #0xf8\n\t"
        "str     r2, [r3]\n\t"
   );
}
