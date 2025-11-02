#include <cstdio>
#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>
#include "xxhash.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: xxh3hash <file>\n";
        return 1;
    }
    const char* path = argv[1];
    std::ifstream f(path, std::ios::binary);
    if (!f.good()) { std::cerr << "Failed to open file\n"; return 1; }
    f.seekg(0, std::ios::end);
    size_t size = (size_t)f.tellg();
    f.seekg(0, std::ios::beg);
    std::vector<uint8_t> data(size);
    f.read(reinterpret_cast<char*>(data.data()), data.size());
    uint64_t h = XXH3_64bits(data.data(), data.size());
    std::printf("%016llX\n", (unsigned long long)h);
    return 0;
}
