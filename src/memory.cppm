module;

#include <array>
#include <cstdint>

export module Memory;

export class Memory {
public:
  Memory() = default;

private:
  // Initialize 4KB of RAM as per CHIP-8 specifications
  std::array<uint8_t, 4096> memory{};
};
