module;

#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>

export module Memory;

namespace chip8 {

// 4KB of RAM as per CHIP-8 specifications
constexpr size_t RAM_Size{4096};

export class Memory {
public:
  Memory() = default;

  void load_rom(std::fstream &rom);

private:
  std::array<uint8_t, RAM_Size> memory{};
};

void Memory::load_rom(std::fstream &rom) {
  char byte{};
  int bytec{0};

  while (rom.get(byte)) {
    memory[0x200 + bytec] = byte;
    bytec++;
  }

  std::cout << "Loading complete!\n";
  std::cout << "Bytes loaded: " << bytec << "\n";
}

} // namespace chip8
