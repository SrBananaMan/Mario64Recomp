#include <cstdint>
#include "recomp.h"

// Forward declare before use to avoid undeclared identifier errors.
gpr get_entrypoint_address();
static gpr get_entry_function_address();

extern "C" void recomp_entrypoint(uint8_t * rdram, recomp_context * ctx) {
    SET_FENV_ACCESS();
    // Ensure we have a valid initial stack pointer for the first function.
    // N64 RDRAM is 0x80000000 - 0x80800000 (8MB). Use a high address as SP if unset.
    if (ctx->r29 == 0) {
        ctx->r29 = (gpr)(int32_t)0x807FF000u;
    }
    // Jump to the recompiled program's entry function.
    recomp_func_t* entry = LOOKUP_FUNC(get_entry_function_address());
    entry(rdram, ctx);
}

// Provide the entrypoint address used at startup.
gpr get_entrypoint_address() {
    // Super Mario 64 (US) ROM is mapped with main section base at 0x80246000.
    return (gpr)(int32_t)0x80246000u;
}

static gpr get_entry_function_address() {
    // Use thread1_idle as the start function: 0x80246000 + 0x00000CF0
    return (gpr)(int32_t)0x80246CF0u;
}
