/*
NOTE!!!
If running on Windows, find & replace
"%rdi" -> "%rcx"
*/

#include<array>
#include<cstdint>
#include<immintrin.h>
using State = std::array<std::uint64_t,32>;
extern "C" void ljseed(State* state, __m512d seed);
asm(R"-(
.section .text
.globl ljseed
ljseed:
    # state in rdi, seed in zmm0 (linux convention)

    # load constants
    # ideally extract this part, maybe use zmm30/31
    # or some registers that isn't used anywhere else
    vbroadcastsd PI(%rip), %zmm1
    vbroadcastsd E(%rip), %zmm2
    
    vbroadcastsd MASK(%rip), %zmm10
    vbroadcastsd MASK2(%rip), %zmm11
    vbroadcastsd MASK3(%rip), %zmm12
    vbroadcastsd MASK4(%rip), %zmm13

    vmulpd %zmm0, %zmm1, %zmm0
    vaddpd %zmm0, %zmm2, %zmm0
    vmovapd %zmm0, %zmm3
    vpcmpuq $1, %zmm10, %zmm3, %k1
    vpaddq %zmm3, %zmm10, %zmm3{%k1}
    vmovapd %zmm3, (%rdi)

    vmulpd %zmm0, %zmm1, %zmm0
    vaddpd %zmm0, %zmm2, %zmm0
    vmovapd %zmm0, %zmm3
    vpcmpuq $1, %zmm11, %zmm3, %k1
    vpaddq %zmm3, %zmm11, %zmm3{%k1}
    vmovapd %zmm3, 64(%rdi)

    vmulpd %zmm0, %zmm1, %zmm0
    vaddpd %zmm0, %zmm2, %zmm0
    vmovapd %zmm0, %zmm3
    vpcmpuq $1, %zmm12, %zmm3, %k1
    vpaddq %zmm3, %zmm12, %zmm3{%k1}
    vmovapd %zmm3, 128(%rdi)

    vmulpd %zmm0, %zmm1, %zmm0
    vaddpd %zmm0, %zmm2, %zmm0
    vmovapd %zmm0, %zmm3
    vpcmpuq $1, %zmm13, %zmm3, %k1
    vpaddq %zmm3, %zmm13, %zmm3{%k1}
    vmovapd %zmm3, 192(%rdi)

.section .rodata
PI:
    .quad 0x400921fb54442d18   # Pi in 64-bit IEEE 754 format
E:
    .quad 0x4005bf0996a58c58   # e in 64-bit IEEE 754 format
MASK:
    .quad 2
MASK2:
    .quad 64
MASK3:
    .quad 512
MASK4:
    .quad 131072

.section .text
)-");