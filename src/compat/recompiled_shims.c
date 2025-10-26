#include "recomp.h"

// Converts a numeric string at a0 into a 32-bit value in v0.
void string_to_u32_recomp(uint8_t* rdram, recomp_context* ctx) {
    gpr addr = ctx->r4; // a0
    char buf[64];
    unsigned i = 0;
    while (i < sizeof(buf) - 1) {
        char c = (char)MEM_BU(0, addr);
        if (c == '\0') break;
        buf[i++] = c;
        addr = ADD32(addr, 1);
    }
    buf[i] = '\0';

    char* endptr = NULL;
    unsigned long val = strtoul(buf, &endptr, 0);
    (void)endptr;
    ctx->r2 = (gpr)(int32_t)(uint32_t)val; // v0
}

// No-op sender; used by a few debug or message paths in the game.
void send_recomp(uint8_t* rdram, recomp_context* ctx) {
    (void)rdram;
    (void)ctx;
}
