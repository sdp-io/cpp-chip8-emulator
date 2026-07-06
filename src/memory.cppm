module;

#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>

export module Memory;

namespace chip8 {

constexpr size_t Font_Size{80};

// Font data required for instruction 'FX29'
constexpr std::array<uint8_t, Font_Size> Font_Data{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

export class Memory {
public:
  Memory() { load_font(); }

  uint8_t operator[](int index) const {
    if (index < 0 || index >= RAM_Size) {
      throw std::out_of_range{"Index is out of range!"};
    }

    return memory[index];
  }

  uint8_t operator[](int index) {
    if (index < 0 || index >= RAM_Size) {
      throw std::out_of_range{"Index is out of range!"};
    }

    return memory[index];
  }

  // Load font at very beginning of reserved RAM
  void load_font(void);

  void load_rom(std::fstream &rom);

  uint8_t get_sprite_data(const uint16_t &index) { return memory[index]; }

private:
  static constexpr size_t RAM_Size{4096}; // 4KB
  std::array<uint8_t, RAM_Size> memory{};
};

void Memory::load_font(void) {
  for (auto i = 0; i < Font_Size; i++) {
    memory[i] = Font_Data[i];
  }
}

void Memory::load_rom(std::fstream &rom) {
  char byte{};
  int bytec{0};

  while (rom.get(byte)) {
    // First 512 bytes are reserved, so offset
    memory[0x200 + bytec] = byte;
    bytec++;
  }

  std::cout << "Loading complete!\n";
  std::cout << "Bytes loaded: " << bytec << "\n";
}

} // namespace chip8
