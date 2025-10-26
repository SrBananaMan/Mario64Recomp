#ifndef __ZELDA_GAME_H__
#define __ZELDA_GAME_H__

#include <vector>
#include <span>
#include <cstdint>

namespace zelda64 {
    void quicksave_save();
    void quicksave_load();
    // Majora's Mask ROM decompression routine (Yaz0). Returns a decompressed ROM buffer.
    std::vector<uint8_t> decompress_mm(std::span<const uint8_t> compressed_rom);
};

#endif
