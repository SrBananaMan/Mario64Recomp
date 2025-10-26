#include <cstdint>
#include "recomp.h"

// Forward declare before use to avoid undeclared identifier errors.
gpr get_entrypoint_address();

extern "C" void recomp_entrypoint(uint8_t * rdram, recomp_context * ctx) {
    SET_FENV_ACCESS();
    // Jump to the recompiled program's entrypoint.
    recomp_func_t* entry = LOOKUP_FUNC(get_entrypoint_address());
    entry(rdram, ctx);
}

// Provide the entrypoint address used at startup.
gpr get_entrypoint_address() {
    // Super Mario 64 (US) main section base address.
    return (gpr)(int32_t)0x80246000u;
}
