#include <cstdint>
#include <fstream>
#include <iostream>
import CPU;
import Memory;
import Display;

int main() {
  // chip8::Display display{};
  chip8::CPU cpu{};
  chip8::Memory memory{};

  std::cout << "Hello world!\n";
  std::string rom_path{"../data/roms/IBMLogo.ch8"};

  std::fstream rom{rom_path, rom.binary | rom.in};

  if (!rom.is_open()) {
    std::cout << "Error getting ROM!\n";
    return 1;
  } else {
    // Load ROM here
    memory.load_rom(rom);
  }

  // Main CHIP-8 program loop
  while (true) {
    // Fetch
    uint16_t opcode{cpu.fetch(memory)};

    // Decode
    struct chip8::Decoded_Inst di{cpu.decode(opcode)};
  }
  return 0;
}
